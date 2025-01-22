#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include "llf.h" 

double generateExeTime(double lower, double upper) {
    return ((double)rand() / RAND_MAX) * (upper - lower) + lower;
}

double get_laxity(Task *task, double curr_time) {
    return ((double)(*task).deadline - curr_time - (*task).execution_left);
}

// modified read tasks from file
void addReadyTasksToQueue(const char *filename, PriorityQueue **pq, int currentTime) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        printf("Error opening file.\n");
        exit(1);
    }

    int totalTasks;
    if (fscanf(fin, "%d", &totalTasks) != 1) {
        printf("Error reading task count.\n");
        fclose(fin);
        exit(1);
    }

    *pq = init_priority_queue(totalTasks);

    // Read tasks and add only those ready at current_time
    for (int i = 0; i < totalTasks; i++) {
        Task tempTask;

        if (fscanf(fin, " %d %d %d %d %d", 
        &tempTask.id, 
        &tempTask.arrival, 
        &tempTask.period, 
        &tempTask.WCET, 
        &tempTask.deadline) != 5) {
            printf("Error reading task data.\n");
            fclose(fin);
            exit(1);
        }

        // Check if the task is ready to execute
        if (tempTask.arrival <= currentTime) {
            tempTask.instance_cnt = 1;
            tempTask.execution_left = generateExeTime(tempTask.WCET * 0.5, tempTask.WCET);
            tempTask.laxity = get_laxity(&tempTask, (double)currentTime);

            enqueue(*pq, tempTask);
        }
    }

    fclose(fin);
}

int checkDeadlineMiss(PriorityQueue *pq, double currentTime) {
    for (int i=0; i<pq->size; i++) {
        Task currTask = pq->data[i];

        double remainingTimeBeforeDeadline = (double)currTask.deadline - currentTime;
        if (currTask.execution_left > remainingTimeBeforeDeadline) {
            printf("Error: Task ID: %d missed its deadline at Time: %.2f\n", currTask.id, currentTime);
            printf("Task missed its deadline by %.2f units.\n", currTask.execution_left - remainingTimeBeforeDeadline);
            return 1;
        }
    }
    return 0;
}

// print tasks
void printTasks(PriorityQueue **pq, double *currentTime) {
    printf("Current Time: %.2f\n", *currentTime);

    // Loop through all tasks in the priority queue
    for (int i = 0; i < (*pq)->size; i++) {
        Task task = (*pq)->data[i];
        printf("Task ID: %d | Arrival: %d | WCET: %d | Deadline: %d | Exec Left: %.2f | Laxity: %.2f | Instances: %d\n", 
               task.id, task.arrival, task.WCET, task.deadline, task.execution_left, task.laxity, task.instance_cnt);
    }

    printf("\n");
}

void printSingleTask(Task task) {
    printf("Task ID: %d | Arrival: %d | WCET: %d | Deadline: %d | Exec Left: %.2f | Laxity: %.2f | Instances: %d\n", 
           task.id, task.arrival, task.WCET, task.deadline, task.execution_left, task.laxity, task.instance_cnt);
}
