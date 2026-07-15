#include "fcfs.h"

int get_next_fcfs(Process processes[], int n, int current_time) {
    int selected_idx = -1;
    int earliest_arrival = 999999; // Một số rất lớn để khởi tạo

    for (int i = 0; i < n; i++) {
        // Chỉ xét những tiến trình đang đợi CPU (state == 1)
        if (processes[i].state == 1) {
            // Chọn tiến trình đến sớm nhất
            if (processes[i].arrival_time < earliest_arrival) {
                earliest_arrival = processes[i].arrival_time;
                selected_idx = i;
            }
        }
    }
    return selected_idx;
}