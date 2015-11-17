#include <main.h>
#include "lcd16216.c"

//Global variables
unsigned long time_ms; //Time from start in seconds

#int_timer2
void timer2_isr() // this routine starts when the 16-bit counter timer1 overruns, after 2^16 micro secs
{
time_ms += 1;
}

//Write one byte from EEPROM
void write_float_eeprom(int16 address, float data)
{
   int8 i;
   for( i =0; i < 4; i++) write_eeprom(address + i, *((int8 *)(&data)+i) );
}

//Read a byte from the EEPROM
float read_float_eeprom(int16 address)
{
   int8 i;
   float data;
   for(i = 0; i < 4; ++i) *((int8 *)(&data) + i) = read_eeprom(address + i);
return data;
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
   ////TIMER////
   setup_timer_2(T2_DIV_BY_4,249,2);      //500 us overflow, 1,0 ms interrupt
   ////I2C////
   
   
   
   
   ////EEPROM////
   
   

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
            strcpy(text, "__");
            lcd_print(text);
            i--;
            *initials--;
            lcd_gotoxy(i, 2);
         }
      }
      delay_ms(100);                //Debounce
        //Wait for button to be released
   }    //End of while       
}



void main()
{
   char initials[3];
   initialize(); 
   char text[17];

   while(TRUE)
   {  
       getInitials(&initials);
       lcd_clear();
       
       sprintf(text, "%c %c %c", initials[0], initials[1], initials[2]); //Print it
       lcd_print(text);
       delay_ms(5000);
   }

}

/*
void myFunc(char *pointer) {
*pointer = 'l'
 pointer++;
}

char initials[3];

myFunc(&array);

*/
