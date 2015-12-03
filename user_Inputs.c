/*
THIS FILE CONTAINS THE FUNCTION:
- getInitials()

*/

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


