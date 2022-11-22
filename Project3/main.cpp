/*
Header
Name: Brian Leavell
Assignment: Project 3
Purpose: Monitor temperature and humidity of environment
Date of Creation: 11/18/2022 
Modules:
Inputs: Matrix keypad, DHT11
Outputs: LCD, LEDs, Dot matrix array
Assignment Restrictions: 
     
References: Nucleo Manual/Pin layout - https://os.mbed.com/platforms/ST-Nucleo-L552ZE-Q
            MBed Handbook/Documentation Guide: https://os.mbed.com/handbook
*/ 

#include "mbed.h"
#include "lcd1602.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream> 

//configure LCD, PB_9 and PB_8 declared for SDA and SLA respectively
CSE321_LCD lcd(16,2,LCD_5x8DOTS,PB_9,PB_8);

InterruptIn pressed4 (PD_6, PullDown);
void row4(void);

int main(){


    lcd.begin(); //initializes LCD for program
    lcd.clear(); //clears screen incase of previous msg written on it
    lcd.setCursor(0,0);
    lcd.print("System Status:");
  
    //activating bus for port A for output LED used by ISR
    RCC->AHB2ENR |= 0x1; 
    GPIOA -> MODER |= 0x4; //Setting the 1 bit
    GPIOA -> MODER &=~ (0x8); //Setting the 0 bit
  
     while (true){
       cycle();
   } 
    return 0;
}

//cycles the voltage through each column one at a time
void cycle (){
    //set column 1 to high, the other 3 to low
    GPIOC->ODR |= 0x100;
    GPIOC->ODR &=~ (0xE00);
    wait_us(100);
    //column 2
    GPIOC->ODR |= 0x200;
    GPIOC->ODR &=~ (0xD00);
    wait_us(100);
    //column 3
    GPIOC->ODR |= 0x400;
    GPIOC->ODR &=~ (0xB00);
    wait_us(100);
    //column 4
    GPIOC->ODR |= 0x800;
    GPIOC->ODR &=~ (0x700);
    wait_us(100);
}

//Button pressed in row 4
void row4(void){
    GPIOA -> ODR |= 0x2;
    wait_us(800000);     
    GPIOA -> ODR &=~ (0x2); 
    wait_us(100000); 

    int x = GPIOC->ODR & 0x100;
    int y = GPIOC->ODR & 0x200;
    int z = GPIOC->ODR & 0x400;
    int v = GPIOC->ODR & 0x800;
