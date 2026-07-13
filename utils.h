#ifndef UTILS_H
#define UTILS_H
#include "types.h"

void read_input(const char *filename, Process processes[], int *n);
void simulate_scheduling(Process processes[], int n, int algo_type, const char *output_filename);

#endif