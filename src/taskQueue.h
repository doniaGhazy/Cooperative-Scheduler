#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef void (*funcPtr)(void);

// some macros for min_heap algorithm
#define left(p) (2 * p + 1)  // left node of tree
#define right(p) (2 * p + 2) // right node of tree
#define p(c) ((c - 1) / 2)   // parent node of child
#define min(a, b) (a < b ? a : b)

// Task Object
struct task
{
    funcPtr f;
    uint16_t priority;
    uint16_t delay;
};

//Task queue

struct taskQueue
{
    uint16_t currentSize;
    uint16_t maxSize;
    struct task *tasks;
};

// default task

static const struct task defaultTask = {NULL, 65535, 65535}; // uint16_t MAX

// Functions (APIs) Prototypes
void que_init(struct taskQueue *que, uint16_t size);
void enqueue(struct taskQueue *que, funcPtr f, uint16_t priority, uint16_t delay);
void _enqueue(struct taskQueue *que, struct task Task);
void min_heap(struct taskQueue *que, int i);
void swap(struct task *t1, struct task *t2);
struct task dequeue(struct taskQueue *que);
void decrementDelay(struct taskQueue *que, uint16_t ticks);
void pushReady(struct taskQueue *delayed_que, struct taskQueue *main_que);
uint8_t compare(struct task t1, struct task t2);

// Helper function that serves as a less than operator for the tasks (t<delay,priority>)
uint8_t compare(struct task t1, struct task t2)
{
    //min delay first
    if (t1.delay < t2.delay)
        return 1;
    // min priority if delays are equal
    return (t1.delay == t2.delay) && (t1.priority < t2.priority);
}

// Helper function to swap tasks' references
void swap(struct task *t1, struct task *t2)
{
    struct task temp = *t1;
    *t1 = *t2;
    *t2 = temp;
}

// task queue constructor
void que_init(struct taskQueue *que, uint16_t size)
{
    que->currentSize = 0;
    que->maxSize = size;
    que->tasks = (struct task *)malloc(size * sizeof(struct task));
    for (uint16_t i = 0; i < que->maxSize; i++)
        que->tasks[i] = defaultTask;
}

// public enqueue function

void enqueue(struct taskQueue *que, funcPtr f, uint16_t priority, uint16_t delay)
{
    // create a task and call private enqueue
    volatile struct task Task = {f, priority, delay};
    _enqueue(que, Task);
}

// private enqueue function
void _enqueue(struct taskQueue *que, struct task Task)
{
    if (que->currentSize == que->maxSize - 1)
    {
        printf("ERROR! Queue is full");
        return;
    }
    // add task to the end of the queue
    que->tasks[que->currentSize++] = Task;

    // climb the tree until it cannot be swapped with its parent
    int i = que->currentSize - 1;
    while (i != 0 && compare(que->tasks[i], que->tasks[p(i)]))
    {
        swap(&que->tasks[i], &que->tasks[p(i)]);
        i = p(i);
    }
}

// Min heap algorithm for dequeuing (heapify)
void min_heap(struct taskQueue *que, int i)
{
    int min = i;
    int left = left(i);
    int right = right(i);
    // get the minimum (left , right , current) NODE
    if (left <= que->currentSize && compare(que->tasks[left], que->tasks[min]))
        min = left;
    if (right <= que->currentSize && compare(que->tasks[right], que->tasks[min]))
        min = right;
    // if this node is minimum, then left , right , subtree are heapified
    if (min == i)
        return;
    // else put the minimum on top , then heapify the resulting tree
    swap(&que->tasks[min], &que->tasks[i]);
    min_heap(que, min);
}

// dequeue function

struct task dequeue(struct taskQueue *que)
{
    struct task ret = defaultTask;
    if (que->currentSize == 0)
    {
        printf("ERROR! dequeuing from Empty Queue");
        return ret; // returns the default task
    }
    // Take task reference and replace its reference with the last task in the queue
    ret = que->tasks[0];
    que->tasks[0] = que->tasks[que->currentSize - 1];
    que->currentSize--;

    // heapify only when dequeuing (put the last task in its place in the tree)
    min_heap(que, 0);
}

// decrements all the delays in a queue (used with delayed Queue)
void decrementDelay(struct taskQueue *que, uint16_t ticks)
{
    for (uint8_t i = 0; i < que->currentSize; i++)
        que->tasks[i].delay -= min(ticks, que->tasks[i].delay);
}

// pushes all ready tasks from the delayed queue to the ready queue

void pushReady(struct taskQueue *delayed_que, struct taskQueue *main_que)
{
    while (delayed_que->currentSize > 0 && delayed_que->tasks[0].delay == 0)
        _enqueue(main_que, dequeue(delayed_que));
}