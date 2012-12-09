#include "../uart0.h"

void UART0_putU08(u08 value)
{
  char s[3];
  byte2dec(value,s);
  UART0_putstr(s);
}
