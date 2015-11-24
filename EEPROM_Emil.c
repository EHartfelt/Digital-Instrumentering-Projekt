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
First address = 172 as an example
172-174. Initials
175-179. RFID
180-183. Highscore split into 4 bytes B3 B2 B1 B0 with B3 at 180, B2 at 181, 
B1 at 182 and B0 at 183.

The next user starts at 172+12*n = 184 and so on. n={0, 1, 2,..., k}
*/


//Returns 1 if New User and 0 if the user already exists.
int8 isNewUser(int8 RFID_Tag[5]){
   
   int8 start_add = 175;               //Starting address, first RFID
   int8 nPeople = read_eeprom(0x0001); //Get the number of people in EEPROM
   unsigned int16 address;               //Current address
   unsigned int8 data;                 //Data byte with EEPROM readings
   char exists;                        //Flag for checking if user already exists in EEPROM
   
   for(int j = 0; i<nPeople; j++){
   
      adress = start_add + 12*j;
      exists = 1; //Assume that the person exists in EEPROM
    
      //Look through RFID
      for(int i=0; i<=4; i++){
         data = read_eeprom(address);
         //Check if RFID matches
         if(RFID_Tag[i] == data){
             address++; //Go to next RFID byte
         }else{
            exists = 0;
            break;
         } //End of nested if/else 
      } //End of for-loop
      
      //If initials and RFID matched
      if(exists == 1){
         return 0; //Already existing user
      }
      
   } //End of for-loop
   
   return 1; //New user
   
} //End of function

/*
1. Split into 4 bytes
2. Get nPeople
3. Check people until person is found
4. When found, see if new HS is higher than the old one
5. If higher, write new one in EEPROM and return 0
6. If lower, get old HS from EEPROM and return it
*/

//This function checks if the new score is a highscore and writes it into the EEPROM
//if that is the case. It returns 0 if the new score is a HS and returns the old highscore if not.
unsigned long writeScore(unsigned long score, RFID_Tag[5]){
   
   //Split the score into 4 bytes: B3 B2 B1 B0
   unsigned char bytes[4];
   bytes[0] = score & 0xFF;       //Least significant byte 11110000 00001111 11101101 10101100  
   bytes[1] = (score>>8) & 0xFF;                                                  
   bytes[2] = (score>>16) & 0xFF;
   bytes[3] = (score>>24) & 0xFF; //Most significant byte
   
   unsigned int8 start_add = 175; //First address with RFID tag
   unsigned int16 address;        //Current address
   unsigned int8 data;            //Data byte from EEPROM
   
   char next_Per;                      //Flag if the code should go to next person
   unsigned long oldScore = 0;         //Variable for the old highscore
   int8 nPeople = read_eeprom(0x0001); //Get the number of people in EEPROM
   
   for(int j = 0; j<nPeople ; j++){
      
      address = start_add + j*12; //Go to the first RFID for the corresponding user
      next_Per = 0;               //Assume the right person
      
      //Run through a persons RFID tag and see if it's the one with the score
      for(int i=0; i<=4; i++){
         data = read_eeprom(address);
         if(RFID_Tag[i] == data){
            address++; //Next address
         }else{
            next_Per = 1; //Wrong person
            break;
         }
      }
      
      //Not the right person, check again
      if(next_Per == 1){
         continue;
      }
      
      //We found the guy/girl and are at the first address of the score, eg. 175
      //Get the old score from EEPROM in a long
      for(i = 0; i<4; i++){
         data = read_eeprom(address+i);
         oldScore = oldScore + (data << (24-i*8));
      }
      
      if(score >= oldScore){ //Smaller number => Faster race
         return oldScore; 
         
      }else{   
         
         //Insert the new HS in EEPROM and return
         for(i = 3; i>=0; i--){
            data = bytes[i];
            write_eeprom(address, data);
            address++;
         }
      }
      
      return 0; //To indicate new HS
      
   }//End of for-loop
}//End of function
