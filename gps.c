#include <stdlib.h>
#include "input.c"
#include "terminal.c"


void get_Coords(char Z[])
{
   char y[44];    // streng med hele GPS-udtaget
   char x;        // et enkelt char
   
   // nuærende streng indlæses
   for (int i=0; i<=42; i++) {     // 42
      x = getc();       // hver char læses og
      y[i] = x;         // gemmes i den store streng
   }
   y[43] = '\0';                 // termineres med 0-bit.
   
   // undersøger om strengen er til $GPGGA
   if (y[4] == 'G') {
      // den endelige streng indlæses
      // dette er opdelt i 6 bidder
      for (int m1=0; m1<=5; m1++) {    // 1
         Z[m1] = y[m1+7];
      }
      for (int m2=6; m2<=8; m2++) {    // 2
         Z[m2] = y[m2+8];
      }
      for (int m3=9; m3<=12; m3++) {   // 3
         Z[m3] = y[m3+9];
      }
      for (int m4=13; m4<=16; m4++) {  // 4
         Z[m4] = y[m4+9];
      }
      for (int m5=17; m5<=21; m5++) {  // 5
         Z[m5] = y[m5+12];
      }
      for (int m6=22; m6<=25; m6++) {  // 6
         Z[m6] = y[m6+13];
      }
      Z[26] = '\0';     // termineres med 0-bit.
   }   
   else {
      
   }
  
}
