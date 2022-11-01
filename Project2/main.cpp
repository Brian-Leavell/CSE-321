/*
Header
Name: Brian Leavell
Assignment: Project 2
Purpose: After ther correct 4 digits are entered, the system will either unlock or lock
Date of Creation: 10/10/2022 
Modules: Main, 
Inputs: Matrix keypad
Outputs: LCD, LEDs
Assignment Restrictions: 
      -Register must be controlled bitwise
      -bounce must be addressed
      -One ISR and interrupt must be implemented

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

//Passcode length (4 digits)
int length = 0;
//Correct Code: 3239
char passcode[] = {'3','2','3','9'};

//code entered by user, compare it to passcode
char entered[4] = {};

//state used to check is system is locked(1)/unlcocked(0)
int current_state = 1;

//Interrupts for buttons pressed (on inputs/rows)
InterruptIn pressed1 (PD_3, PullDown);
InterruptIn pressed2 (PD_4, PullDown);
InterruptIn pressed3 (PD_5, PullDown);
InterruptIn pressed4 (PD_6, PullDown);

//CUSTOM FUNCTIONS//

//ISRs for Interrupts to jump to
void row1(void);
void row2(void);
void row3(void);
void row4(void);

//Compare passcode and entered code
void check(void);

//cycle voltage through columns
void cycle (void);

//Display current status (locked or unlocked)
void status(void);

int main(){


    lcd.begin(); //initializes LCD for program
    lcd.clear(); //clears screen incase of previous msg written on it
    lcd.setCursor(0,0);
    lcd.print("System Status:");

    //activating bus for port A for output LED used by ISR
    RCC->AHB2ENR |= 0x1; 
    GPIOA -> MODER |= 0x4; //Setting the 1 bit
    GPIOA -> MODER &=~ (0x8); //Setting the 0 bit

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
    
    //jumps to ISR once interrupt detected on rising edge
    pressed1.rise(&row1); 
    pressed2.rise(&row2);
    pressed3.rise(&row3);
    pressed4.rise(&row4);

    //enables interrupts
    pressed1.enable_irq(); 
    pressed2.enable_irq(); 
    pressed3.enable_irq(); 
    pressed4.enable_irq(); 

    //custom function, displaying current state on LCD
    status();

   while (true){

    cycle();
    //once 4 digits have been entered, check if they match the correct value
    if (length == 4){
        check();
    }
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

//Button pressed in row 1
void row1(void){
    //flash the LED
    GPIOA -> ODR |= 0x2; 
    wait_us(800000);
    GPIOA -> ODR &=~ (0x2);
    wait_us(100000); 

    //ANDing the columns to see which value will be non-zero aka HIGH
    int x = GPIOC->ODR & 0x100;
    int y = GPIOC->ODR & 0x200;
    int z = GPIOC->ODR & 0x400;
    int v = GPIOC->ODR & 0x800;

    //whatever column is a non-zero is the column that the button was pressed in
    if (x != 0){
       entered[length] = '1'; 
       length += 1;   
    }
    else if (y != 0){
        entered[length] = '2'; 
        length += 1;   
    }
    else if (z != 0){
      entered[length] = '3';  
      length += 1;
    }
    else if (v != 0){
       entered[length] = 'A';
       length += 1;
        
    } 
}

//Button pressed in row 2
void row2(void){
    GPIOA -> ODR |= 0x2;
    wait_us(800000);     
    GPIOA -> ODR &=~ (0x2);
    wait_us(100000); 

    int x = GPIOC->ODR & 0x100;
    int y = GPIOC->ODR & 0x200;
    int z = GPIOC->ODR & 0x400;
    int v = GPIOC->ODR & 0x800;

    if (x != 0){
      entered[length] = '4';
      length += 1;
       
    }
    else if (y != 0){
       entered[length] = '5';
       length += 1;
       
    }
    else if (z != 0){
       entered[length] = '6';
       length += 1;
       
    }
    else if (v != 0){
        entered[length] = 'B';
        length += 1;
    } 
    
}
//Button pressed in row 3
void row3(void){
   GPIOA -> ODR |= 0x2;
   wait_us(800000); 
   GPIOA -> ODR &=~ (0x2); 
   wait_us(100000); 

    int x = GPIOC->ODR & 0x100;
    int y = GPIOC->ODR & 0x200;
    int z = GPIOC->ODR & 0x400;
    int v = GPIOC->ODR & 0x800;

    if (x != 0){
     entered[length] = '7';
     length += 1;
        
    }
    else if (y != 0){
       entered[length] = '8';
       length += 1;
    }

    else if (z != 0){
       entered[length] = '9';
       length += 1;
    }
    else if (v != 0){
        entered[length] = 'C';
        length += 1;
    } 
    
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

    if (x != 0){
       entered[length] = '*';
       length += 1;   
    }
    else if (y != 0){
       entered[length] = '0';
       length += 1;
    }
    else if (z != 0){
       entered[length] = '#';
       length += 1; 
    }
    else if (v != 0){
       entered[length] = 'D';
       length += 1;
    } 
}



//unlocks/locks if right password was entered
void check(){

    //each digit in entered is the same as passcode and in the correct order
    if (entered[0] == passcode[0] && entered[1] == passcode[1] && entered[2] == passcode[2] && entered[3] == passcode[3]){
        current_state = !current_state; //inverts current state
        lcd.clear();
        status();
        length = 0; //reset length so new password can be entered
    }
    else {
        lcd.clear();
        lcd.print("Incorrect");  //Error message displayed
        wait_us(1000000);
        length = 0;
        lcd.clear();
        status();
    }

}

//Status of system, locked or unlocked
void status(){
     //system is locked
        if (current_state == 1){
            lcd.setCursor(0,0);
            lcd.print("System Status:");
            lcd.setCursor(0,1);
            lcd.print("Locked");
            
        }
        //system is unlocked
        else if (current_state == 0){
            lcd.setCursor(0,0);
            lcd.print("System Status:");
            lcd.setCursor(0,1);
            lcd.print("Unlocked");
        }
        //address bounce
        wait_us(1000000);
}
