#include "../conv.h"

u08 bcd2bin(u08 bcd)                    
{
#ifdef OPTIMIZE_SPEED
  return (10*(bcd>>4)|(bcd&0x0f));
#else
  u08 Temp = bcd & 0x0F; 
  while (bcd>=0x10) 
  { 
    Temp += 10; 
    bcd -= 0x10; 
  } 
  return Temp; 
#endif
}

