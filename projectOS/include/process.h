#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 100
#define MAX_BURSTS 1000  // Số lần xen kẽ CPU và Resource tối đa

// Định nghĩa các loại burst
#define CPU_BURST 1
#define RESOURCE_BURST 2

// Cấu trúc lưu thông tin một burst
typedef struct {
    int type;           // CPU_BURST hoặc RESOURCE_BURST
    int duration;       // Thời gian của burst này
} Burst;

// Cấu trúc tiến trình
typedef struct {
    int pid;                    // ID tiến trình (1-4)
    int arrival_time;           // Thời gian đến
    Burst bursts[MAX_BURSTS];   // Dãy các burst
    int burst_count;            // Số lượng burst
    int current_burst_index;    // Vị trí burst hiện tại
    int remaining_time;         // Thời gian còn lại của burst hiện tại
    int state;                  // Trạng thái: 0=ready, 1=running, 2=waiting_resource, 3=completed
} Process;

// Hàm khởi tạo tiến trình
Process* create_process(int pid, int arrival_time);

// Hàm thêm burst
void add_burst(Process* p, int type, int duration);

// Hàm lấy burst hiện tại
Burst* get_current_burst(Process* p);

// Hàm chuyển sang burst tiếp theo
int move_to_next_burst(Process* p);

// Hàm kiểm tra tiến trình đã hoàn thành chưa
int is_process_completed(Process* p);

#endif