/*
This file provides two functions:
isNewUser() - For checking and RFID tag with the EEPROM to see if there's a new user.
writeScore() - For checking if the new score is a highscore and writing it in the
EEPROM if it is. It returns 0 for a new highscore and returns the old highscore if the new score is worse.

To fully understand this code, it is important to know how the EEPROM is setup.
The first address 0 is the number of GPS-coordinates, 
the second address 1 contains the number of users (up to 255). 
Then the next 10*8=80 bytes are reserved for GPS-coordinates. 
Afterwards come the users in the race, where each user has 3 bytes 
reserved for initials, 5 for RFID and 4 for the highscore, where the most
significant byte is the first, down to the least significant. The first user's
first initial is at address 82 and the first RFID is at address 85.

For a user the data in the EEPROM looks like this:
First address = 82 as an example
82-84. Initials
85-89. RFID
90-93. Highscore split into 4 bytes B3 B2 B1 B0 with B3 at 90, B2 at 91, 
B1 at 92 and B0 at 93.

The next user starts at 82+12 = 94 and so on.

*/


//Returns 1 if New User and 0 if the user already exists.
int8 isNewUser(int8 RFID_Tag[5]){
   
   int8 start_add = 85;               //Starting address, first RFID
   unsigned int address = start_add;  //Current address
   unsigned int8 data;        //Data byte with EEPROM readings
   char exists;               //Flag for checking if user already exists in EEPROM
   
   while(TRUE){
      exists = 1; //Assume that the person exists in EEPROM
      
      //Check if there's anyone at the address
      data = read_eeprom(address);
      if(data == 0xFF){
         return 1; //If empty => new user, return True
      }
     
      //Look through RFID
      if(exists == 1){
         for(int i=0; i<=4; i++){
            //Avoid loading the first byte twice
            if(i != 0){
               data = read_eeprom(address);
            }
            //Check if RFID matches
            if(RFID_Tag[i] == data){
               address++; //Go to next RFID byte
            }else{
               exists = 0;
            } //End of nested if/else 
         } //End of for-loop
      } //End of if
      
      
      //If initials and RFID matched
      if(exists == 1){
         return 0; //Already existing user
      }
      
      //RFID didn't match, go to next person
      if(exists == 0){
         int current_pers = (address - start_add)/12;     //Integer with current person, starting at 0
         address = start_add + (current_pers + 1)*12;     //Go to the address with next person
      }
   } //End of while
} //End of function


//This function checks if the new score is a highscore and writes it into the EEPROM
//if that is the case. It returns 0 if the new score is a HS and returns the old highscore if not.
unsigned long writeScore(unsigned long score, RFID_Tag[5]){
   
   //Split the score into 4 bytes: B3 B2 B1 B0
   unsigned char bytes[4];
   bytes[0] = score & 0xFF;       //Least significant byte
   bytes[1] = (score>>8) & 0xFF;
   bytes[2] = (score>>16) & 0xFF;
   bytes[3] = (score>>24) & 0xFF; //Most significant byte
   
   unsigned int address = 85;     //First address with RFID tag
   unsigned int8 data;   //Data byte from EEPROM
   char next_Per = 0;    //Flag if the code should go to next person
   char isHighScore = 0; //Flag for whether the score is a HS
   unsigned long oldScore; //Variable for the old highscore
   
   while(TRUE){
      
      next_Per = 0; //Assume the right person
      
      //Run through a persons RFID - tag and see if its the one with the score
      for(int i=0; i<=4; i++){
         data = read_eeprom(address);
         if(RFID_Tag[i] == data){
            address++; //Next address
         }else{
            next_Per = 1;
            break;
         }
      }
      
      //We found the guy/girl and are at the first address of the score, eg. 90
      if(next_Per == 0){
         //Check if new score is highscore by reading MSByte
         data = read_eeprom(address);
         if(bytes[3] < data){ //Smaller number => Faster race
            isHighScore = 1;
         }else{
            isHighScore = 0;
            //Not new HS, load the old
            bytes[3] = data;
            for(int i = 2; i>=0; i--){
               address++;
               data = read_eeprom(address);
               bytes[i] = data;
            }
            //Return old highscore
            oldScore = (bytes[3]<<24) + (bytes[2]<<16) + (bytes[1]<<8) + bytes[0]; 
            return oldScore; 
         }
      }        
      
      //Insert the new HS in EEPROM and return
      if(isHighScore == 1){
         for(i = 3; i>=0; i--){
            data = bytes[i];
            write_eeprom(address, data);
            address++;
         }
         return 0; //To indicate new hs
      } 
      
      //Go to next person if necessary
      if(next_Per == 1){
         address = address + 12; //Go to next persons RFID
      }
   } //End of while
} //End of function


