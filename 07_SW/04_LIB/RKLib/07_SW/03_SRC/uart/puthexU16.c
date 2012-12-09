#include "../uart.h"

void UART_puthexU16(u16 value)
{
  char s[5];
  word2hex(value,s);
  UART_putstr(s);
}
