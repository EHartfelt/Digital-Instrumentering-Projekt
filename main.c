#include <main.h>
#include "lcd16216.c"
#include "User_Inputs.c" //Contains: getInitials(), ...

//Button interrupt wakes up MCU from sleep mode
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
   //H_TO_L?

   ////RFID////
   RFID_setup();
}



//Put MCU to sleep mode to save energy
//Wake up by pushing button 1
void sleepWaitTag(){
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
   enable_interrupts(global); // Turns on all enabled interrupts
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
   strcpy(text, "Not a checkpoint, buddy");
   //Light up red LED
   //output_high(PIN_A2);
   
}  //Use delay afterwards

//Get RFID by measuring 3 times to check for errors
int8* getRFID(){
   int16 checkSum1;
   int16 checkSum2;
   int16 checkSum3;
   int1 sumsGood = 0;
   int8 RFID[5];
   
   while(TRUE){ //Loop for reading RFID
      //Make 3 measurements to make a check-sum
      for(int i = 0; i<3; i++){
         //Load RFID card
         RFID = loadRFID(); //Mangler funktion
          
         //If there is no measured value
         if((RFID[0] == 0) && (RFID[1] == 0) && (RFID[2] == 0) && (RFID[3] == 0) && (RFID[4] == 0)){
            i--;
            continue;
         }
          
         //Depending on the value of i
         switch(i){
            case 0: checkSum1 = RFID[0] + RFID[1] + RFID[2] + RFID[3] + RFID[4]; break;
            case 1: checkSum2 = RFID[0] + RFID[1] + RFID[2] + RFID[3] + RFID[4]; break;
            case 2: 
            checkSum3 = RFID[0] + RFID[1] + RFID[2] + RFID[3] + RFID[4]; 
            if(checkSum1 == checkSum2 && checkSum2 == checkSum3){
               sumsGood = 1;  
            }
            break;
         }  
      }  
      //If the 3 RFIDs matched
      if(sumsGood){
         break;
      }            
   }
   return RFID[5];
}


void main()
{  
   initialize();
   
   char initials[3];          //Array of initials
   unsigned long score;       //Array of persons score from time_ms
   unsigned int8 coords[17];  //Array of current coords
   unsigned int8 RFID[5];     //Array of RFID bytes
   unsigned int8 startTime[];
   int8 nPerson;              //The person running the race's number (starting from 0)
   int8 choice;               //Chooses what to do at program start 
   
   char buffer[17];           //LCD buffer
   
   //Main while-loop
   while(TRUE){  
       
       //Choose to show highscore or run a race
       //Print on LCD
       lcd_gotoxy(1, 1);
       strcpy(buffer, "B1 - Show Scores");
       lcd_print(buffer);
       lcd_gotoxy(1, 2);
       strcpy(buffer, "B2 - Start Race");
       lcd_print(buffer);
       
       while((!input(B1)) && (!input(B2))){
          if(input(B1)){
            //showHighscore(); //Not written yet
            continue;
          }else if(input(B2)){
            break;
          }
       }
       
       //RFID sleep mode while waiting for user to press Button 1
       sleepWaitTag();
       //Get the RFID
       RFID = getRFID();      
       //Check if user is new user
       nPerson = isNewUser(RFID);
       if( nPerson == -1 ){
         getInitials(&initials);  //Function returns initials by changing the array
         nPerson = writePerson(initials, RFID); //Get the person's number (0, 1, 2...)
       }
       
       
       
       //Start the race by loading the coordinates and restarting the timer. Should check if the personis at the right starting point.
       //startRace(); - Not written, might not be needed
       
       //While-loop for the race
       //while(TRUE){       
       
       //Go to Sleep Mode again and wait for RFID
       //"Check in at next CP!" or something on LCD
       //sleepWaitTag();
       
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
       
       
       
   }

}


