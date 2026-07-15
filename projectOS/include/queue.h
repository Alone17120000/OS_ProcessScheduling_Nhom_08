#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

#define MAX_QUEUE_SIZE 100

// Node trong queue
typedef struct {
    Process* process;
} Node;

// Cấu trúc Queue (FIFO)
typedef struct {
    Node items[MAX_QUEUE_SIZE];
    int front;                  // Chỉ số phần tử đầu
    int rear;                   // Chỉ số phần tử cuối
    int size;                   // Số phần tử hiện tại
} Queue;

// Khởi tạo queue
Queue* create_queue();

// Thêm phần tử vào cuối queue (enqueue)
void enqueue(Queue* q, Process* p);

// Lấy phần tử từ đầu queue (dequeue)
Process* dequeue(Queue* q);

// Lấy phần tử đầu queue mà không xóa
Process* peek(Queue* q);

// Kiểm tra queue có rỗng không
int is_empty(Queue* q);

// Kiểm tra queue có đầy không
int is_full(Queue* q);

// Lấy kích thước queue
int queue_size(Queue* q);

// Xóa queue
void free_queue(Queue* q);

#endif