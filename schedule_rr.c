#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "list.h"

#define DISPATCH   1
#define QUANTUM    10

int curr_time = 0;
int dispatcher_time = -DISPATCH;

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

Task *pickNextTask(struct node *head, char* last) {
    // if list is empty, nothing to do
    if (!head)
        return NULL;

    struct node *temp;
    temp = head;

    Task *ret = temp->task;
    Task *final = temp->task;

    while (temp != NULL) {
        if (comesBefore(final->name, temp->task->name)) {
            final = temp->task;
        }
        temp = temp->next;
    }

    temp = head;

    ret = final;
    
    if (final->name == last) {
        ret = pickNextTask_alpha(head);
        return ret;
    }

    while (temp != NULL) {
        if (comesBefore(last, temp->task->name) && comesBefore(temp->task->name, ret->name)) {
            ret = temp->task;
        }
        temp = temp->next;
    }

    return ret;
}

// performs the round robin scheduling algorithm
void schedule(struct node *head, int n) {
    printf("\n\nBeginning round robin\n\n");
    Task *temp;
    char* last = " ";

    char *names[n]; // task names, unsorted
    int first_sight[n]; // time at task's beginning
    int last_sight[n]; // time at task's end
    int burst[n]; // burst time of task
    int index = 0;

    while(head) {
        temp = pickNextTask(head, last);
        last = temp->name;
        run(temp, smallest(temp->burst, QUANTUM));

        if (getIndex(names, temp->name, n) == -1) {
            names[index] = temp->name;
            first_sight[index] = curr_time;
            burst[index] = temp->burst;
            index += 1;
        }

        curr_time += smallest(temp->burst, QUANTUM);
        dispatcher_time += DISPATCH;
        printf("Time is now: %d\n", curr_time);
        temp->burst -= smallest(temp->burst, QUANTUM);

        if (temp->burst == 0) {
            last_sight[getIndex(names, temp->name, n)] = curr_time;
            delete(&head, temp);
            free(temp);
        }
    }
    free(head);
    dispatcher_time += curr_time;
    bonus(curr_time, dispatcher_time);
    runBonus(names, first_sight, last_sight, burst, n);
}