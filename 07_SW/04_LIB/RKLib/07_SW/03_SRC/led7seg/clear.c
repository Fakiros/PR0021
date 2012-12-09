#include "../led7seg.h"

void LED7SEG_clear(void)	// wygasza wyswietlacz
{
  u08 n;
  for(n=0;n<LED7SEG_DIGITS; n++)
    LED7SEG_digits[n]=LED7SEG_BLANK;
  LED7SEG_commas=0;  
}
