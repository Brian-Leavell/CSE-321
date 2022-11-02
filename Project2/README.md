# CSE-321 Project 2 Overview
For this project, we were tasked with designing a system that uses an external matrix keypad to "unlock" or "lock" a system as displayed on an LCD. This project involved implementing ISRs, some APIs, as well as utilizing Interrupts and bitwise controlled registers. 

## About 
This is program involves using a 4-digit code to unlock or lock a system. The actual locking/unlocking of the system is purely just for namesake, but an LCD screen will display either word based on what state it is in.
A user will enter 4 digits on an external keypad (last 4 digits of person number), with an LED lighting up every time a button is pressed. If the correct combination is entered, then the display will update from locked to
unlocked or vice versa. If the wrong combination is entered, the screen will display some form of an error message.

## Project Description 
Use an LCD API and interrupt along with bitwise controlled registers so that a user can enter 4 digits using the keypad. If its a matching input code, the LCD will update to say "locked"
or "unlocked. If it is incorrect, an error message is printed to the LCD.

## Features
When a button is pressed on the keypad, an LED will light up.
If the correct 4 digit code is entered, the system will lock or unlock
If the wrong 4 digit code is entered, the screen will display an error message

## Required Materials
* Nucleo L4R5ZI
* MBed Studio
* Form of text editor/code editor 
* LED
* USB Data Cable
* Laptop/PC
* Breadboard
* Wiring Kit
* Resistors
* Keypad

## Getting Started
Open up MBed studio on a computer and connect your nucleo to it using the data cable. Find the correct ports on the nucleo that are being used in this program, and connect the wires from the nucleo into the keypad. Another wire
should be connected to the port linked with the external LED that flashes when a button is pressed, as well as the LCD being set up using the SDA and SLA pins listed in the program. Now the program
can be ran. Everytime a keypad button is pressed the extneral led should light up. The LCD screen should display "locked" or "unlocked". Type in the correct passcode to change this display and 
it should work as expected. Now enter a wrong passcode and the display screen should display an error message of some kind.

## Things Declared
**Variables** - length, passcode, entered, current_state

**Functions** - row1, row2, row3, row4, check, cycle, status

**Libraries** - mbed.h, lcd1602.h, cstdio, cstring, string, iostream

**API** - InterruptIn (pressed1, pressed2, pressed3, pressed4)

## API and Built In Elements Used
InterruptIn

CSE321_LCD

.enable_irq

.rise

wait_us

## Custom Functions
 
### ***row (1-4):***

These functions are called when an interrupt is detected on a rising edge for each input pin. They all do relatively the same thing: When a button is pressed in its respective row,
the external LED will turn on and off (resembling a blink). Next, each column has its bits ANDed and assigned to a variable (x,y,z, or c). Conditionals are now used to check which variable is equal to a non-zero
value. If it is a non-zero value, that means that specific column has a hihg voltage a.k.a we now know what column the button pressed was in. Since that specific row's ISR was called, we can now determine exactly what button was pressed.
Next, that specifc button is added to an array that stores the entered code and increments our counter by 1 (after this coutner reaches 4 it checkes if the code entered is correct).

### ***check:***

This function is called after our counter (length) reaches the value of 4. This means that 4 digits have been entered, so now we compare the entered code (stored in the char array "entered") to the correct code 
(stored in the char array "passcode", value being 3239). If it is correct, we invert the variable "current_state", reset our counter, clear the LCD screen, and call the function status to display our updated status. If the wrong code was entered, and error 
message is displayed, our coutner is reset, and the user must start over.

### ***cycle:***

This function is constatntly being called inside the while loop. What is does essentially is constantly cycling the voltages through the columns of the keypad. This is done by setting the voltage to high in one column at a time,
while setting the other columns to low. This is done by OR and AND logic, as well as using wait_us in between cycles to address bounce and ensure there is no error when our ISRs are called.

### ***status:***

This function reads our variable "current_state" (either a 1 or 0 value), and prints to the screen the status of our system. If it is a 1, that means our system is locked so the LCD will have such informtation printed to the screen.
Same goes if the variable is a 0.

## Files Used In This Program
[main.cpp File](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project2/main.cpp)

[lcd1602.h File](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project2/lcd1602.h)

[lcd1602.cpp File](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project2/lcd1602.cpp)


## References
[Project 2 Handout](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project2/Project%202%20Overview.pdf)

[Nucleo Refernce PDF](https://www.st.com/resource/en/user_manual/dm00368330-stm32-nucleo-144-boards-mb1312-stmicroelectronics.pdf)

[MBed Handbook/Documentation Guide](https://os.mbed.com/handbook)

[LCD Setup & Functions](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project2/LCD%20API%20Instructions.pdf)

[1602 LCD Display Manual](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project2/1602%20LCD%20Datasheet.pdf)
