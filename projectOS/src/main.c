#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "utils.h"


void print_main_menu() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║    CPU & RESOURCE SCHEDULING SIMULATOR                 ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║  Please select a scheduling algorithm:                 ║\n");
    printf("║  [1] FCFS (First Come First Serve)                     ║\n");
    printf("║  [2] RR   (Round Robin)                                ║\n");
    printf("║  [3] SJF  (Shortest Job First)                         ║\n");
    printf("║  [4] SRTN (Shortest Remaining Time Next)               ║\n");
    printf("║  [0] EXIT                                              ║\n");
    printf("║  Enter your choice (0-4): ");
}

void print_rr_menu() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║  ROUND ROBIN - QUANTUM SELECTION                       ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║  Enter quantum value (default=2):                      ║\n");
    printf("║  [1] Quantum = 1                                       ║\n");
    printf("║  [2] Quantum = 2 (Default)                             ║\n");
    printf("║  [3] Quantum = 3                                       ║\n");
    printf("║  [4] Quantum = 4                                       ║\n");
    printf("║  [5] Custom quantum (enter manually)                   ║\n");
    printf("║  Enter your choice (1-5): ");
}

void print_file_menu() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║  FILE SELECTION                                        ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║  [1] Use input_test.txt (Default test file)            ║\n");
    printf("║  [2] Enter custom input filename                       ║\n");
    printf("║  Enter your choice (1-2): ");
}

int main() {
    int algorithm = -1;
    int quantum = 2;  
    char input_file[256];
    char output_file[256];
    Process processes[MAX_PROCESSES];
    int n;
    
    // ========== MENU CHÍNH ==========
    while (algorithm == -1) {
        print_main_menu();
        scanf("%d", &algorithm);
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        if (algorithm == 0) {
            printf("\n✅ Exiting program...\n");
            return 0;
        }
        if (algorithm < 1 || algorithm > 4) {
            printf("\n❌ Invalid choice! Please enter 1, 2, 3, or 4.\n");
            algorithm = -1;
        }
    }
    
    // ========== CHỌN FILE INPUT ==========
    print_file_menu();
    int file_choice;
    scanf("%d", &file_choice);
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (file_choice == 1) {
        strcpy(input_file, "input_test.txt");
        printf("✓ Using default input file: input_test.txt\n");
    } else {
        printf("\nEnter input filename: ");
        fgets(input_file, sizeof(input_file), stdin);
        input_file[strcspn(input_file, "\n")] = 0;
        printf("✓ Using input file: %s\n", input_file);
    }
    
    // ========== CHỌN QUANTUM NẾU LÀ RR ==========
    if (algorithm == 2) {
        print_rr_menu();
        int quantum_choice;
        scanf("%d", &quantum_choice);
        while ((c = getchar()) != '\n' && c != EOF);
        
        if (quantum_choice >= 1 && quantum_choice <= 4) quantum = quantum_choice;
        else if (quantum_choice == 5) {
            printf("Enter custom quantum value: ");
            scanf("%d", &quantum);
            while ((c = getchar()) != '\n' && c != EOF);
        } else {
            quantum = 2;
            printf("✓ Invalid choice, using default quantum = 2\n");
        }
    }
    
    // ========== TẠO FILENAME OUTPUT ==========
    const char* algo_name = "";
    if (algorithm == 1) algo_name = "fcfs";
    else if (algorithm == 2) sprintf(output_file, "output_rr_q%d.txt", quantum);
    else if (algorithm == 3) algo_name = "sjf";
    else if (algorithm == 4) algo_name = "srtn";
    
    if (algorithm != 2) sprintf(output_file, "output_%s.txt", algo_name);
    
    // ========== ĐỌC FILE VÀ CHẠY SCHEDULER ==========
    printf("\n📖 Reading input file...\n");
    read_input(input_file, processes, &n);
    printf("✅ Read %d processes successfully!\n", n);
    
    printf("\n⏳ Running scheduler...\n");
    simulate_scheduling(processes, n, algorithm, quantum, output_file);
    
    // ========== IN KẾT QUẢ GANTT LÊN MÀN HÌNH ==========
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║                    GANTT CHART                         ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    
    FILE* fp = fopen(output_file, "r");
    if (fp != NULL) {
        char line[1000];
        printf("║ CPU:      ");
        if (fgets(line, sizeof(line), fp)) printf("%s", line);
        printf("║ Resource: ");
        if (fgets(line, sizeof(line), fp)) printf("%s", line);
        fclose(fp);
    }
    printf("╚════════════════════════════════════════════════════════╝\n");
    
    // Menu vòng lặp chơi tiếp
    printf("\n[1] Run again  |  [2] Exit : ");
    int next_choice;
    scanf("%d", &next_choice);
    if (next_choice == 1) {
        printf("\n🔄 Restarting...\n");
        return main();
    }
    return 0;
}