#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "list.h"

#define DISPATCH   1

int curr_time = 0;
int dispatcher_time = -DISPATCH;

Task *pickNextTask_burst(struct node *head) {
    if (!head) {
        return NULL;
    }
    // traverse(head);
    struct node *temp = head;
    Task *ret = temp->task;
    while (temp != NULL) {
        if (temp->task->burst < ret->burst || (temp->task->burst == ret->burst && comesBefore(temp->task->name, ret->name))) {
            ret = temp->task;
        }
        temp = temp->next;
    }
    return ret;
}

// performs the sjf scheduling algorithm
void schedule(struct node *head, int n) {
    printf("\n\nBeginning shortest job first\n\n");
    Task *temp;

    char *names[n]; // task names, unsorted
    int first_sight[n]; // time at task's beginning
    int last_sight[n]; // time at task's end
    int burst[n]; // burst time of task
    int index = 0;

    while(head) {
        temp = pickNextTask_burst(head);
        run(temp, temp->burst);

        names[index] = temp->name;
        first_sight[index] = curr_time;
        burst[index] = temp->burst;

        curr_time += temp->burst;
        dispatcher_time += DISPATCH;
        printf("Time is now: %d\n", curr_time);
        
        last_sight[index] = curr_time;

        delete (&head, temp);
        free(temp);

        index += 1;
    }
    free(head);
    dispatcher_time += curr_time;
    bonus(curr_time, dispatcher_time);
    runBonus(names, first_sight, last_sight, burst, n);
}