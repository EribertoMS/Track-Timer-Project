# Track-Timer-Project
The purpose of this project is to design a track timer. This track timer has the ability to measure the amount of time it takes an athlete to cover a certain distance. For example I use this device for tracking my run times for the 100m race. This device will show me the amount of time it takes to cover that 100m distance. The project uses two modules. Module one is the 'Master Blink' module. This module acts like the start gun in a track race. It tells the user when to begin running by using a series of LED's to countdown and a buzzer to notify when the countdown has ended. Once the countdown ends the Master Blink module will send a signal to module two, or the 'Timer' module, to begin the countdown. The Timer module will continue to count until the PIR sensor onboard is triggered. The PIR sensor stops the timer at the exact moment the user crosses the imaginary finish line. The Timer module then displays the time using a 4-digit seven segment display for the user to see. The user may then reset the timer using the reset button on the Timer module and they are free to run another lap.

Module 1 / Master Blink Module finished PCB with soldered components
![1000012463](https://github.com/user-attachments/assets/fe07811b-e2a3-48d2-a501-8192960ce8b8)




Module 1 / Master Blink Module bare PCB
![1000012461](https://github.com/user-attachments/assets/448768bf-1044-4dff-a1f8-72bd6083e804)

Scematic using KiCad
<img width="1088" height="524" alt="Screen Shot 2025-09-24 at 7 40 50 PM" src="https://github.com/user-attachments/assets/b836b20e-9450-49dc-ba2f-8e76c3a46a12" />

PCB Schematic Layout
<img width="705" height="482" alt="Screen Shot 2025-09-24 at 7 41 56 PM" src="https://github.com/user-attachments/assets/08a3dff6-b963-406d-aa49-2843e499bcb0" />
