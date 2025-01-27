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

// picks the first task, alphabetically, of a certain priority
Task *pickNextTask_alpha(struct node *head, int priority) {
    // if list is empty, nothing to do
    if (!head)
        return NULL;

    struct node *temp;
    temp = head;
    Task *ret = temp->task;
    Task *final = temp->task;

    while (temp != NULL) {
        if (comesBefore(final->name, temp->task->name) && temp->task->priority == priority) {
            final = temp->task;
        }
        temp = temp->next;
    }

    temp = head;

    ret = final;

    while (temp != NULL) {
        if (comesBefore(temp->task->name, ret->name) && temp->task->priority == priority) {
            ret = temp->task;
        }
        temp = temp->next;
    }

    return ret;
}

// picks the next task to go into round robin
Task *pickNextTask(struct node *head, char* last, int priority) {
    // if list is empty, nothing to do
    if (!head)
        return NULL;

    struct node *temp;
    temp = head;

    Task *ret = temp->task;
    Task *final = temp->task;

    while (temp != NULL) {
        if (comesBefore(final->name, temp->task->name) && temp->task->priority == priority) {
            final = temp->task;
        }
        temp = temp->next;
    }

    temp = head;

    ret = final;
    
    if (final->name == last) {
        ret = pickNextTask_alpha(head, priority);
        return ret;
    }

    while (temp != NULL) {
        if (comesBefore(last, temp->task->name) && comesBefore(temp->task->name, ret->name) && temp->task->priority == priority) {
            ret = temp->task;
        }
        temp = temp->next;
    }

    if (ret->name == head->task->name && ret->priority != priority) {
        ret = pickNextTask_alpha(head, priority);
        return ret;
    }

    return ret;
}

// does the priority round robin scheduling algorithm
void schedule(struct node *head, int n) {
    printf("\n\nBeginning priority round robin\n\n");
    struct node *temp;
    Task *toRun;

    char *names[n]; // task names, unsorted
    int first_sight[n]; // time at task's beginning
    int last_sight[n]; // time at task's end
    int burst[n]; // burst time of task
    int index = 0;

    for (int i = 10; head; i-=1) {
        temp = head;
        int num = 0;
        while (temp) {
            if (temp->task->priority == i) {
                num += 1;
                toRun = temp->task;
            }
            temp = temp->next;
        }
        if (num == 1) {
            run(toRun, toRun->burst);

            names[index] = toRun->name;
            first_sight[index] = curr_time;
            burst[index] = toRun->burst;

            curr_time += toRun->burst;
            dispatcher_time += DISPATCH;
            printf("Time is now: %d\n", curr_time);
        
            last_sight[index] = curr_time;

            delete(&head, toRun);
            free(toRun);

            index += 1;
        } else if (num > 1) {
            char *last = " ";
            temp = head;
            while (head && num != 0) {
                if (num != 0) {
                    toRun = pickNextTask(head, last, i);
                    run(toRun, smallest(toRun->burst, QUANTUM));

                    if (getIndex(names, toRun->name, n) == -1) {
                        names[index] = toRun->name;
                        first_sight[index] = curr_time;
                        burst[index] = toRun->burst;
                        index += 1;
                    }

                    last = toRun->name;
                    curr_time += smallest(toRun->burst, QUANTUM);
                    dispatcher_time += DISPATCH;
                    printf("Time is now: %d\n", curr_time);
                    toRun->burst -= smallest(toRun->burst, QUANTUM);
                    if (toRun->burst == 0) {
                        last_sight[getIndex(names, toRun->name, n)] = curr_time;
                        delete(&head, toRun);
                        free(toRun);
                        num -= 1;
                    }
                }
            }
        }
    }
    free(head);
    dispatcher_time += curr_time;
    bonus(curr_time, dispatcher_time);
    runBonus(names, first_sight, last_sight, burst, n);
}