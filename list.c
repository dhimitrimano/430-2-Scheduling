/**
 * Various list operations
 */
 
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"


// add a new task to the list of tasks
void insert(struct node **head, Task *newTask) {
    // add the new task to the list 
    struct node *newNode = malloc(sizeof(struct node));

    newNode->task = newTask;
    newNode->next = *head;
    *head = newNode;
}

struct node **delete(struct node **head, Task *task) {
    struct node *temp;
    struct node *prev;

    temp = *head;
    // special case - beginning of list
    if (strcmp(task->name,temp->task->name) == 0) {
        if ((*head)->next == NULL) {
            *head = NULL;
        } else {
            (*head)->task = NULL;
            *head = (*head)->next;
        }
    }
    else {
        // interior or last element in the list
        prev = *head;
        temp = temp->next;
        while (strcmp(task->name,temp->task->name) != 0) {
            prev = temp;
            temp = temp->next;
        }

        prev->next = temp->next;
        temp->task = NULL;
    }
    free(temp);
    return head;
}

// traverse the list
void traverse(struct node *head) {
    struct node *temp;
    temp = head;
    while (temp != NULL) {
        printf("[%s] [%d] [%d]\n",temp->task->name, temp->task->priority, temp->task->burst);
        temp = temp->next;
    }
}

// delete the last node (the empty first node that was pushed to the back by the task nodes)
void cutOffTail(struct node **head) {
    struct node *tail_cut = *head;
    while (tail_cut->next) {
        tail_cut = tail_cut->next;
    }
    free(tail_cut->next);
    tail_cut->next = NULL;
}