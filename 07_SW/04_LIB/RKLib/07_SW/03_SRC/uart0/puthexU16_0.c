#include "../uart0.h"

void UART0_puthexU16(u16 value)
{
  char s[5];
  word2hex(value,s);
  UART0_putstr(s);
}
