# Cooperative-Scheduler
The scheduler depends mainly on the taskQueue.h file and the tempelate provided in main.c under [src] (https://github.com/doniaGhazy/Cooperative-Scheduler/tree/main/unit_tests%20(.h%20%2B%20.c%20)/Core/Src)
## Main functions

```QueTask```

This function enqueues a task using its function pointer into the main queue. it takes the function pointer and the priority as arguments. it calls the ```enqueue``` function discussed in the taskQueue.h section

```dispatch```

This function is called in the infinite loop each time the SysTick sets the timerFlag to 1 after counting 50ms (the handling of the ticks and time is illustrated in the stm32l4xx_it.c file). it first decrements the delays of all the tasks in the delayed queue and makes use of the ticks value representing the skipped cycles as well if ticks is a non-zero value. it then enqueues the ready tasks (delay==0) from the delayed queue to the main queue. this happens by calling the ```pushReady``` function which takes pointers to both queues and handles the enqueuing. then, the ticks value is set to 0 in order not to over-count the skipped cycles of other tasks. if the main queue is empty, "IDLE" is transmitted through UART2 making use of the HAL_UART_Transmit() function. however, if the main queue is not empty, the first task in the main queue is dequeued and executed through its function pointer. 

```ReRunMe```

This function is responsible for making a task able to periodically run itself every specified delay time units. Moreover, This function is used so that a Task can enqueue itself in either the main_queue or the delayed_qeueue depending on the value of the delay. it takes the signature ```ReRunMe (function pointer , priority , delay)``` .

```init```

This function is mainly responsible for initializing the Queues (delayed Queue and Main Queue) which represents the main data structures of the scheduler. the hardware is initialized using the HAL_functions() after generating the project using CubeMx since the hardware initialization depends heavily on the application

## taskQueue.h

### this file implements the function-Queue-scheduling discussed in the lectures based on the ideas of priority queues (based on binary minimium heaps). 

1. Helpful Macros:
* funcPtr: a pointer to a function that takes void and returns void.
* left(p) (2 * p + 1)  : left direct child node of current parent node (p).
* right(p) (2 * p + 2) : right direct child node of current parent node (p).
* p(c) ((c - 1) / 2) : parent node of child node (c).
* min(a, b) (a < b ? a : b) : minimum of a and b. 

2. Task Struct:

* f: function pointer of type funcPtr corresponding to the task function.
* priority: corresponds to the priority of the task.
* delay: the delay of the task put in the delay queue.

3. taskQueue struct has:
* currentSize : number of elements in the queue.
* maxSize : maximum size.
* tasks: a task struct pointer pointing to the array of tasks the taskQueue has.

4. functions that handle TaskQueue [different from the APIs mentioned above]

```void que_init(struct taskQueue *que, uint16_t size)```

a function that creates a taskQueue of size = size 

```void enqueue(struct taskQueue *que, funcPtr f, uint16_t priority, uint16_t delay)```

a public enqueue function that creates a task making use of ```(f , priority , delay)``` and calls the private enqueue function. 

```void _enqueue(struct taskQueue *que, struct task Task)```

private enqueue function that enqueues a Task in the TaskQueue 

```void min_heap(struct taskQueue *que, int i)```
a heapify function that restructures the queue according to the priorities of the tasks. it is basically called each time the dequeue function is called by putting the last function in the queue in the beginning of the tree and moving down until it reaches its place. 

```void swap(struct task *t1, struct task *t2)```

a Helper function that swaps the references of two tasks t1 and t2. 

```struct task dequeue(struct taskQueue *que)```

a function that gets the head of the queue (the highst priority ready task) to be executed.

```void decrementDelay(struct taskQueue *que, uint16_t ticks)```

a function that decrements the delay of the tasks in the delayed queue according to the number of ticks [skipped cycles] (if it is a non-zero value) and the value of its delay. 

```void pushReady(struct taskQueue *delayed_que, struct taskQueue *main_que)```

a function that enqueues the ready tasks from the delayed queue to the main queue.

```uint8_t compare(struct task t1, struct task t2)```

a Helper function that serves as a less than operator for the tasks **(t<delay,priority>)**.


## Unit tests:
**Test for the ready queue**
                                                        
This part has four tasks. Here, no need to call “Rerun” because it is only the ready queue test. The following table describes their definition: \
- If all tasks have different priorities, so the one with the highest priority level (least priority number) executes first. 
![image](https://user-images.githubusercontent.com/45397911/114049532-cd77f900-988b-11eb-8d1f-5a5ecc354ad2.png)

###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114049574-d8328e00-988b-11eb-9554-35028c147234.png)
###### The Scheduler output:
 ![image](https://user-images.githubusercontent.com/43725885/114219942-a3960380-996b-11eb-84b1-350bde171506.png)
- If two tasks have the same priority level, so anyone of them executes first.
![image](https://user-images.githubusercontent.com/45397911/114049840-1334c180-988c-11eb-87ca-f2caf5d4b99e.png)
###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114049903-1fb91a00-988c-11eb-88bc-ebbba8f653e3.png)
###### The Scheduler output:
![image](https://user-images.githubusercontent.com/43725885/114223819-8c0d4980-9970-11eb-887e-a2b6201036fc.png)
- If two tasks have the same priority level, so anyone of them executes first.
![image](https://user-images.githubusercontent.com/45397911/114050057-3e1f1580-988c-11eb-88ae-6d7985a9a208.png)
###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114050162-51ca7c00-988c-11eb-8c40-78afbb1ed6a3.png)
###### The Scheduler output:
![image](https://user-images.githubusercontent.com/43725885/114225815-15be1680-9973-11eb-98f7-b0d0a3b1c46b.png)
-	If two tasks have the same priority level, so anyone of them executes first.
![image](https://user-images.githubusercontent.com/45397911/114050319-732b6800-988c-11eb-99c7-e201065bf491.png)
###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114050353-7c1c3980-988c-11eb-9241-bf4a2bd36e38.png)
###### The Scheduler output:
![image](https://user-images.githubusercontent.com/43725885/114226077-69306480-9973-11eb-9778-ff9653241c83.png)
- If all tasks have the same priority level, so anyone of them executes first.
![image](https://user-images.githubusercontent.com/45397911/114050470-95bd8100-988c-11eb-8c1d-cc95ffa7a94a.png)
###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114050507-9eae5280-988c-11eb-8114-1b45ddbf4d27.png)
###### The Scheduler output:
![image](https://user-images.githubusercontent.com/43725885/114226866-7e59c300-9974-11eb-8141-d4b8c619dbd6.png)

**Test for the delayed queue**
- If all of them have different priorities:
![image](https://user-images.githubusercontent.com/45397911/114051010-08c6f780-988d-11eb-90c8-3d0669a3762a.png)
###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114051058-141a2300-988d-11eb-8862-5e49c5047764.png)
###### The Scheduler output:
![image](https://user-images.githubusercontent.com/43725885/114228665-15277f00-9977-11eb-9fc1-14c50a4281f6.png)

