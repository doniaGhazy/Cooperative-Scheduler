# Cooperative-Scheduler Applications
## This is the first application used which is Temperature Monitor
The application relys on the scheduler implemented in the main branch. It basically consists of different tasks given to the scheduler with different priorities. Based on each priority, each task will get excuted.

## Main Tasks:
1. **ReadThreshold()**: it basically a task queued by other taks main() using QueTask() and itself using ReRunMe() as well. It receives the threshold value from the user through teraterm (PC) using UART communication. After, receving this value, it will be recalculated in order to get its integer value to be compared later with the temperature measured by the RTC.
#### Priority given:
This task is given a priority of ONE (highest priority) as logically speaking it is the first needed funcionality to be done before even getting the temperature from the RTC.
Here, No need to call the function "Rerun" since we only need to have this value ONLY once at the beginning of the execution.
#### assumptions: 
- This application assumes that the maximum size of the input threshold is ONLY two digits. 
- The threshold value is ONLY entered once at the beginning of the execution.
2. **GettingTemperature()** : it basically a task queued by other taks main() using QueTask(). It measures the room tempreture every 30 seconds using RTC (DS3231) by using HAL_I2C_Master_Receive() and transfers it to the teraterm (PC) using HAL_I2C_Master_Transmit(). While getting a new value every 30 seconds, we compare it with the value of the input threshold (received by ReadThreshold()). In case it is larger, we blinked LED to indicate that it is above the threshold value. Otherwise, we turn the LED off.
Temperature is represented as a 10-bit code with a resolution of 0.25°C and is accessible at location 11h and 12h. The upper 8 bits, the integer portion, are at location 11h and the lower 2 bits, the fractional portion, are in the upper nibble at location 12h. We managed to modify the register values and send them through HAL_I2C_Master_Transmit() in order to successfully read the room temperature.
#### Priority given:
This task is given a priority of TWO as logically speaking it is the second needed funcionality to be done after getting the threshold temperature from the user.
Here, we need to call the function "Rerun" every 30 seconds given the same priority.

3. **BlinkingLED()**: it basically a small task queued by other tasks GettingTemperature() using QueTask(). It just blinks LED whenever the measured temperature is larger than the input threshold temperature. 
#### Priority given:
This task is given a priority of ONE as logically speaking it is a needed funcionality to be done after the condition becomes true. Technically, it is implicitly got repeated every 30 seconds in case the condition is true. 
## Test for this application
![image](https://user-images.githubusercontent.com/45397911/114270671-05a74500-9a0e-11eb-82cd-b6dd850c0fb6.png)
- Here the input threshold is 11 so the condition is true so LED will blink.
#### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114270702-22dc1380-9a0e-11eb-9487-f5cbdf574f8c.png)
#### The scheduler output:
![image](https://user-images.githubusercontent.com/45397911/114270736-54ed7580-9a0e-11eb-832f-ba9c766cfe83.png)
- Here the input threshold is 23 so the condition is false so LED will not blink.
#### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114270807-a4cc3c80-9a0e-11eb-9177-637d5573a6b8.png)
#### The scheduler output:
![image](https://user-images.githubusercontent.com/45397911/114270823-c0374780-9a0e-11eb-8fb8-2ef45b379102.png)
