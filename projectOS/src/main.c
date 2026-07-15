#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/queue.h"
#include "../include/process.h"
#include "../include/scheduler.h"

// Hàm đọc file input
int read_input_file(const char* filename, Process* processes[], int* process_count) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("❌ Error: Cannot open input file '%s'\n", filename);
        return 0;
    }
    
    int n;
    if (fscanf(fp, "%d", &n) != 1) {
        printf("❌ Error: Cannot read number of processes\n");
        fclose(fp);
        return 0;
    }
    
    *process_count = n;
    
    for (int i = 0; i < n; i++) {
        int arrival_time;
        if (fscanf(fp, "%d", &arrival_time) != 1) {
            printf("❌ Error: Cannot read arrival time for process %d\n", i + 1);
            fclose(fp);
            return 0;
        }
        
        // Tạo tiến trình mới
        Process* p = create_process(i + 1, arrival_time);
        
        // Đọc dãy burst
        int value;
        int is_cpu_burst = 1;  // Burst đầu tiên là CPU
        
        while (fscanf(fp, "%d", &value) == 1) {
            if (is_cpu_burst) {
                add_burst(p, CPU_BURST, value);
            } else {
                add_burst(p, RESOURCE_BURST, value);
            }
            is_cpu_burst = 1 - is_cpu_burst;  // Xen kẽ CPU và Resource
            
            // Kiểm tra xem có phải end of line không
            int ch = fgetc(fp);
            if (ch == '\n' || ch == EOF) {
                ungetc(ch, fp);
                break;
            } else {
                ungetc(ch, fp);
            }
        }
        
        processes[i] = p;
    }
    
    fclose(fp);
    return 1;
}

// Hàm hiển thị menu chính
void print_main_menu() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║    CPU & RESOURCE SCHEDULING SIMULATOR                 ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║                                                        ║\n");
    printf("║  Please select a scheduling algorithm:                 ║\n");
    printf("║                                                        ║\n");
    printf("║  [1] FCFS (First Come First Serve)                    ║\n");
    printf("║  [2] RR   (Round Robin)                               ║\n");
    printf("║  [3] SJF  (Shortest Job First) - NOT IMPLEMENTED YET  ║\n");
    printf("║  [4] SRTN (Shortest Remaining Time Next) - NOT YET    ║\n");
    printf("║  [0] EXIT                                             ║\n");
    printf("║                                                        ║\n");
    printf("║  Enter your choice (0-4): ");
}

// Hàm hiển thị menu cho RR quantum
void print_rr_menu() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║  ROUND ROBIN - QUANTUM SELECTION                      ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║                                                        ║\n");
    printf("║  Enter quantum value (default=2):                     ║\n");
    printf("║  [1] Quantum = 1                                      ║\n");
    printf("║  [2] Quantum = 2 (Default)                            ║\n");
    printf("║  [3] Quantum = 3                                      ║\n");
    printf("║  [4] Quantum = 4                                      ║\n");
    printf("║  [5] Custom quantum (enter manually)                  ║\n");
    printf("║                                                        ║\n");
    printf("║  Enter your choice (1-5): ");
}

// Hàm hiển thị menu cho input/output file
void print_file_menu() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║  FILE SELECTION                                       ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║                                                        ║\n");
    printf("║  [1] Use test/input.txt (Default test file)          ║\n");
    printf("║  [2] Enter custom input filename                      ║\n");
    printf("║                                                        ║\n");
    printf("║  Enter your choice (1-2): ");
}

