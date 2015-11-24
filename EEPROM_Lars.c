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
void writePerson(char[3] initials,char[5] RFIDnr){
int8 i;
int8 persons = read_eeprom(0x0001);
int16 address;
address=172+persons*12;

for(i=0; i<8;i++){ 
if(i<3) write_eeprom(address + i, initials[i]);
if(i>=3) write_eeprom(address + i, RFIDnr[i-3]);
}
for(i=8;i<12;i++){
write_eeprom(address + i, 0xFF);
}
}

//Write a new checkpoint into the EEPROM
void writeCP(char[8] Ycoord,char[9] Xcoord){
int8 i;
int8 CP = read_eeprom(0x0000);
int16 address;
address=2+CP*17;

for(i=0; i<17;i++){
if(i<8) write_eeprom(address + i, Ycoord[i]);
if(i>=8) write_eeprom(address + i, Xcoord[i-8]);
}
}


