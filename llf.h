#ifndef LLF
#define LLF

typedef struct {
    int id;
    int arrival;
    int period;
    int WCET;
    int deadline;
    double execution_left;
    double laxity; // deadline - current_time - execution_left
    int instance_cnt;

} Task;

typedef struct {
    Task *data;
    int size;
    int capacity;
} PriorityQueue;

// Function Declaration
void addReadyTasksToQueue(const char *filename, PriorityQueue **pq, int currentTime);
int checkDeadlineMiss(PriorityQueue *pq, double currentTime);
void printTasks(PriorityQueue **pq, double *currentTime);
void printSingleTask(Task task);
double generateExeTime(double lower, double upper);
double get_laxity(Task *task, double curr_time);
double generateExeTime(double lower, double upper);

PriorityQueue* init_priority_queue(int capacity);
void enqueue(PriorityQueue *pq, Task task);
Task dequeue(PriorityQueue *pq);
void free_prioirty_queue(PriorityQueue *pq);

#endif