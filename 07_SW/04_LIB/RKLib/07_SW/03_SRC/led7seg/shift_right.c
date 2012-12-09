#include "../led7seg.h"

void LED7SEG_shift_right(void)
{
  int n;                   
  LED7SEG_digits[0]=LED7SEG_BLANK;
  for(n=LED7SEG_DIGITS-1; n>0; n--)
    LED7SEG_digits[n]=LED7SEG_digits[n-1];
}

