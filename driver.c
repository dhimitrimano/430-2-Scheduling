/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"

#define SIZE    100
#define ALGS      1

struct node **head[ALGS];
int numTasks = 0;

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

int getIndex(char **arr, char *str, int n) {
    for (int i = 0; i < n; i += 1) {
        if (arr[i] == str) {
            return i;
        }
    }
    return -1;
}

int smallest(int a, int b) {
    if (a > b) {
        return b;
    }
    return a;
}

// returns number of digits in an int
int itos(int i) {
    if (i == 0) {
        return 1;
    }
    int ret = 0;
    while (i != 0) {
        ret += 1;
        i /= 10;
    }
    return ret;
}

// does the first part of the bonus
void bonus(int curr_time, int dispatcher_time) {
    printf("\nCPU Utilization: %.2f%%\n", 100 * ((double) curr_time)/((double) dispatcher_time));
}

// does the second part of the bonus
void runBonus(char **names, int *first_sight, int *last_sight, int *burst, int n) {
    int before = -1;
    int largest = 0;
    int bigSize = 0;
    int x;
    for (int j = 0; j < n; j += 1) {
        if (comesBefore(names[largest], names[j])) {
            largest = j;
        }
        if (strlen(names[j]) > bigSize) {
            bigSize = strlen(names[j]);
        }
        if (itos(first_sight[j]) > bigSize) {
            bigSize = itos(first_sight[j]);
        }
        if (itos(last_sight[j]) > bigSize) {
            bigSize = itos(last_sight[j]);
        }
        if (itos(burst[j]) > bigSize) {
            bigSize = itos(burst[j]);
        }
    }
    int order[n]; // holds the indexes of the names ordered alphabetically by first name
    for (int i = 0; i < n; i += 1) {
        order[i] = largest;
        for (int j = 0; j < n; j += 1) {
            if ((before == -1 || comesBefore(names[before], names[j])) && comesBefore(names[j], names[order[i]])) {
                order[i] = j;
            }
        }
        before = order[i];
    }
    printf("\n...|");
    for (int i = 0; i < n; i += 1) {
        x = bigSize - strlen(names[order[i]]);
        for (int i = 0; i < x; i++) {
            printf(" ");
        }
        printf(" %s |", names[order[i]]);
    }
    printf("\nTAT|");
    for (int i = 0; i < n; i += 1) {
        x = bigSize - itos(last_sight[order[i]]) + 1;
        for (int i = 0; i < x; i++) {
            printf(" ");
        }
        printf("%d |", last_sight[order[i]]);
    }
    printf("\nWT |");
    for (int i = 0; i < n; i += 1) {
        x = bigSize - itos(last_sight[order[i]] - burst[order[i]]) + 1;
        for (int i = 0; i < x; i++) {
            printf(" ");
        }
        printf("%d |", last_sight[order[i]] - burst[order[i]]);
    }
    printf("\nRT |");
    for (int i = 0; i < n; i += 1) {
        x = bigSize - itos(first_sight[order[i]]) + 1;
        for (int i = 0; i < x; i++) {
            printf(" ");
        }
        printf("%d |", first_sight[order[i]]);
    }
    printf("\n");
}

void add(char *name, int priority, int burst) {
    for (int i = 0; i < ALGS; i++) {
        Task *newTask = malloc(sizeof(struct Task));
        newTask->name = name;
        newTask->priority = priority;
        newTask->burst = burst;
        insert(head[i], newTask);
    }
    numTasks += 1;
}

int main(int argc, char *argv[])
{
    if (!argv[1]) {
        fprintf(stderr, "ERROR: no input file of scheduled tasks");
        exit(1);
    } else if (access(argv[1], F_OK) != 0) {
        fprintf(stderr, "ERROR: input file doesn't exist");
        exit(1);
    }

    FILE *in;
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;

    in = fopen(argv[1],"r");

    for(int i = 0; i < ALGS; i++) {
        head[i] = malloc(sizeof(struct node) * ALGS * 2);
    }

    int counter = 0;
    
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        priority = atoi(strsep(&temp,","));
        burst = atoi(strsep(&temp,","));

        // add the task to the scheduler's list of tasks
        add(name,priority,burst);

        free(temp);
        counter = 1;
    }

    if (counter == 0) {
        fprintf(stderr, "ERROR: this program doesn't work with zero scheduled tasks");
        exit(1);
    }
    for (int i = 0; i < ALGS; i++) {
        cutOffTail(head[i]);
    }

    fclose(in);

    // invoke the scheduler
    schedule(*head[0], numTasks);

    return 0;
}
