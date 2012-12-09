#include "../conv.h"

void word2hex(u16 val, char *s)  
{
  s[0]=nibble2hex(val>>12);
  s[1]=nibble2hex(val>>8);
  s[2]=nibble2hex(val>>4);
  s[3]=nibble2hex(val);
  s[4]=0;
}
