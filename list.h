/**
 * list data structure containing the tasks in the system
 */
 
#include "task.h"
#pragma once

struct node {
    Task *task;
    struct node *next;
};

// insert and delete operations.
void insert(struct node **head, Task *task);
// void delete(struct node **head, Task *task);
struct node **delete(struct node **head, Task *task);
void traverse(struct node *head);

// gets rid of that garbage data of a tail
void cutOffTail(struct node **head);