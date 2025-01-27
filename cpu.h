#include "task.h"

// length of a time quantum
#define QUANTUM 10

// run the specified task for the following time slice
void run(Task *task, int slice);

// perform the bonus CPU utilization
void bonus(int curr_time, int dispatcher_time);

// perform the bonus print
void runBonus(char **names, int *first_sight, int *last_sight, int *burst, int n);

// returns whether or not a comes before b alphabetically
bool comesBefore(char *a, char *b);

// returns the index of the string in the char **array, or -1 if it isn't there
int getIndex(char **arr, char *str, int n);

// returns the smallest of two integers
int smallest(int a, int b);