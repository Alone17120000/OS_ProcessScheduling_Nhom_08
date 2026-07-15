#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "sjf.h"
#include "srtn.h"
#include "fcfs.h"
#include "rr.h"

void read_input(const char *filename, Process processes[], int *n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Loi mo file!\n");
        exit(1);
    }
    fscanf(file, "%d", n);
    for (int i = 0; i < *n; i++) {
        processes[i].pid = i + 1;
        processes[i].state = 0; 
        processes[i].current_burst_index = 0;
        processes[i].is_new_arrival = false;
        
        fscanf(file, "%d", &processes[i].arrival_time);
        char c;
        int burst_idx = 0;
        while (fscanf(file, "%d%c", &processes[i].bursts.cpu_burst[burst_idx], &c) == 2) {
            if (c == '\n' || c == '\r' || feof(file)) {
                processes[i].bursts.r_burst[burst_idx] = 0;
                burst_idx++;
                break;
            }
            fscanf(file, "%d%c", &processes[i].bursts.r_burst[burst_idx], &c);
            burst_idx++;
            if (c == '\n' || c == '\r' || feof(file)) break;
        }
        processes[i].bursts.num_bursts = burst_idx;
    }
    fclose(file);
}

void simulate_scheduling(Process processes[], int n, int algo_type, int quantum, const char *output_filename) {
    int current_time = 0;
    int completed_processes = 0;
    int current_cpu_process = -1;
    int current_r_process = -1;
    int rr_timer = 0;

    int gantt_cpu[1000];
    int gantt_r[1000];
    
    for(int i = 0; i < 1000; i++) {
        gantt_cpu[i] = -1;
        gantt_r[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        processes[i].remaining_cpu_time = processes[i].bursts.cpu_burst[0];
        processes[i].remaining_r_time = processes[i].bursts.r_burst[0];
    }

    while (completed_processes < n && current_time < 1000) {
        
        // 1. Cập nhật tiến trình mới đến hàng đợi
        for (int i = 0; i < n; i++) {
            if (processes[i].state == 0 && processes[i].arrival_time == current_time) {
                processes[i].state = 1;
                processes[i].is_new_arrival = true;
            }
        }

        // 2. Xử lý tài nguyên R
        if (current_r_process != -1) {
            processes[current_r_process].remaining_r_time--;
            if (processes[current_r_process].remaining_r_time == 0) {
                processes[current_r_process].current_burst_index++;
                int next_idx = processes[current_r_process].current_burst_index;
                
                if (next_idx < processes[current_r_process].bursts.num_bursts) {
                    processes[current_r_process].state = 1; // Quay lại Ready Queue chờ CPU
                    processes[current_r_process].is_new_arrival = true;
                    processes[current_r_process].remaining_cpu_time = processes[current_r_process].bursts.cpu_burst[next_idx];
                    processes[current_r_process].remaining_r_time = processes[current_r_process].bursts.r_burst[next_idx];
                } else {
                    processes[current_r_process].state = 5; // Hoàn thành
                    completed_processes++;
                }
                current_r_process = -1; // Giải phóng R
            }
        }

        // 3. Xử lý CPU (Đã Fix Lỗi Vòng Lặp)
        if (current_cpu_process != -1) {
            processes[current_cpu_process].remaining_cpu_time--;
            rr_timer++;

            // ƯU TIÊN 1: Kiểm tra xem đã chạy xong chu kỳ CPU hiện tại chưa
            if (processes[current_cpu_process].remaining_cpu_time == 0) {
                int burst_idx = processes[current_cpu_process].current_burst_index;
                
                if (processes[current_cpu_process].bursts.r_burst[burst_idx] > 0) {
                    processes[current_cpu_process].state = 3; // Chuyển sang Wait R
                } else {
                    processes[current_cpu_process].current_burst_index++;
                    int next_idx = processes[current_cpu_process].current_burst_index;
                    
                    if (next_idx < processes[current_cpu_process].bursts.num_bursts) {
                        processes[current_cpu_process].state = 1;
                        processes[current_cpu_process].remaining_cpu_time = processes[current_cpu_process].bursts.cpu_burst[next_idx];
                        processes[current_cpu_process].remaining_r_time = processes[current_cpu_process].bursts.r_burst[next_idx];
                    } else {
                        processes[current_cpu_process].state = 5; // Hoàn thành toàn bộ
                        completed_processes++;
                    }
                }
                current_cpu_process = -1; // Nhả CPU
                rr_timer = 0;
            } 
            // ƯU TIÊN 2: Nếu chưa xong, kiểm tra xem có bị cướp quyền (Preemptive) không
            else {
                if (algo_type == 4) { // SRTN
                    int next_candidate = get_next_srtn(processes, n, current_time);
                    if (next_candidate != -1 && next_candidate != current_cpu_process) {
                        // Chỉ cướp quyền nếu thời gian ngắn hơn, hoặc bằng nhưng là tiến trình mới đến (ưu tiên theo đề)
                        if (processes[next_candidate].remaining_cpu_time < processes[current_cpu_process].remaining_cpu_time || 
                           (processes[next_candidate].remaining_cpu_time == processes[current_cpu_process].remaining_cpu_time && processes[next_candidate].is_new_arrival)) {
                            
                            processes[current_cpu_process].state = 1; // Bị đẩy về Ready
                            current_cpu_process = next_candidate;     // Kẻ thách thức chiếm CPU
                            processes[current_cpu_process].state = 2; 
                            rr_timer = 0;
                        }
                    }
                } else if (algo_type == 2 && rr_timer == quantum) { // Round Robin
                    processes[current_cpu_process].state = 1; // Hết Quantum, về Ready
                    current_cpu_process = -1;
                    rr_timer = 0;
                }
            }
        }

        // 4. Nếu CPU đang trống, gọi hàm chọn tiến trình
        if (current_cpu_process == -1) {
            if (algo_type == 1) current_cpu_process = get_next_fcfs(processes, n, current_time);
            else if (algo_type == 2) current_cpu_process = get_next_rr(processes, n, current_time, quantum, &rr_timer);
            else if (algo_type == 3) current_cpu_process = get_next_sjf(processes, n, current_time);
            else if (algo_type == 4) current_cpu_process = get_next_srtn(processes, n, current_time);
            
            if (current_cpu_process != -1) processes[current_cpu_process].state = 2; // Đánh dấu Running
        }

        // 5. Nếu R đang trống, cấp FCFS cho ai đang đợi
        if (current_r_process == -1) {
            for (int i = 0; i < n; i++) {
                if (processes[i].state == 3) {
                    current_r_process = i;
                    processes[i].state = 4;
                    break;
                }
            }
        }

        // 6. Ghi nhận biểu đồ Gantt
        gantt_cpu[current_time] = current_cpu_process;
        gantt_r[current_time] = current_r_process;

        // Reset cờ new_arrival cho giây tiếp theo
        for (int i = 0; i < n; i++) processes[i].is_new_arrival = false;
        
        current_time++;
    }

    // 7. Xuất file kết quả
    FILE *out_file = fopen(output_filename, "w");
    if (out_file) {
        for(int i = 0; i < current_time; i++) fprintf(out_file, "%s ", (gantt_cpu[i] != -1) ? (char[]){(char)(processes[gantt_cpu[i]].pid + '0'), 0} : "_");
        fprintf(out_file, "\n");
        for(int i = 0; i < current_time; i++) fprintf(out_file, "%s ", (gantt_r[i] != -1) ? (char[]){(char)(processes[gantt_r[i]].pid + '0'), 0} : "_");
        fclose(out_file);
        printf("Da ghi so do Gantt thanh cong vao file %s!\n", output_filename);
    } else {
        printf("Loi khong the tao file output!\n");
    }
}