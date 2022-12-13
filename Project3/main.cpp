/*
Header
Name: Brian Leavell

Assignment: Project 3

Purpose: Temperature and Humidity will be monitored and outputted to user, along with a dot matrix array displaying if 
the current conditions are ideal for the plant life being monitored
Modules: Main

Inputs: Matrix keypad, DHT11

Outputs: LCD, LEDs, Dot Matrix Array

Assignment Restrictions: 
      - At least 1 register must be controlled bitwise
      - Watchdog timer needs to be configured properly
      - At least 1 interrupt must be implemented
      - 1 task/thread intentionally incorporated
      - Critical section must be protected

References: Nucleo Manual/Pin layout - https://os.mbed.com/platforms/ST-Nucleo-L552ZE-Q
            MBed Handbook/Documentation Guide: https://os.mbed.com/handbook
*/ 
#include "mbed.h"
#include "rtos.h"  //for threading
#include "lcd1602.h"
#include "DHT.h"   //temp sensor
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream> 
#include "mbed_events.h"
//#include <MD_Parola.h>  //dot array library
//#include <MD_MAX72xx.h>  //dot array library
//#include <SPI.h>         //dot array library
  
//Setups for LCD scree,. matrix array, and dht11 sensor
using namespace std;

CSE321_LCD lcd(16,2,LCD_5x8DOTS,PB_9,PB_8);

//Setup for temperatrure sensor
DHT11 dht(PA_6);

//setup for the dot array
//MD_Parola dotarray(MD_MAX72XX::PAROLA_HW, PE_4, PE_5, PE_6, 11);


//If user pushes *, this will change to 1
int Tempcheck = 0;
//If user pushes #, this will change to 1
int Humcheck = 0;
//shows status LEDs
int sta = 0;

//Global values for temp and humidity readings
string temp;
string hum;


//Column 1 row 4 = * -> Temperature
//Column 3 row 4 = # -> Humidity

//**********************IMPORTANT INFORMATION BELOW*********************************
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//-------USER ADJUST THESE NUMBERS BEFORE RUNNING PROGRAM--------------------------//
double tempGood = 68.0;
double tempMod = 40.0;
double tempBad = 30.0;

double humGood = 40.0;
double humMod = 0.0;
double humBad = 0.0;
//--------------------------------------------------------------------------------------

//Watchdog timer initialization
Watchdog &doggo=Watchdog::get_instance();

//Mutex for synchronization
Mutex mutX;

//Thread used for LED and for button press
Thread thread;
//Event Queue for synchronization 
EventQueue queue(32*EVENTS_EVENT_SIZE);

#define timeout 16000 //system has 4 seconds before watchdog resets it

// Interrupt for button pushed in row 4
InterruptIn pressed4 (PD_6, PullDown);
InterruptIn pressed1 (PD_3,PullDown);

//Monitoring LEDs for temperature and humidity ranges
DigitalOut greenLED (PA_3);
DigitalOut orangeLED (PA_2);
DigitalOut redLED (PC_3);
DigitalOut runningLED(PB_0); //shows user that the system is reading data by blinking when turned on

//CUSTOM FUNCTIONS//
void Online(void); //Shows the user that the systsem is running

void Temp(void); //Takes input data and prints temperature to screen for user

void Hum(void);  //Takes input data and prints humidity to screen for user

//ISR for interrupt
void row4(void);
void row1(void);

//Cycles voltage through keypad
void Vcycle(void);

//displays status of room onto dot array and controls LED states
void status(void);

void operation(void);


int main()
{
    doggo.start(timeout); //start watchdog timer

    lcd.begin(); //initializes LCD for program

    Online(); //shows system is running for user

    dht.read();  //initial read of system

    //start of event queue 
    thread.start(callback(&queue, &EventQueue::dispatch_forever));

    //ISR to jump to once a button is pressed
    pressed4.rise(queue.event(&row4));

    //enables interrupt
    pressed4.enable_irq(); 

    //activating bus for port d for rows
    RCC -> AHB2ENR |= 0x8;

     //activating bus for port c for columns
    RCC -> AHB2ENR |= 0x4;

    //activating 4 pins for inputs PC8-11
    //PC8
    GPIOC -> MODER |= 0x10000; //sets 1
    GPIOC -> MODER &=~ (0x20000); //sets 0
    //PC9
    GPIOC -> MODER |= 0x40000;
    GPIOC -> MODER &=~ (0x80000);
    //PC10
    GPIOC -> MODER |= 0x100000;
    GPIOC -> MODER &=~ (0x200000);
    //PC11
    GPIOC -> MODER |= 0x400000;
    GPIOC -> MODER &=~ (0x800000);

    while (true){
        //constantly updates "Status" LEDs
        sta = !sta;
    //cycles voltage through keypad
        Vcycle();

        if (sta == 1){  //updates LEDs 
            status();
        }
        if (Tempcheck == 1){ //user reqested to see live temperature reading
            Temp();
        }
        else if (Humcheck == 1){   //user requested to see live humidity reading
            Hum();
        }
        doggo.kick();
    }
        return 0;
}

