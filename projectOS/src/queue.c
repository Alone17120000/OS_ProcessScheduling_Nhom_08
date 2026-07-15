#include <stdio.h>
#include <stdlib.h>
#include "../include/queue.h"

// Khởi tạo queue
Queue* create_queue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    return q;
}

// Thêm phần tử vào cuối queue
void enqueue(Queue* q, Process* p) {
    if (!is_full(q)) {
        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
        q->items[q->rear].process = p;
        q->size++;
    } else {
        printf("Queue is full!\n");
    }
}

// Lấy phần tử từ đầu queue
Process* dequeue(Queue* q) {
    if (is_empty(q)) {
        return NULL;
    }
    
    Process* p = q->items[q->front].process;
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    q->size--;
    return p;
}

// Lấy phần tử đầu queue mà không xóa
Process* peek(Queue* q) {
    if (is_empty(q)) {
        return NULL;
    }
    return q->items[q->front].process;
}

// Kiểm tra queue có rỗng không
int is_empty(Queue* q) {
    return q->size == 0;
}

// Kiểm tra queue có đầy không
int is_full(Queue* q) {
    return q->size == MAX_QUEUE_SIZE;
}

// Lấy kích thước queue
int queue_size(Queue* q) {
    return q->size;
}

// Xóa queue
void free_queue(Queue* q) {
    free(q);
}