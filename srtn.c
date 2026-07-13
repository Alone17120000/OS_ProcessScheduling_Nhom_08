#include "srtn.h"

int get_next_srtn(Process processes[], int n, int current_time) {
    int min_remaining = 999999;
    int selected_index = -1;

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time <= current_time && processes[i].state == 1) {
            int remaining = processes[i].remaining_cpu_time;
            if (remaining < min_remaining) {
                min_remaining = remaining;
                selected_index = i;
            } else if (remaining == min_remaining) {
                if (processes[i].is_new_arrival) {
                    selected_index = i;
                }
            }
        }
    }
    return selected_index;
}