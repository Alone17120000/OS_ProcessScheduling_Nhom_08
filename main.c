#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Loi: Thieu tham so!\n");
        printf("Cach dung: %s <Input_file> <Output_file> <Algorithm>\n", argv[0]);
        return 1;
    }

    char *inputFile = argv[1];
    char *outputFile = argv[2];
    int algo_type = atoi(argv[3]);
    Process processes[MAX_PROCESSES];
    int n;

    read_input(inputFile, processes, &n);

    printf("--- DU LIEU DOC TU FILE: %s ---\n", inputFile);
    printf("So tien trinh: %d\n", n);
    for (int i = 0; i < n; i++) {
        printf("P%d | Arrival: %d | Bursts: ", processes[i].pid, processes[i].arrival_time);
        for (int j = 0; j < processes[i].bursts.num_bursts; j++) {
            printf("[CPU: %d, R: %d] ", processes[i].bursts.cpu_burst[j], processes[i].bursts.r_burst[j]);
        }
        printf("\n");
    }
    printf("--------------------------------------\n");

    simulate_scheduling(processes, n, algo_type, outputFile);

    return 0;
}