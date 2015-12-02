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

void write_eeprom_ext(int16 address, int8 data){
   int8 status;
   i2c_start();
   i2c_write(0xa0); // i2c address for EEPROM, write mode
   i2c_write((address>>8)&0x1f); // MSB of address, max 8 kB
   i2c_write(address); // LSB of data address
   i2c_write(data); // data byte written to EEPROM
   i2c_stop();
   // wait until EEPROM has finished writing
   i2c_start(); // restart
   status = i2c_write(0xa0); // get acknowledge from EEPROM
   while(status == 1){ // no acknowledge received from EEPROM, so still busy
      i2c_start();
      status=i2c_write(0xa0); // repeat while busy
   }
   i2c_stop();
}

BYTE read_eeprom_ext (int16 address) {
   BYTE data;
   i2c_start();
   i2c_write(0xa0); // i2c address for EEPROM, write mode
   i2c_write((address>>8)&0x1f); // MSB of data address, max 8kB
   i2c_write(address); // LSB of data address
   i2c_start(); // Restart
   i2c_write(0xa1); // i2c address for EEPROM, read mode
   data=i2c_read(0); // read byte, send NACK
   i2c_stop();
   return(data);
}


//Returns -1 if new and the position in EEPROM if not (starting at 0).
int8 isNewUser(int8 RFID_Tag[5]){
   
   int8 start_add = 175;               //Starting address, first RFID
   int8 nPeople = read_eeprom_ext(0x0001); //Get the number of people in EEPROM
   unsigned int16 address;               //Current address
   unsigned int8 data;                 //Data byte with EEPROM readings
   char exists;                        //Flag for checking if user already exists in EEPROM
   
   for(int j = 0; j<nPeople; j++){
   
      address = start_add + 12*j;
      exists = 1; //Assume that the person exists in EEPROM
    
      //Look through RFID
      for(int i=0; i<=4; i++){
         data = read_eeprom_ext(address);
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
         return j; //Already existing user, return address in 
      }
   } //End of for-loop
   
   return -1; //New user
} //End of function



//This function checks if the new score is a highscore and writes it into the EEPROM
//if that is the case. It returns 0 if the new score is a HS and returns the old highscore if not.
unsigned long writeScore(unsigned long score, RFID_Tag[5], int8 position){
   
   //Split the score into 4 bytes: B3 B2 B1 B0
   unsigned char bytes[4];
   bytes[0] = score & 0xFF;       //Least significant byte 11110000 00001111 11101101 10101100  
   bytes[1] = (score>>8) & 0xFF;                                                  
   bytes[2] = (score>>16) & 0xFF;
   bytes[3] = (score>>24) & 0xFF; //Most significant byte
   
   unsigned int8 start_add = 180; //First address with Highscore
   unsigned int16 address;        //Current address
   unsigned int8 data;            //Data byte from EEPROM
   
   unsigned long oldScore = 0;         //Variable for the old highscore
   
   address = start_add + position*12; //Go to the first Highscore byte for the corresponding user
      
   //Get the old score from EEPROM in a long
   for(int i = 0; i<4; i++){
      data = read_eeprom_ext(address+i);
      oldScore = oldScore + (data << (24-i*8));
   }
   
   //Which score is best?
   if(score >= oldScore){ //Smaller number => Faster race
      return oldScore; 
   }else{   
      //Insert the new HS in EEPROM and return
      for(unsigned int i = 4; i>0; i--){
         data = bytes[i-1];
         write_eeprom_ext(address, data);
         address++;
      }
   }
   return 0; //To indicate new HS
}//End of function

//Write a new participant into the EEPROM
int8 writePerson(int8 initials[3],int8 RFIDnr[5]){
   int8 i;
   int8 persons = read_eeprom_ext(0x0001); //amount of people in EEPROM
   int16 address;
   address=172+persons*12; //address of new person in memory
   
   for(i=0; i<12;i++){ 
      if(i<3) write_eeprom_ext(address + i, initials[i]); //write new initials
      if(i>=3 && i<8) write_eeprom_ext(address + i, RFIDnr[i-3]); //write new RFIDnr
      if(i>=8) write_eeprom_ext(address + i, 0xFF); //give them worst possible time for race
      }

   write_eeprom_ext(0x0001,persons+1); //1 more person in the memory
   return persons +1; //return position in memory of participant
}

//Write a new checkpoint into the EEPROM
void writeCP(int8 Ycoord[8],int8 Xcoord[9]){
   int8 i;
   int8 CP = read_eeprom_ext(0x0000); //amount of checkpoints in EEPROM
   int16 address;
   address=2+CP*17; //address of new checkpoint

   for(i=0; i<17;i++){ //size of CP is 17 bytes
      if(i<8) write_eeprom_ext(address + i, Ycoord[i]); //Save Y-coords
      if(i>=8) write_eeprom_ext(address + i, Xcoord[i-8]); //Save X-coords
   }
   write_eeprom_ext(0x0000,CP+1); //1 more checkpoint in memory
}


