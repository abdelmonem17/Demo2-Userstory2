# Demo2-Userstory2
A demo project using freeRTOS

The project has four tasks with the following specifications.
1.	Task A: Responsible for monitoring push button 1 state.
2.	Task B: Responsible for monitoring push button 2 state.
3.	Task C: Responsible for switching an LED ON/OFF.
4.	Task D: Responsible for monitoring state of LED and printing its state on LCD and which button is currently activating the LED. 
When push button 1 is pressed the LED is switched ON as long as it’s pressed and LCD prints current LED state and current push button 1 state. If push button 2 is pressed the LED is switched ON as long as it’s pressed and LCD prints current LED state and current push button 2 state. Only one push button is allowed to be pressed at a time to switch LED ON.
