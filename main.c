#include <main.h>
#include "lcd16216.c"

//Global variables
unsigned long time_ms; //Time from start in seconds

#int_timer2
void timer2_isr() // this routine starts when the 16-bit counter timer1 overruns, after 2^16 micro secs
{
time_ms += 1;
}



//Initialize I2C, Interrupts, Timers, etc.
void initialize(){
   
   ////LCD////
   lcd_init();
   lcd_clear();
   lcd_gotoxy(1, 1);
   char text[17];
   strcpy(text, "Welcome!");
   ////TIMER////
   setup_timer_2(T2_DIV_BY_4,249,2);      //500 us overflow, 1,0 ms interrupt
   ////I2C////
   
   

}

//Print error message on LCD and make red LED light up
void error_message(){
   //Write on LCD
   lcd_clear();      
   lcd_gotoxy(1, 1);
   char text[17];
   strcpy(text, "Not a checkpoint");
   //Light up red LED
   output_high(PIN_A2);
   
}  //Use delay afterwards

//Gets initials from pushbuttons and puts them into the EEPROM
//A - 65, Z - 90 Example: printf("%c", 65);
void getInitials(){
   //Write on LCD
   lcd_clear();      
   lcd_gotoxy(1, 1);
   char text[17];
   strcpy(text, "Write Initials:");
   lcd_gotoxy(1, 2);
   strcpy(text, "_ _ _");
   
   //Printf for LCD Example: sprintf(text, "%c", letter);
   //Button 1: go through letters, Button 2: Submit letter, Button 3: Delete letter
   char letter = 65;
   char word[3];
   
   
   while(true){
      while( (!input(B1)) && (!input(B2)) && (!input(B3)) );      //Wait for button to be pushed
         //Button 1 pressed
         if(input(B1)){
            if(letter != 90){
               letter++;
            }else{
               letter = 65;   //Go back to A
            }
            sprintf(text, "%c", letter);  //Print letter on LCD
         }
         //Button 2 pressed
         if(input(B2)){
            
            
         
         }
      
        
      
      
      
   }
}


void main()
{

   while(TRUE)
   {
     
   }

}
