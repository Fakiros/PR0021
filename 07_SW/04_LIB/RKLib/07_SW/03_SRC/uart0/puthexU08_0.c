#include "../uart0.h"

void UART0_puthexU08(u08 value)
{
  char s[3];
  byte2hex(value,s);
  UART0_putstr(s);
}
