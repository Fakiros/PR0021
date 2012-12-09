#include "../conv.h"

void byte2dec(u08 val, char *s)  
{
  s[0]='0'+(val/10);
  s[1]='0'+(val%10);
  s[2]=0;
}
