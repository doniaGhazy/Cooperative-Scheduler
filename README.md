# Cooperative-Scheduler
## This is the second application used which is Parking sensor using HC-SR04 Ultrasonic Sensor 
The application relys on the scheduler implemented in the main branch. It basically consists of different tasks given to the scheduler with different priorities. Based on each priority, each task will get excuted.

## Main Tasks:
1. **Trig_task()**: it basically a task queued by other tasks main() using QueTask() and itself using ReRunMe() as well. It starts the TRIG signal of the sensor so that it can send out 8 pulses of sound waves at an ultrasonic frequency of 40KHz. This can be done bby calling HAL_GPIO_WritePin() to set the configured pin and then call HAL_TIM_IC_Start_IT() to start the timer.
#### Priority given:
This task is given a priority of two, highest priority among the tasks, as logically speaking it is the first needed funcionality to be done before even 
receiving the digital pulse on the echo pin of the sensor.
Here, we need to call the function "Rerun" every 10us given the same priority.


2. **evaluate_echo()** : it basically a task queued by other taks main() using QueTask() and itself using ReRunMe() as well. The rational behind it is the fact that The sound travels straight-line path until it hits an object then it reflects back to the sensor module which sends out a digital pulse on the echo pin that has a width equal to the travel time of sound going back and forth between the module and the sensed object. This is done in programming by getting two time values: the first one once the TRIG pin is set and the other when the echo is received by the sensor. This time difference got multiplied by 0.0343 (speed of light) and then divided by two to accomedate for the forward and backward travel time of the sound. Then, this calculated distance (cm) is displayed on teraterm via UART communication.  
#### Priority given:
This task is given a priority of two, highest priority among the tasks, as logically speaking it is a needed funcionality to be done after the TRIG pin of the sensor is set.
Here, we need to call the function "Rerun" as the distance needs to be calculated regulary.

3. **buzzer()**: it basically a small task queued by other tasks GettingTemperature() using QueTask(). It just blinks LED whenever the measured temperature is larger than the input threshold temperature. 
#### Priority given:
This task is given a priority of ONE as logically speaking it is a needed funcionality to be done after the condition becomes true. Technically, it is implicitly got repeated every 30 seconds in case the condition is true. 



