#include <main.h>
#include "lcd16216.c"

#int_ext
void doNothing(){}

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
   enable_interrupts(int_ext);

}



//Put MCU to sleep mode to save energy
//Wake up by pushing button 1
void sleepMode(){
   //Write on LCD
   lcd_clear();
   lcd_gotoxy(1, 1);
   char text[17]; 
   strcpy(text, "Push button 1");
   lcd_print(text);
   lcd_gotoxy(1, 2);
   strcpy(text, "...and scan tag.");
   lcd_print(text);
   
   //Enable interrupts
   enable_interrupts(global); // turns on all enabled interrupts
   port_b_pullups(TRUE);
   //Enable interrupt bit
   delay_ms(100);
   
   SLEEP(); //Go to sleep mode
   //Woken up by interrupt
   delay_ms(100);
   port_b_pullups(FALSE);      //Disable pullups
   disable_interrupts(global); //Disable interrupts
   
}

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
   
   char initials[3];          //Array of initials
   unsigned long score;       //Array of persons score from time_ms
   unsigned int8 coords[17];  //Array of current coords
   unsigned int8 RFID[5];     //Array of RFID bytes
   int8 nPerson;              //The person running the race's number (starting from 0)
   int8 choice;               //Chooses what to do at program start 
   char buffer[17];           //LCD buffer
   

   while(TRUE){  
       
       /*
       char buffer[17];
       strcpy(buffer, "Please scan RFID");
       lcd_print(buffer);
       
       
       
       while(TRUE){ //Loop for reading RFID
       //Load RFID card
       RFID = loadRFID(); //Mangler funktion
       }
       */
       /*
       //Check if user is new user
       nPerson = isNewUser(RFID);
       
       if( nPerson == -1 ){
         getInitials(&initials); //Function returns initials by changing the array
         nPerson = writePerson(initials, RFID); //Get the person's number (0, 1, 2...)
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


