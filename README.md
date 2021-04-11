# Cooperative-Scheduler
## This is the second application used which is Parking sensor using HC-SR04 Ultrasonic Sensor 
The application relys on the scheduler implemented in the main branch. It basically consists of different tasks given to the scheduler with different priorities. Based on each priority, each task will get excuted.

## Main Tasks:
1. **Trig_task()**: it basically a task queued by other tasks main() using QueTask() and itself using ReRunMe() as well. It starts the TRIG signal of the sensor so that it can send out 8 pulses of sound waves at an ultrasonic frequency of 40KHz. This can be done bby calling HAL_GPIO_WritePin() to set the configured pin and then call HAL_TIM_IC_Start_IT() to start the timer.
#### Priority given:
This task is given a priority of two, highest priority among the tasks, as logically speaking it is the first needed funcionality to be done before even 
receiving the digital pulse on the echo pin of the sensor.
Here, we need to call the function "Rerun" every 10us given the same priority.


2. **evaluate_echo()** : it basically a task queued by other taks main() using QueTask(). It measures the room tempreture every 30 seconds using RTC (DS3231) by using HAL_I2C_Master_Receive() and transfers it to the teraterm (PC) using HAL_I2C_Master_Transmit(). While getting a new value every 30 seconds, we compare it with the value of the input threshold (received by ReadThreshold()). In case it is larger, we blinked LED to indicate that it is above the threshold value. Otherwise, we turn the LED off.
Temperature is represented as a 10-bit code with a resolution of 0.25°C and is accessible at location 11h and 12h. The upper 8 bits, the integer portion, are at location 11h and the lower 2 bits, the fractional portion, are in the upper nibble at location 12h. We managed to modify the register values and send them through HAL_I2C_Master_Transmit() in order to successfully read the room temperature.
#### Priority given:
This task is given a priority of TWO as logically speaking it is the second needed funcionality to be done after getting the threshold temperature from the user.
Here, we need to call the function "Rerun" every 30 seconds given the same priority.


3. **buzzer()**: it basically a small task queued by other tasks GettingTemperature() using QueTask(). It just blinks LED whenever the measured temperature is larger than the input threshold temperature. 
#### Priority given:
This task is given a priority of ONE as logically speaking it is a needed funcionality to be done after the condition becomes true. Technically, it is implicitly got repeated every 30 seconds in case the condition is true. 
