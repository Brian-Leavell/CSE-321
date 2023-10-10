# CSE-321 Project 3 Overview

## About

This project involves the use of threads and synchronization, as well as the use of interrupts and bitwise control. The purpose of this specific project is to monitor environments geared towards growing 
plantlife, similar to green houses or large scale gardens, in order to ensure the plants are adequetly cared for. A temperature/humdity sensor will be used to read in the current temperature and humidity of its surroundings.
Using a matrix keypad, a user can push two specific buttons to have the current temperature or humidity printed to an LCD screen. Depending on the acceptable range for temperature and humidity (set by the user before running the program),
a dot matrix array will display the state of the growing conditions for the plants. The array will either display "Ideal", "Moderate", or "Bad", depending on the curent temperature and humidtiy. This will allow the user to be 
aware that the plants are not in an ideal growing environment.
## Project Description

Utilize Interrupts, threads, APIs, and synchronization to display specific data (Temperature/Humidity) to an LCD as well as the state of the environment to a dot matrix array.
A user can use a keypad to display the current temperature or humidity when needed. The dot matrix array will keep the user aware of the current environment and if it needs to be adjusted.
## Features

A sensor will be used to read in live data of the current environment
When a button is pressed on the keypad, an LED will light up.
A button will display either the temperature or the humidity to the LCD
A dot matrix array will allow the user to see if the environment has ideal conditions for the plantlife
## Required Materials

- Resistors
- Keypad
- Dot Matrix Array
- 1602 LCD Display Screen
- DHT11 Sensor
- Nucleo L4R5ZI
- MBed Studio
- 4 LEDs (Red, Yellow, Green Required)
- Dot matrix array
- Breadboard
- Wiring Kit
## Getting Started

Open up MBed studio on a computer and connect your nucleo to it using the data cable. Find the correct ports on the nucleo that are being used in this program, and connect the wires from the nucleo into the keypad.
The dot matrix array and LCD should be connected already, if not, find the correct ports and pins to plug into to set them up. Under the specified section, input the range of temperatures and humidities that are ideal for
the specific plantlife that is being monitored, as well as the levels that are "dangerous" or poor for the plants. Once the values have been set, run the program. Pushing the star key (*) will display the current temperature
to the LCD, and pushing the pound key (#) will display the current humidity. The dot matrix array should display one of three messages based on the current data depending on what numbers were set before running the program.
## Things Declared

**Variables** - Tempcheck, Humcheck, sta, temp, hum. tempGood, tempMod, tempBad, humGood, humMod, humBad, timeout

**Functions** - Online, Temp, Hum, row4, Vcycle, stastus, operation

**Libraries** - mbed.h, lcd1602.h, cstdio, cstring, string, iostream, rtos.h, DHT.h, mbed_events.h, MD_Parola.h, MD_MAXX72xx.h, SPI.h

**API** - InterruptIn, DigitalOut, WatchDog, Event Queue, Threads

## API and Built In Elements Used

InterruptIn

DigitalOut

enable_irq

.rise

thread_sleep_for

WatchDog Timer

Event Queue

Mutex

CSE321_LCD

DHT11

MD_Parola dotarray

## Custom Functions

### ***Online:***
This function is responsible for displaying a message on the LCD screen to let the user know the system is active and running.

### ***Temp:***
This function is called once a user pushed the "Temperature" button on the keypad. It will print to the LCD screen the most recent temperature (F) reading from the DHT11 sensor

### ***Hum:***
This function is called once a user pushed the "Humidity" button on the keypad. It will print to the LCD screen the most recent humidity (%) reading from the DHT11 sensor

### ***row4:***
This function is called when an interrupt is detected on a rising edge for each input pin. Each column has its bits ANDed and assigned to a variable (x,y,z, or c). Conditionals are now used to check which variable is equal to a non-zero
value. If it is a non-zero value, that means that specific column has a high voltage a.k.a we now know what column the button pressed was in. Since that specific row's ISR was called, we can now determine exactly what button was pressed.
Next, a global variable is updated to one (either the temperature or the humidity). Inside the "while" loop of main, the respective temperature or humdity funciton is then called to display readings from the DHT11 onto the LCD.

### ***Vcycle:***
This function is constatntly being called inside the while loop of the main function. What is does essentially is constantly cycling the voltages through the columns of the keypad by setting the voltage to high in one column at a time,
while setting the other columns to low. This is done by OR and AND logic, as well as using wait_us in between cycles to address bounce and ensure there is no error when our ISRs are called.

### ***status:***
This function controls the three external LEDs. After a user sets the preferred temperature and humidity levels (ideal, moderate, dangerous), whenever the DHT11 takes in a live reading, a specific LED will then turn on. This is to 
alert the user of the system of the current environmental conditions. If the temperature and humidity remain within range of what the user set as "ideal", the green LED will power on. This logic applied for the yellow and red LEDs

### ***operation:***
This simple function will visually let the user see the system running by making an LED blink whenever the system is powered on.


## Files Used In This Program

[main.cpp File](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project3/main.cpp)

[lcd1602.h File](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project3/lcd1602.h)

[lcd1602.cpp File](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project3/lcd1602.cpp)

[DHT11.h](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project3/DHT.h)

[DHT11.cpp](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project3/DHT.cpp)

## References
[Project Slideshow Presentation](https://github.com/Brian-Leavell/CSE-321/blob/main/Project3/CSE321_project3_%20bsleavel_%20report.pdf)

[Project 3 Handout](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project3/CSE321%20Project%203%20Fall%202022.pdf)

[Nucleo Refernce PDF](https://www.st.com/resource/en/user_manual/dm00368330-stm32-nucleo-144-boards-mb1312-stmicroelectronics.pdf)

[MBed Handbook/Documentation Guide](https://os.mbed.com/handbook)

[LCD Setup & Functions](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project2/LCD%20API%20Instructions.pdf)

[1602 LCD Display Manual](https://github.com/Brian-Leavell/CSE321-Course-Progress/blob/main/Project2/1602%20LCD%20Datasheet.pdf)

[Github containing files for Dot Matrix Array Setup](https://www.makerguides.com/max7219-led-dot-matrix-display-arduino-tutorial/)
