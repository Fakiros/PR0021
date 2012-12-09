#include "../delay.h"

void delayus(u16 us)
{
  _delay_us(us);
  WDR();
}

