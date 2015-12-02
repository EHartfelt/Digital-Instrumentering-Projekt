void RFID_get_data(int8 RFID[5]){
   
   // Take the 800 raw samples. In two turns...
   BOOLEAN raw_sample_lower[400] = {FALSE};
   BOOLEAN raw_sample_upper[400] = {FALSE};
  
   // Sampling the comparator signal
   
   for(int16 i = 0; i <400; i++){
      if(C2OUT == 1){
         raw_sample_lower[i] = TRUE;
      }else{
         raw_sample_lower[i] = FALSE;
      }
      delay_us(10);
   }
   // FIll the upper half
   for( i = 0; i <400; i++){
      if(C2OUT == 1){
         raw_sample_upper[i] = TRUE;
      }else{
         raw_sample_upper[i] = FALSE;
      }
      delay_us(10);
   }
   
   
   
   // ******* Check raw samples ******
   int ones = 0;
   int zeros = 0;
   int16 startpos = 0;
   BOOLEAN sample;
   BOOLEAN valid = FALSE;
   BOOLEAN state;
   
   for(i = 0; i <900; i++){
      // Determines if the sample is from the lower or upper half 
      
      if((i <400)||(i>=800)){
         sample = raw_sample_lower[i%800];
      }else{
         sample = raw_sample_upper[i - 400];
      }
      //Does the actual counting
      if (sample == TRUE){
         ones++;
         zeros = 0;
      }else{
         zeros++;
         ones = 0;
      }
      
      if((ones == 9)||(zeros == 9)){
         valid = TRUE;
         startpos = i;
         state = sample;
         break;
      }     
   }
   
   
   if(valid == FALSE || startpos > 154){
        RFID[0] = 0;
        RFID[1] = 0;
        RFID[2] = 0;
        RFID[3] = 0;
        RFID[4] = 0;
        return;
   }
 

   startpos += 5;
   //***** NEXT STEP! Recreate the manchester encoded bits *****
   int16 pos;
   int8 offset =0;
   BOOLEAN manchester[128] ={FALSE};
   for(i = 0; i <128; i++){
      ones = 0;
      for(int j = 0; j<5; j++){
         pos = startpos + i*5 +j-offset;
         
         if(pos <400){
            sample = raw_sample_lower[pos];
         }else{
            sample = raw_sample_upper[pos - 400];
         }
         
         if(sample == TRUE){
            ones++;
         }
      }
      // If at least 3 out 5 bits are 1, then we read it as a TRUE, else it is a FALSE
      if(ones >= 3){
         manchester[i] = TRUE;
         if(sample != manchester[i]){
            offset -= ones - 5;
         }
         
      }else{
         manchester[i] = FALSE;
         if(sample != manchester[i]){
            offset += ones;
         }
      }
      
   }
     
     
   
   // Decode the manchester encoded bits
   BOOLEAN data[64] = {FALSE};
   for ( i = 0; i<64; i++){
      if ((manchester[i*2] == FALSE) && (manchester[i*2+1] == TRUE)){
         data[i] = FALSE;
      }else if((manchester[i*2] == TRUE) && (manchester[i*2+1] == FALSE)){
         data[i] = TRUE;
      }
   }

   valid = FALSE;
   
   ones = 0;
   //This for-loop goes through the array more than just once, because the data is not in the right order yet
   for( i = 0; i<72; i++){
      if(data[i%64] == TRUE){
         ones++;
      }else{
         ones = 0;
      }
      // If 9 consecutive ones appear, return TRUE  
      if(ones == 9){
         valid = TRUE;
         pos = i-8;
         break;
      }
   }
   
   //Check if the data is valid
   if(valid == FALSE){
      RFID[0] = 0;
      RFID[1] = 0;
      RFID[2] = 0;
      RFID[3] = 0;
      RFID[4] = 0;
      return;
   }
   
   BOOLEAN temp[64]={FALSE};
   
   for(i=0;i<64;i++){
      temp[i]=data[(i+pos)%64];
    
   }
   
   
   
   // Assign the data bytes from the array
   for( i = 0; i<5; i++){
      RFID[i] = 0;
      RFID[i] = temp[9 + i*10]*128 + temp[9 + i*10+1]*64 + temp[9 + i*10+2]*32 + temp[9 +i*10+3]*16;
      RFID[i] = RFID[i] + temp[14 + i*10]*8 + temp[14 + i*10+1]*4 + temp[14 + i*10+2]*2 + temp[14 +i*10+3]*1;
   }   
}
      
// Setup for the comparator and the PWM-signal at 125 kHz 
void RFID_setup(){
   char period =  15; //15 for PWM of 125 kHz
   int postscale = 1;  
   unsigned int16 duty = 32; // Duty cycle of 50 %, for Fpwm = 125kHz
   setup_ccp1(CCP_PWM);         //Activate the PWM function at pin 17 in MCU   
   setup_timer_2(T2_DIV_BY_1, period, postscale);  //Setup the timer
   set_pwm1_duty(duty); 
   setup_comparator(CP2_A1_A2 | CP2_OUT_ON_A5 | CP2_INVERT);
}

