#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/scheduler.h"

// Khởi tạo scheduler
Scheduler* create_scheduler(int algorithm, int quantum) {
    Scheduler* sched = (Scheduler*)malloc(sizeof(Scheduler));
    sched->process_count = 0;
    sched->algorithm = algorithm;
    sched->quantum = quantum;
    sched->ready_queue = create_queue();
    sched->resource_queue = create_queue();
    sched->gantt_length = 0;
    
    // Khởi tạo Gantt chart (-1 = chưa được gán)
    for (int i = 0; i < MAX_TIME_UNITS; i++) {
        sched->cpu_gantt[i] = -1;
        sched->resource_gantt[i] = -1;
    }
    
    return sched;
}

// Thêm tiến trình vào scheduler
void add_process(Scheduler* sched, Process* p) {
    if (sched->process_count < MAX_PROCESSES) {
        sched->processes[sched->process_count] = p;
        sched->process_count++;
    }
}

// ============================================
// FCFS (First Come First Serve) Scheduler
// ============================================
void schedule_fcfs(Scheduler* sched) {
    Queue* ready_queue = sched->ready_queue;
    Queue* resource_queue = sched->resource_queue;
    
    int time = 0;
    
    // Biến theo dõi trạng thái
    Process* current_cpu = NULL;
    Process* current_resource = NULL;
    
    int total_processes = sched->process_count;
    int completed_count = 0;
    
    while (completed_count < total_processes) {
        // Bước 1: Thêm các tiến trình mới đến vào ready queue
        for (int i = 0; i < sched->process_count; i++) {
            Process* p = sched->processes[i];
            if (p->arrival_time == time && p->current_burst_index == 0) {
                // Tiến trình đầu tiên đến
                p->current_burst_index = 0;
                p->remaining_time = p->bursts[0].duration;
                enqueue(ready_queue, p);
            }
        }
        
        // Bước 2: Xử lý Resource R (FCFS)
        if (current_resource == NULL && !is_empty(resource_queue)) {
            current_resource = dequeue(resource_queue);
        }
        
        if (current_resource != NULL) {
            sched->resource_gantt[time] = current_resource->pid;
            current_resource->remaining_time--;
            
            // Nếu Resource burst xong
            if (current_resource->remaining_time == 0) {
                // Di chuyển sang burst tiếp theo
                if (move_to_next_burst(current_resource)) {
                    // Còn burst tiếp theo
                    current_resource->remaining_time = current_resource->bursts[current_resource->current_burst_index].duration;
                    if (current_resource->bursts[current_resource->current_burst_index].type == CPU_BURST) {
                        // Quay lại CPU
                        enqueue(ready_queue, current_resource);
                    }
                } else {
                    // Tiến trình xong hết
                    completed_count++;
                }
                current_resource = NULL;
            }
        } else {
            sched->resource_gantt[time] = 0;  // 0 = idle (sẽ in dấu _)
        }
        
        // Bước 3: Xử lý CPU
        if (current_cpu == NULL && !is_empty(ready_queue)) {
            current_cpu = dequeue(ready_queue);
        }
        
        if (current_cpu != NULL) {
            sched->cpu_gantt[time] = current_cpu->pid;
            current_cpu->remaining_time--;
            
            // Nếu CPU burst xong
            if (current_cpu->remaining_time == 0) {
                // Di chuyển sang burst tiếp theo
                if (move_to_next_burst(current_cpu)) {
                    // Còn burst tiếp theo
                    current_cpu->remaining_time = current_cpu->bursts[current_cpu->current_burst_index].duration;
                    if (current_cpu->bursts[current_cpu->current_burst_index].type == RESOURCE_BURST) {
                        // Cần Resource
                        enqueue(resource_queue, current_cpu);
                    }
                } else {
                    // Tiến trình xong hết
                    completed_count++;
                }
                current_cpu = NULL;
            }
        } else {
            sched->cpu_gantt[time] = 0;  // 0 = idle
        }
        
        time++;
        sched->gantt_length = time;
        
        // Bảo vệ vô hạn loop
        if (time > MAX_TIME_UNITS) {
            printf("Error: Scheduling exceeded maximum time units!\n");
            break;
        }
    }
}

