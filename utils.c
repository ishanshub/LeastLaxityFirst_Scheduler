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
        printf("Memory allocation failed.\n");
        exit(1);
    }

    int totalTasks;
    if (fscanf(fin, "%d", &totalTasks) != 1) {
        printf("Error reading the file.\n");
        fclose(fin);
        exit(1);
    }

    *pq = init_priority_queue(totalTasks);

    // Read tasks and add only those ready at current_time
    for (int i = 0; i < totalTasks; i++) {
        Task tempTask;

        if (fscanf(fin, " %d %d %d %d %d", &tempTask.id, &tempTask.arrival, &tempTask.period, &tempTask.WCET, &tempTask.deadline) != 4) {
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

// print tasks
void printTasks(PriorityQueue **pq, double *currentTime) {
    printf("Current Time: %f\n\n", *currentTime);

    for (int i=0; i<(*pq)->size; i++) {
        printf("Task: %d\t %d, %d, %d, %d, %f, %f\n", 

        (*pq)->data[i].id,
        (*pq)->data[i].arrival, 
        (*pq)->data[i].period, 
        (*pq)->data[i].WCET, 
        (*pq)->data[i].deadline, 
        (*pq)->data[i].execution_left,
        (*pq)->data[i].laxity);
    }
}

printSingleTask(Task task) {

}