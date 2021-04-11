#include "taskQueue.h"
#include <time.h>

void delay(int milliseconds);

static const uint16_t MAX_SIZE = 16;
static struct taskQueue main_que;
static struct taskQueue delayed_que;

static volatile uint16_t ticks = 0;
static volatile char timerFlag = 0;
// Systick simulator
void Systick_Handler(void)
{
    timerFlag = 1;
    ticks++;
}
// Enqueue task in the main queue
static void QueTask(funcPtr f, uint16_t priority)
{
    enqueue(&main_que, f, priority, 0);
}

// dispatch a task and run it if available, otherwise display 'IDLE'

static void dispatch()
{
    // handling the tasks in the delayed Queue first
    decrementDelay(&delayed_que, ticks);
    // enqueue the ready ones to the main Queue
    pushReady(&delayed_que, &main_que);
    // new dispatch --> ticks = 0 for the rest of the tasks
    ticks = 0;
    // dispatch tasks if available
    if (main_que.currentSize > 0)
        dequeue(&main_que).f();
    else
        printf("IDLE\n"); // for test purposes
}

static void ReRunMe(funcPtr f, uint16_t priority, uint16_t delay)
{
    //enqueue to the main queue if the task is not delayed, otherwise enqueue in the delayed Queue
    if (delay == 0)
        enqueue(&main_que, f, priority, delay);
    else
        enqueue(&delayed_que, f, priority, delay);
}

// initialize Hardware peripherals and Queues
static void init()
{
    // initialize Hardware
    // hardware_init();

    // initialize queues
    que_init(&main_que, MAX_SIZE);
    que_init(&delayed_que, MAX_SIZE);
}

//example tasks
static void f1()
{
    printf("ONE\n");
    ReRunMe(&f1, 1, 3);
}

static void f2()
{
    printf("TWO\n");
    ReRunMe(&f2, 2, 6);
}
static void f3()
{
    printf("THREE\n");
    ReRunMe(&f3, 3, 8);
}
static void f4()
{
    printf("FOUR\n");
    ReRunMe(&f4, 4, 12);
}

void delay(int milliseconds)
{
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause)
        now = clock();
}
// Main program
int main()
{
    init();
    QueTask(&f1, 1);
    QueTask(&f2, 2);
    QueTask(&f3, 3);
    QueTask(&f4, 4);

    while (1)
    {

        Systick_Handler();
        if (timerFlag)
        {
            // dispatch only if manual delay is not invoked
            dispatch();
            // feedback on manual delay
            timerFlag = 0;
        }
        delay(50);
    }

    return 0;
}
