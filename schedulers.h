#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// add a task to the list 
void add(char *name, int priority, int burst);

// invoke the scheduler
void schedule();

// bool comesBefore(char *a, char *b);
// int smallest(int a, int b);

// Task *pickNextTask_alpha(struct node *head);
// Task *pickNextTask_burst(struct node *head);
// Task *pickNextTask_priority(struct node *head);