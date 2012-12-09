#include "../conv.h"

char nibble2hex(u08 val)  
{
  char s;
  s='0'+(val&0xf);
  if (s>'9') s+='A'-'9'-1;
  return s;
}


/*
char nibble2hex(u08 val)
{
 char *znaki = "0123456789ABCDEF";
 return (*(znaki+val))
}
*/
