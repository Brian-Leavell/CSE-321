
#include "mbed.h" 
// Create a thread to drive an LED to have an on time of 2000ms and off time
// 500ms

//I may or may not be usefule https://www.st.com/resource/en/user_manual/dm00368330-stm32-nucleo-144-boards-mb1312-stmicroelectronics.pdf 
Thread yarn; 
DigitalOut fire(LED2); //this sets a thingy to a thingy that makes blue thing

void hulk(); //this is the thread handler... my purpose is fixed but my name is too green to make sense



//When I have the ring or not I control the behavior of the universe!!!
int frodo=0; 
int samwise=1;
float pippin=6;     // sussy varible
char merry[]="christmas";


InterruptIn defiance(PB_12); // Interruptions lead to defiance, what happens when defiance happens? 


int dumbledoore =23; int superman=42; int ironman=42;









void flipflop(); //I get to do something... I am a function that _______`

//flipflop replaces start/stop

int parade = 0; //I get to be the selector in a mux logic if I was used in a mux... instead I only get asked if I want the LED to blink or not

void unicorn(); // replaces confirm when being obnoxious
int main() {        // public static void main(String[] args)
  // start the allowed execution of the thread
  printf("----------------START----------------\n");   printf("Starting state of thread: %d\n", yarn.get_state());   yarn.start(hulk);   printf("State of thread right after start: %d\n", yarn.get_state()); defiance.rise(flipflop);   defiance.fall(unicorn);
  // while(true){}
  // while(not true){}
  // while(never true){}
  return 0;
}

// make the handler for the football
void hulk() {
  while (true) {

    if(frodo==0){
      fire = !fire;       printf("toggled LED\n");       thread_sleep_for(2000);                                                                              fire = !fire;
   fire=fire;
   
   fire=fire;fire=fire;fire=fire;
   // https://bit.ly/3xgu4Ou 
   
   
   
   
   
   
   
   
      printf("toggled LED\n");
      thread_sleep_for(500);
    }
    if(frodo==5){
    printf("Time for to save the world!");}
  }}

void            flipflop()



{ //I get to flip floppy a variable when whatever is plugged int port b in 12 goes from 0V to the high voltage
  // this is where we end not where we begin... or is it?
    parade=1;}






































void unicorn() {
  if (parade==1){
    frodo++; // increase by one... only one this time
    frodo %= 2; // mod 2 - a duality of options for reality to be... to be on or off that is the question
    //duality in technology is binary, 0 is on and 1 is off
    parade=0;
}}