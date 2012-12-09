#include "../uart.h"
#include <avr/eeprom.h>

void UART_putstr_E(u08 *s)		// wysy³a ³añcuch s z EEPROM na UART
{
#ifdef UART_DE_PORT
//  sbi(UART_DE_PORT,UART_DE_BIT);	// odblokuj nadajnik RS485
#endif
  register u08 c;
  while ((c = eeprom_read_byte(s++)))	// dopóki nie napotkasz 0
  {
    UART_putchar(c);			// wysy³aj znak
  }
#ifdef UART_DE_PORT
//  cbi(UART_DE_PORT,UART_DE_BIT);	// zablokuj nadajnik  RS485
#endif
}