void operation(){
    while(true){
    runningLED = !runningLED;
    thread_sleep_for(500);
  }
} 

//shows system is running
void Online(){
    lcd.clear(); //clears screen incase of previous msg written on it
    lcd.setCursor(0,0);
    lcd.print("System Running");
  }

//voltage cycling for keypad
void Vcycle (){
    //set column 1 to high, the other 3 to low
    GPIOC->ODR |= 0x100;
    GPIOC->ODR &=~ (0xE00);
    thread_sleep_for(100);
    //column 2
    GPIOC->ODR |= 0x200;
    GPIOC->ODR &=~ (0xD00);
    thread_sleep_for(100);
    //column 3
    GPIOC->ODR |= 0x400;
    GPIOC->ODR &=~ (0xB00);
    thread_sleep_for(100);
    //column 4
    GPIOC->ODR |= 0x800;
    GPIOC->ODR &=~ (0x700);
    thread_sleep_for(100);
}

// //Button pressed in row 4
// void row1(void){
//     //ANDing values to see which button was pressed
//     int v = GPIOC->ODR & 0x800;

//     // A pushed
//     if (v != 0){
//        on = 1;
//        threadLED.start(operation);
//     }
// }
//Button pressed in row 4
void row4(void){
   
   
    //ANDing values to see which button was pressed
    int x = GPIOC->ODR & 0x100;
    int z = GPIOC->ODR & 0x400;

    // * pushed
    if (x != 0){
        Tempcheck = 1;
    }
    // # pushed
    else if (z != 0){
      Humcheck = 1;
    }
    //blinks LED 
    runningLED = 1;
    thread_sleep_for(1000);
    runningLED = 0;
}
//shows Temperature values on LCD
void Temp(void){
    
    mutX.lock();  //locks mutex incase another function tries to access the DHT values at the same time
    printf("Temperature values being accessed");

    dht.read(); //ensures recent readings are being used

    temp =  to_string(dht.getFahrenheit()); //live readings from sensor,conversion to string so it can be printed to LCD

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp(F):");
    lcd.setCursor(9,0);
    lcd.print(temp.c_str());
    thread_sleep_for(1000);

    //Reset var, LCD displays original "Running" message
    Tempcheck = 0;

    mutX.unlock(); //unlcoks MUTEX for other functions to access

    Online();
}

void Hum(void){

   mutX.lock();  //locks mutex incase another function tries to access the DHT values at the same time

   dht.read(); //ensures recent readings are being used

   printf("Temperature values being accessed");

    hum = to_string(dht.getHumidity()); //live readings from sensor

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humidity(%):");
    lcd.setCursor(13,0);
    lcd.print(hum.c_str());
    thread_sleep_for(1000);

    //Reset var, LCD displays original "Running" message
    Humcheck = 0;

    mutX.unlock();
    Online();
}

//temperature and humidity inputs
//lights up corresponding LEDs with temp/humidity range set by user
void status(){

//good conditions
if (dht.getFahrenheit() >= tempGood || dht.getFahrenheit() > tempMod && dht.getFahrenheit() > tempBad && dht.getHumidity() >= humGood || dht.getHumidity() > humMod){
    greenLED = 1;
    orangeLED = 0;
    redLED = 0;
    sta = !sta;
    doggo.kick();
    //dotarray.displayText("Ideal", PA_CENTER, dotarray.getSpeed(), dotarray.getPause(), PA_SCROLL_DOWN, PA_SCROLL_UP);
 }
 //moderate conditions
else if (dht.getFahrenheit() >= tempMod || dht.getFahrenheit() > tempBad && dht.getFahrenheit() < tempGood && dht.getHumidity() >= humMod || dht.getHumidity() < humGood && dht.getHumidity() > humBad){
    greenLED = 0;
    orangeLED = 1;
    redLED = 0;
    sta = !sta;
     doggo.kick();
    //dotarray.displayText("Moderate", PA_CENTER, dotarray.getSpeed(), dotarray.getPause(), PA_SCROLL_DOWN, PA_SCROLL_UP);
 }
 //bad conditions
else if (dht.getFahrenheit() <= tempBad && dht.getHumidity() <= humBad){
    greenLED = 0;
    orangeLED = 0;
    redLED = 1;
    sta = !sta;
    doggo.kick();
   // dotarray.displayText("Dangerous", PA_CENTER, dotarray.getSpeed(), dotarray.getPause(), PA_SCROLL_DOWN, PA_SCROLL_UP);
 }
 else{
     printf("No Readings"); 
 }
}
