#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

int main(int argc, char *argv[]) {
    // Kiểm tra xem người dùng có nhập đủ ít nhất 3 tham số không
    if (argc < 4) {
        printf("Loi: Thieu tham so!\n");
        printf("Cach dung: %s <Input_file> <Output_file> <Algorithm> [Quantum]\n", argv[0]);
        return 1;
    }

    char *inputFile = argv[1];
    char *outputFile = argv[2];
    int algorithm = atoi(argv[3]);
    int quantum = (argc >= 5) ? atoi(argv[4]) : 2; // Mặc định quantum = 2 nếu không nhập

    printf("--- THONG TIN KHOI CHAY ---\n");
    printf("Input File: %s\n", inputFile);
    printf("Output File: %s\n", outputFile);
    printf("Algorithm ID: %d\n", algorithm);
    if (algorithm == 2) {
        printf("Quantum: %d\n", quantum);
    }

    // TODO: Gọi hàm đọc file input.txt ở đây
    // TODO: Gọi hàm chạy thuật toán định thời ở đây
    
    return 0;
}