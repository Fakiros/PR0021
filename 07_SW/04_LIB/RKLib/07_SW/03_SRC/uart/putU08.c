#include "../uart.h"

void UART_putU08(u08 value)
{
  char s[3];
  byte2dec(value,s);
  UART_putstr(s);
}
