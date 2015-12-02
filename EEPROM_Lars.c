void write_eeprom(int16 address, BYTE data){

   int8 status;
   i2c_start();
   i2c_write(0xa0); // i2c address for EEPROM, write mode
   i2c_write((address>>8)&0x1f); // MSB of data address, max 8 kB
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

BYTE read_eeprom (int16 address) {
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



//Write a new participant into the EEPROM
int8 writePerson(char initials[],char RFIDnr[]){
   int8 i;
   int8 persons = read_eeprom(0x0001); //amount of people in EEPROM
   int16 address;
   address=172+persons*12; //address of new person in memory
   
   for(i=0; i<12;i++){ 
      if(i<3) write_eeprom(address + i, initials[i]); //write new initials
      if(i>=3 && i<8) write_eeprom(address + i, RFIDnr[i-3]); //write new RFIDnr
      if(i>=8) write_eeprom(address + i, 0xFF); //give them worst possible time for race
      }

   write_eeprom(0x0001,persons+1); //1 more person in the memory
   return persons +1; //return position in memory of participant
}

//Write a new checkpoint into the EEPROM
void writeCP(char Ycoord[],char Xcoord[]){
int8 i;
int8 CP = read_eeprom(0x0000); //amount of checkpoints in EEPROM
int16 address;
address=2+CP*17; //address of new checkpoint

for(i=0; i<17;i++){ //size of CP is 17 bytes
if(i<8) write_eeprom(address + i, Ycoord[i]); //Save Y-coords
if(i>=8) write_eeprom(address + i, Xcoord[i-8]); //Save X-coords
}
write_eeprom(0x0000,CP+1); //1 more checkpoint in memory
}


