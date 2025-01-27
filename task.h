/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// representation of a task
typedef struct Task {
    char *name;
    // int tid;
    int priority;
    int burst;
} Task;

void run(Task *task, int slice);

#endif
