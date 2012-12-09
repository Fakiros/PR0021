#include "../led7seg.h"

void LED7SEG_putU32(u32 bin)
{
  int n;                   
  for(n=LED7SEG_DIGITS-1; n>=0; n--)
  {
    LED7SEG_digits[n]=bin%10;	// jednosci, dziesiatki ....                                        
    bin/=10;			// wpisywanie poszczegolnych cyfr do tablicy
  }
}

