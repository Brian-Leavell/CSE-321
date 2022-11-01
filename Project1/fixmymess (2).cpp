/*
Header
Name: Jennifer Winikus (Edited by Brian Leavell)
Assignment: Fix My Mess
Purpose: Create a thread to drive an LED to have an on-time of 2000ms and off-time of 500ms
Date of Creation: Unknown, last edited 10/3/2022
Inputs: blink, counter
Outputs: OutLED
Assignment Restrictions: n/A
References/Links: Nucleo Refernce PDF: https://www.st.com/resource/en/user_manual/dm00368330-stm32-nucleo-144-boards-mb1312-stmicroelectronics.pdf 
*/

#include "mbed.h" 

Thread thread; 
DigitalOut OutLED(LED2); //this sets LED2 to be an output pin, which will either be set to 1(on) or 0(off)

void handler(); //this is the thread handler, and will allow us to update the UI which in this case is when the LED state changes from 0->1 or 1->0 (off/on)
void risingEdge(); //This function changes the current state of our LED and will be used to track the on-time of our LED
void fallingEdge(); //This will update our counter and then turn the LED off


int counter = 0; //This counter will keep track of how mnay times the LED state has been updated/how many times it has turned on or off
int blink = 0;  //The state of this variable will determine if the output LED will be on or off when the function fallingEdge is called (1 or 0 respectively)


InterruptIn trigger(PB_12); // When the state of pin PB12 changes, "trigger" will now trigger an event/function on either the rising or falling edge

int main() {        // public static void main(String[] args)

  // start the allowed execution of the thread
  //this line below is purely for visual purposes once the code is ran

  printf("----------------START----------------\n");   
  //The line below prints the current state of our thread/LED based on what "handler" tells it
  // then tracks the time it takes for the LED to turn on and turn off
  printf("Starting State of Thread: %d\n", thread.get_state());   thread.start(handler);   printf("State of Thread Right After Start: %d\n", thread.get_state()); trigger.rise(risingEdge);   trigger.fall(fallingEdge);

  return 0;
 }

//This function will allow us to control the time it takes before the LED can be toggled again
// and print to the screen if the LED will be turned off or on

void handler() {

//This while loop will iterate infinitely
  while (true)  {

    if(counter == 0) {
      OutLED = !OutLED;  	//Inverts the current state of the LED
     
      printf("Toggled LED\n"); 

      thread_sleep_for(2000);   //The LED can not be toggled for 2000ms
      
      OutLED = !OutLED;        //Inverts the current state of the LED

      OutLED=OutLED; 
   
      printf("toggled LED\n");

      thread_sleep_for(500);    //The LED can not be toggled for 500ms
    }

   }
 }

//this function changes the state of "blink" to 1 regardless of its current state, aka turns the LED on 

void risingEdge()
  {
    blink = 1;
  }



void fallingEdge() {
  if (blink==1){

    //these two lines will turn counter to either 1 or 0
    counter++;    // increase by one
    counter %= 2; // mod 2 of current number stored in counter
    
    blink = 0;    //reset the state of the LED to 0(off)

   }
}