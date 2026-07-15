#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "queue.h"

#define MAX_TIME_UNITS 10000  // Tối đa số đơn vị thời gian trong Gantt chart

// Định nghĩa thuật toán
#define FCFS 1
#define RR   2
#define SJF  3
#define SRTN 4

// Cấu trúc Scheduler
typedef struct {
    Process* processes[MAX_PROCESSES];
    int process_count;
    
    Queue* ready_queue;
    Queue* resource_queue;
    
    int cpu_gantt[MAX_TIME_UNITS];          // Gantt chart của CPU
    int resource_gantt[MAX_TIME_UNITS];     // Gantt chart của Resource R
    int gantt_length;                        // Độ dài Gantt chart
    
    int quantum;                             // Quantum cho Round Robin
    int algorithm;                           // Loại thuật toán (1-4)
} Scheduler;

// Khởi tạo scheduler
Scheduler* create_scheduler(int algorithm, int quantum);

// Thêm tiến trình vào scheduler
void add_process(Scheduler* sched, Process* p);

// Chạy scheduling FCFS
void schedule_fcfs(Scheduler* sched);

// Chạy scheduling Round Robin
void schedule_rr(Scheduler* sched);

// Xuất Gantt chart ra file
void export_gantt_chart(Scheduler* sched, const char* filename);

// Giải phóng scheduler
void free_scheduler(Scheduler* sched);

#endif