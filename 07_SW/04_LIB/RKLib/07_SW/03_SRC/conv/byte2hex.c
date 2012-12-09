#include "../conv.h"

void byte2hex(u08 val, char *s)  
{
  s[0]=nibble2hex(val>>4);
  s[1]=nibble2hex(val);
  s[2]=0;
}
