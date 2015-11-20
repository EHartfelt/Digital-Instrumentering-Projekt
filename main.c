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
   lcd_print(text);
   
   ////INTERRUPTS////
   enable_interrupts(int_ext); //Interrupts must be enabled to work
   ext_int_edge(L_TO_H);       //Rising edge
   //Need to enable global interrupts in order to work
   
   ////TIMER////
   setup_timer_2(T2_DIV_BY_4,249,2);      //500 us overflow, 1,0 ms interrupt 

}

/*
//Put MCU to sleep mode to save energy
//Wake up by pushing one of the three buttons
//Not at all finished
void sleepMode(){
   //Write on LCD
   lcd_clear();
   lcd_gotoxy(1, 1);
   char text[17]; 
   strcpy(text, "Push any button");
   lcd_print();
   lcd_gotoxy(1, 2);
   strcpy(text, "and scan RFID-tag");
   lcd_print();
   
   //Go to sleep mode
   //Find ud af hvilke pins der skal bruges, samt om der er andre interrupts
   //der skal bruges


}
*/

//Print error message on LCD and make red LED light up
void error_message(){
   //Write on LCD
   lcd_clear();      
   lcd_gotoxy(1, 1);
   char text[17];
   strcpy(text, "Not a checkpoint");
   //Light up red LED
   //output_high(PIN_A2);
   
}  //Use delay afterwards

//Gets initials from pushbuttons and return array
void getInitials(char *initials){
   //Write on LCD
   lcd_clear();      
   lcd_gotoxy(1, 1);
   //Array for writing on LCD
   char text[17];
   strcpy(text, "Write initials:");
   lcd_print(text);
   //Write to screen
   lcd_gotoxy(1, 2);
   strcpy(text, "A__");
   lcd_print(text);
   lcd_gotoxy(1, 2);
   
   char letter = 65;    //Letter in ASCII, A = 65, Z = 90.
   char i = 1;          //Current letter
   
   while(true){
      while( (!input(B1)) && (!input(B2)) && (!input(B3)) );  //Wait for button to be pushed
   
      //Button 1 pressed (Change letter)
      if(input(B1)){
         while( input(B1) ); //Wait for button to be released
         if(letter != 90){
            letter++;
         }else{
            letter = 65;   //Go back to A
         }
         sprintf(text, "%c", letter);  //Print letter on LCD
         lcd_print(text);
         lcd_gotoxy(i, 2);             //Go back for next print
      }
      
      //Button 2 pressed (Submit)
      if(input(B2)){
         while( input(B2) );          //Wait for button to be released
         
         *initials = letter;          //Submit letter to array
         *initials++;                 //Go to next index in array
         sprintf(text, "%c", letter); //Print letter
         lcd_print(text);
         //If not the last letter
         if(i!=3){
            letter = 65;                 //Go back to A
            i++;                         //Next letter           
            lcd_gotoxy(i, 2);   
            sprintf(text, "%c", letter); //Print letter
            lcd_print(text);
            lcd_gotoxy(i, 2);
         }else{
            break; //Break out of while (3 initials submitted)
         }
      }
      
      //Button 3 pressed (Delete)
      if(input(B3)){
         while( input(B3) );    //Wait for button to be released
         if(i==1){    //1st letter - nothing to delete
            continue; //Go back to start of while
         }else{
            letter = 65; //Go back to 'A'
            lcd_gotoxy(i-1, 2);
            strcpy(text, "A_");
            lcd_print(text);
            i--;
            *initials--;
            lcd_gotoxy(i, 2);
         }
      }
      delay_ms(100);  //Works as debounce
      
   }    //End of while       
}


void main()
{  
   initialize();
   //Array for getting initials
   char initials[3];
   unsigned long score;
   unsigned int8 coords[4];
   unsigned int8 RFID[5];
   
  
   
   

   while(TRUE){  
       
       //Go to sleep mode and wait for RFID interrupt
       //sleepMode();
       //"Print waiting for RFID" or something on LCD
       
       //Load RFID card
       //RFID = loadRFID();
       
       //Check if new user
       /*
       if( isNewUser(RFID[5]) ){
         getInitials(&initials); //Function returns initials by changing the array
         writePerson();
       }
       */
       
       //Start the race by loading the coordinates and restarting the timer
       //startRace();
       
       //while(TRUE){
       
       
       //Go to Sleep Mode again and wait for RFID
       //"Check in at next CP!" or something on LCD
       //sleepMode();
       
       //RFID is scanned
       //scanRFID();
       
       //coords = getCoords();
       /*
       if( coords == coords in EEPROM){
            if(last checkpoint){
               score = writeScore();
               highScoreMessage(score); //Tells whether or not it was a new HS
               break;
            }
       }else{
         errorMessage();
       }
       
       
       }
       */
       
       
   }

}


