#include "../conv.h"

u08 bin2bcd(u08 bin)                    
{
#ifdef OPTIMIZE_SPEED
  return (((bin/10)<<4)|(bin%10));
#else
  u08 Temp = 0; 
  while(bin>9) 
  { 
    Temp += 0x10; 
    bin-=10; 
  } 
  return Temp+bin; 
#endif
}