// Main function
int main() {
    int algorithm = -1;
    int quantum = 2;  // Default quantum
    char input_file[256];
    char output_file[256];
    
    // ========== MENU CHÍNH ==========
    while (algorithm == -1) {
        print_main_menu();
        scanf("%d", &algorithm);
        
        // Clear input buffer
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
    printf("\n");
    print_file_menu();
    int file_choice;
    scanf("%d", &file_choice);
    
    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (file_choice == 1) {
        strcpy(input_file, "test/input.txt");
        printf("✓ Using default input file: test/input.txt\n");
    } else if (file_choice == 2) {
        printf("\nEnter input filename (e.g., test/input.txt): ");
        fgets(input_file, sizeof(input_file), stdin);
        // Remove newline
        input_file[strcspn(input_file, "\n")] = 0;
        printf("✓ Using input file: %s\n", input_file);
    } else {
        strcpy(input_file, "test/input.txt");
        printf("✓ Invalid choice, using default: test/input.txt\n");
    }
    
    // ========== CHỌN QUANTUM NẾU LÀ RR ==========
    if (algorithm == RR) {
        printf("\n");
        print_rr_menu();
        int quantum_choice;
        scanf("%d", &quantum_choice);
        
        // Clear input buffer
        while ((c = getchar()) != '\n' && c != EOF);
        
        switch (quantum_choice) {
            case 1:
                quantum = 1;
                printf("✓ Quantum = 1\n");
                break;
            case 2:
                quantum = 2;
                printf("✓ Quantum = 2 (Default)\n");
                break;
            case 3:
                quantum = 3;
                printf("✓ Quantum = 3\n");
                break;
            case 4:
                quantum = 4;
                printf("✓ Quantum = 4\n");
                break;
            case 5:
                printf("Enter custom quantum value: ");
                scanf("%d", &quantum);
                // Clear input buffer
                while ((c = getchar()) != '\n' && c != EOF);
                printf("✓ Quantum = %d\n", quantum);
                break;
            default:
                quantum = 2;
                printf("✓ Invalid choice, using default quantum = 2\n");
        }
        
        if (quantum < 1) {
            printf("❌ Quantum must be >= 1, using default = 2\n");
            quantum = 2;
        }
    }
    
    // ========== TẠO FILENAME OUTPUT ==========
    const char* algo_name = "";
    switch (algorithm) {
        case FCFS:
            algo_name = "fcfs";
            break;
        case RR:
            sprintf(output_file, "output_rr_q%d.txt", quantum);
            break;
        case SJF:
            algo_name = "sjf";
            break;
        case SRTN:
            algo_name = "srtn";
            break;
    }
    
    if (algorithm != RR) {
        sprintf(output_file, "output_%s.txt", algo_name);
    }
    
    printf("✓ Output file: %s\n", output_file);
    
    // ========== ĐỌC FILE INPUT ==========
    printf("\n");
    printf("📖 Reading input file...\n");
    
    Process* processes[MAX_PROCESSES];
    int process_count = 0;
    
    if (!read_input_file(input_file, processes, &process_count)) {
        return 1;
    }
    
    printf("✅ Read %d processes from '%s'\n", process_count, input_file);
    
    // ========== HIỂN THỊ THÔNG TIN ==========
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║                   SCHEDULING INFO                      ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║ Algorithm: ");
    
    switch (algorithm) {
        case FCFS:
            printf("FCFS (First Come First Serve)\n");
            break;
        case RR:
            printf("Round Robin (Quantum = %d)\n", quantum);
            break;
        case SJF:
            printf("SJF (Shortest Job First)\n");
            break;
        case SRTN:
            printf("SRTN (Shortest Remaining Time Next)\n");
            break;
    }
    
    printf("║ Processes: %d\n", process_count);
    printf("║ Input File: %s\n", input_file);
    printf("║ Output File: %s\n", output_file);
    printf("╚════════════════════════════════════════════════════════╝\n");
    
    // ========== CHẠY SCHEDULER ==========
    printf("\n⏳ Running scheduler...\n\n");
    
    // Tạo scheduler
    Scheduler* sched = create_scheduler(algorithm, quantum);
    
    // Thêm tiến trình vào scheduler
    for (int i = 0; i < process_count; i++) {
        add_process(sched, processes[i]);
    }
    
    // Chạy scheduler theo thuật toán
    switch (algorithm) {
        case FCFS:
            schedule_fcfs(sched);
            printf("✅ FCFS scheduling completed!\n");
            break;
        case RR:
            schedule_rr(sched);
            printf("✅ Round Robin scheduling completed!\n");
            break;
        case SJF:
            printf("❌ Error: SJF not implemented yet\n");
            return 1;
        case SRTN:
            printf("❌ Error: SRTN not implemented yet\n");
            return 1;
        default:
            printf("❌ Error: Unknown algorithm\n");
            return 1;
    }
    
    // ========== XUẤT KẾT QUẢ ==========
    export_gantt_chart(sched, output_file);
    printf("✅ Scheduling completed!\n");
    printf("✅ Output written to '%s'\n", output_file);
    
    // ========== HIỂN THỊ OUTPUT ==========
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║                    GANTT CHART                         ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    
    FILE* fp = fopen(output_file, "r");
    if (fp != NULL) {
        char line[10000];
        
        printf("║ CPU:      ");
        if (fgets(line, sizeof(line), fp)) {
            printf("%s", line);
        }
        
        printf("║ Resource: ");
        if (fgets(line, sizeof(line), fp)) {
            printf("%s", line);
        }
        
        fclose(fp);
    }
    
    printf("║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    
    // ========== MENU TIẾP THEO ==========
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║                   WHAT'S NEXT?                         ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║                                                        ║\n");
    printf("║  [1] Run another test with different algorithm        ║\n");
    printf("║  [2] Open output file                                 ║\n");
    printf("║  [3] Exit                                             ║\n");
    printf("║                                                        ║\n");
    printf("║  Enter your choice (1-3): ");
    
    int next_choice;
    scanf("%d", &next_choice);
    
    // Clear input buffer
    while ((c = getchar()) != '\n' && c != EOF);
    
    // Giải phóng bộ nhớ
    free_scheduler(sched);
    
    if (next_choice == 1) {
        printf("\n🔄 Restarting program...\n");
        printf("════════════════════════════════════════════════════════\n");
        return main();  // Gọi lại main() để chạy lại từ đầu
    } else if (next_choice == 2) {
        printf("\n📂 Output file: %s\n", output_file);
        printf("✓ Open this file with a text editor to view the Gantt chart\n");
    }
    
    printf("\n✅ Thank you for using Scheduling Simulator!\n");
    printf("════════════════════════════════════════════════════════\n");
    
    return 0;
}