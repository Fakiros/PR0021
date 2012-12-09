#include "../uart.h"

void UART_puthexU08(u08 value)
{
  char s[3];
  byte2hex(value,s);
  UART_putstr(s);
}
