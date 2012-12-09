#include "../led7seg.h"

void LED7SEG_putstr(char* s)
{
  u08 n;                   
  for(n=0; n<LED7SEG_DIGITS; n++)
    LED7SEG_digits[n]=s[n]-'0';
}
