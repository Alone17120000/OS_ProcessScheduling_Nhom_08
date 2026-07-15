#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define MAX_PROCESSES 100
#define MAX_BURSTS 20

typedef struct {
    int cpu_burst[MAX_BURSTS];
    int r_burst[MAX_BURSTS];
    int num_bursts;
} BurstCycle;

typedef struct {
    int pid;
    int arrival_time;
    BurstCycle bursts;
    int state;
    int current_burst_index;
    int remaining_cpu_time;
    int remaining_r_time;
    bool is_new_arrival;
} Process;

#endif