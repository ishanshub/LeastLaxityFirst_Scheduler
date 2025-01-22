#include <stdio.h>
#include <stdlib.h>
#include "llf.h"

void swap(Task *a, Task *b) {
    Task temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PriorityQueue *pq, int index) {
    while (index > 0) {
        int parent = (index-1)/2;
        if (pq->data[index].laxity < pq->data[parent].laxity) {
            swap(&pq->data[index], &pq->data[parent]);
            index = parent;
        }
        else {
            break;
        }
    }
}

void heapify_down(PriorityQueue *pq, int index) {
    int left = 2*index+1;
    int right = 2*index+2;
    int smallest = index;

    if (left < pq->size && pq->data[left].laxity < pq->data[smallest].laxity) {
        smallest = left;
    }
    if (right < pq->size && pq->data[right].laxity < pq->data[smallest].laxity) {
        smallest = right;
    }

    if (smallest != index) {
        swap(&pq->data[index], &pq->data[smallest]);
        heapify_down(pq, smallest);
    }
}

// init pq
PriorityQueue* init_priority_queue(int capacity) {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    if (!pq) {
        printf("Memory allocation failed for PriorityQueue.\n");
        exit(1);
    }
    pq->data = (Task *)malloc(capacity * sizeof(Task));
    if (!pq->data) {
        printf("Memory allocation failed for PriorityQueue data.\n");
        free(pq);
        exit(1);
    }
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

// enqueue
void enqueue(PriorityQueue *pq, Task task) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->data = (Task *)realloc(pq->data, pq->capacity * sizeof(Task));
        if (!pq->data) {
            printf("Memory reallocation failed for PriorityQueue.\n");
            exit(1);
        }
    }

    pq->data[pq->size] = task;
    heapify_up(pq, pq->size);
    pq->size++;
}

// dequeue
Task dequeue(PriorityQueue *pq) {
    if (pq->size == 0 ) {
        printf("Priority Queue is empty.\n");
        exit(1);
    }

    Task top = pq->data[0];
    pq->data[0] = pq->data[pq->size-1];
    pq->size--;
    heapify_down(pq, 0);
    return top;

}

//clean up
void free_prioirty_queue(PriorityQueue *pq) {
    free(pq->data);
    free(pq);
}