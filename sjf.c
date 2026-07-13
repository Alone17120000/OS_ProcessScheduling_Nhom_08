#include "sjf.h"

int get_next_sjf(Process processes[], int n, int current_time) {
    int min_time = 999999;
    int selected_index = -1;

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time <= current_time && processes[i].state == 1) { 
            int burst = processes[i].bursts.cpu_burst[processes[i].current_burst_index];
            if (burst < min_time) {
                min_time = burst;
                selected_index = i;
            } else if (burst == min_time) {
                if (processes[i].is_new_arrival) {
                    selected_index = i;
                }
            }
        }
    }
    return selected_index;
}