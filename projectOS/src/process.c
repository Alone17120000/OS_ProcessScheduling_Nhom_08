#include <stdio.h>
#include <stdlib.h>
#include "../include/process.h"

// Hàm khởi tạo tiến trình
Process* create_process(int pid, int arrival_time) {
    Process* p = (Process*)malloc(sizeof(Process));
    p->pid = pid;
    p->arrival_time = arrival_time;
    p->burst_count = 0;
    p->current_burst_index = 0;
    p->remaining_time = 0;
    p->state = 0;  // ready
    return p;
}

// Hàm thêm burst vào tiến trình
void add_burst(Process* p, int type, int duration) {
    if (p->burst_count < MAX_BURSTS) {
        p->bursts[p->burst_count].type = type;
        p->bursts[p->burst_count].duration = duration;
        p->burst_count++;
    }
}

// Hàm lấy burst hiện tại
Burst* get_current_burst(Process* p) {
    if (p->current_burst_index < p->burst_count) {
        return &(p->bursts[p->current_burst_index]);
    }
    return NULL;
}

// Hàm chuyển sang burst tiếp theo
int move_to_next_burst(Process* p) {
    p->current_burst_index++;
    if (p->current_burst_index < p->burst_count) {
        p->remaining_time = p->bursts[p->current_burst_index].duration;
        return 1;  // Còn burst tiếp theo
    }
    return 0;  // Không còn burst nữa
}

// Hàm kiểm tra tiến trình đã hoàn thành chưa
int is_process_completed(Process* p) {
    return (p->current_burst_index >= p->burst_count);
}