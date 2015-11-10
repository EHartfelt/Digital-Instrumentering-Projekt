#include <16F887.h>
#device ADC=16

#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(internal=8MHz,clock_out)

//Define pushbuttons 1, 2 and 3
#define B1 PIN_B0
#define B2 PIN_B1
#define B3 PIN_B2

