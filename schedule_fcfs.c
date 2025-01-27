#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "list.h"

#define DISPATCH   1

int curr_time = 0;
int dispatcher_time = -DISPATCH;

// based on traverse from list.c
// finds the task whose name comes first in dictionary
Task *pickNextTask_alpha(struct node *head) {
    // if list is empty, nothing to do
    if (!head)
        return NULL;

    struct node *temp;
    temp = head;
    Task *ret = temp->task;

    while (temp != NULL) {
        if (comesBefore(temp->task->name, ret->name)) {
            ret = temp->task;
        }
        temp = temp->next;
    }
    return ret;
}

// does the fcfs scheduling algorithm
void schedule(struct node *head, int n) {
    printf("\n\nBeginning first come first serve\n\n");
    Task *temp;

    char *names[n]; // task names, unsorted
    int first_sight[n]; // time at task's beginning
    int last_sight[n]; // time at task's end
    int burst[n]; // burst time of task
    int index = 0;

    for (int i = 0; i < n; i++) {
        names[n] = "";
    }
    while(head) {
        temp = pickNextTask_alpha(head);
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