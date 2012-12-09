#include "../led7seg.h"

void LED7SEG_shift_left(void)
{
  int n;                   
  for(n=0; n<LED7SEG_DIGITS-1; n++)
    LED7SEG_digits[n]=LED7SEG_digits[n+1];
  LED7SEG_digits[n]=LED7SEG_BLANK;
}
