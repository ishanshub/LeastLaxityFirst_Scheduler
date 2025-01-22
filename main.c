#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <math.h>
#include "llf.h" 

void run_scheduler(const char *filename, PriorityQueue **pq, double *currentTime, int timeQuanta) {

    addReadyTasksToQueue(filename, pq, *currentTime);

    if (*pq == NULL || (*pq)->size == 0) {
        printf("No tasks in the queue.\n");
        return;
    }

    while (*pq != NULL && (*pq)->size > 0) {
        Task currTask = dequeue(*pq);

        printf("Executing Task ID: %d\tTime: %.2f\n", currTask.id, *currentTime);
        printf("Before Execution\n");
        printSingleTask(currTask);

        double executedTime = fmin(currTask.execution_left, timeQuanta);
        currTask.execution_left -= executedTime;
        *currentTime += executedTime;

        if (checkDeadlineMiss(*pq, *currentTime)) {
            return;
        }

        printf("After Execution.\n");
        if (currTask.execution_left <= 0) {
            currTask.instance_cnt++;

            if (currTask.instance_cnt <= 5) {
                currTask.arrival += currTask.period;
                currTask.deadline = currTask.arrival + currTask.period;

                currTask.execution_left = generateExeTime(currTask.WCET * 0.5, currTask.WCET);
                currTask.laxity = get_laxity(&currTask, *currentTime);

                printf("Task ID: %d completed. Remaining Time Before Deadline: %.2f\n", currTask.id, currTask.deadline - *currentTime);
                enqueue(*pq, currTask);
            }
        }
        else {
            currTask.laxity = get_laxity(&currTask, *currentTime);
            enqueue(*pq, currTask);
        }

        printSingleTask(currTask);
        printf("\n");
    }

    printf("\n---All Jobs Completed---\n");
        return;
}

int main(int argc, char *argv[]) {

    double currentTime = 0.0;

    PriorityQueue *pq = NULL;

    if (argc < 2) {
        printf("Enter file name in cmd.\n");
        return 1;
    }

    //run the scheduler
    run_scheduler(argv[1], &pq, &currentTime, 1);
    //printTasks(&pq, &currentTime);

    free_prioirty_queue(pq);

    return 0;
}