// ============================================
// Round Robin (RR) Scheduler
// ============================================
void schedule_rr(Scheduler* sched) {
    Queue* ready_queue = sched->ready_queue;
    Queue* resource_queue = sched->resource_queue;
    int quantum = sched->quantum;
    
    int time = 0;
    int total_processes = sched->process_count;
    int completed_count = 0;
    
    // Biến theo dõi trạng thái
    Process* current_cpu = NULL;
    Process* current_resource = NULL;
    int cpu_quantum_used = 0;  // Quantum đã sử dụng cho burst hiện tại
    
    while (completed_count < total_processes) {
        // Bước 1: Thêm các tiến trình mới đến vào ready queue
        for (int i = 0; i < sched->process_count; i++) {
            Process* p = sched->processes[i];
            if (p->arrival_time == time && p->current_burst_index == 0) {
                p->current_burst_index = 0;
                p->remaining_time = p->bursts[0].duration;
                enqueue(ready_queue, p);
            }
        }
        
        // Bước 2: Xử lý Resource R (FCFS - không có quantum)
        if (current_resource == NULL && !is_empty(resource_queue)) {
            current_resource = dequeue(resource_queue);
        }
        
        if (current_resource != NULL) {
            sched->resource_gantt[time] = current_resource->pid;
            current_resource->remaining_time--;
            
            // Nếu Resource burst xong
            if (current_resource->remaining_time == 0) {
                // Di chuyển sang burst tiếp theo
                if (move_to_next_burst(current_resource)) {
                    // Còn burst tiếp theo
                    if (current_resource->bursts[current_resource->current_burst_index].type == CPU_BURST) {
                        current_resource->remaining_time = current_resource->bursts[current_resource->current_burst_index].duration;
                        // Quay lại CPU
                        enqueue(ready_queue, current_resource);
                    }
                } else {
                    // Tiến trình xong hết
                    completed_count++;
                }
                current_resource = NULL;
            }
        } else {
            sched->resource_gantt[time] = 0;  // 0 = idle
        }
        
        // Bước 3: Xử lý CPU (với Round Robin)
        // Kiểm tra nếu CPU burst xong hoặc hết quantum
        if (current_cpu == NULL || cpu_quantum_used >= quantum || current_cpu->remaining_time == 0) {
            if (current_cpu != NULL && current_cpu->remaining_time > 0) {
                // CPU burst chưa xong nhưng hết quantum -> xếp lại vào queue
                enqueue(ready_queue, current_cpu);
            }
            
            // Lấy tiến trình tiếp theo từ ready queue
            if (!is_empty(ready_queue)) {
                current_cpu = dequeue(ready_queue);
                cpu_quantum_used = 0;
            } else {
                current_cpu = NULL;
            }
        }
        
        if (current_cpu != NULL) {
            sched->cpu_gantt[time] = current_cpu->pid;
            current_cpu->remaining_time--;
            cpu_quantum_used++;
            
            // Nếu CPU burst xong
            if (current_cpu->remaining_time == 0) {
                // Di chuyển sang burst tiếp theo
                if (move_to_next_burst(current_cpu)) {
                    // Còn burst tiếp theo
                    current_cpu->remaining_time = current_cpu->bursts[current_cpu->current_burst_index].duration;
                    if (current_cpu->bursts[current_cpu->current_burst_index].type == RESOURCE_BURST) {
                        // Cần Resource
                        enqueue(resource_queue, current_cpu);
                    }
                } else {
                    // Tiến trình xong hết
                    completed_count++;
                }
                current_cpu = NULL;
                cpu_quantum_used = 0;
            }
        } else {
            sched->cpu_gantt[time] = 0;  // 0 = idle
        }
        
        time++;
        sched->gantt_length = time;
        
        // Bảo vệ vô hạn loop
        if (time > MAX_TIME_UNITS) {
            printf("Error: Scheduling exceeded maximum time units!\n");
            break;
        }
    }
}

// ============================================
// Xuất Gantt Chart ra file
// ============================================
void export_gantt_chart(Scheduler* sched, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Cannot create output file!\n");
        return;
    }
    
    // Xuất CPU Gantt chart
    for (int i = 0; i < sched->gantt_length; i++) {
        if (sched->cpu_gantt[i] == 0) {
            fprintf(fp, "_");
        } else {
            fprintf(fp, "%d", sched->cpu_gantt[i]);
        }
        
        if (i < sched->gantt_length - 1) {
            fprintf(fp, " ");
        }
    }
    fprintf(fp, "\n");
    
    // Xuất Resource Gantt chart
    for (int i = 0; i < sched->gantt_length; i++) {
        if (sched->resource_gantt[i] == 0) {
            fprintf(fp, "_");
        } else {
            fprintf(fp, "%d", sched->resource_gantt[i]);
        }
        
        if (i < sched->gantt_length - 1) {
            fprintf(fp, " ");
        }
    }
    fprintf(fp, "\n");
    
    fclose(fp);
}

// Giải phóng scheduler
void free_scheduler(Scheduler* sched) {
    free_queue(sched->ready_queue);
    free_queue(sched->resource_queue);
    for (int i = 0; i < sched->process_count; i++) {
        free(sched->processes[i]);
    }
    free(sched);
}