#ifndef RR_H
#define RR_H
#include "types.h"

int get_next_rr(Process processes[], int n, int current_time, int quantum, int *time_spent);

#endif