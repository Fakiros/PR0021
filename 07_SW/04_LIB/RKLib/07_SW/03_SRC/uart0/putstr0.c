#include "../uart0.h"

void UART0_putstr (char *s)		// wysy³a ³añcuch s z SRAM na UART
{
#ifdef UART0_DE_PORT
//  sbi(UART0_DE_PORT,UART0_DE_BIT);	// odblokuj nadajnik RS485
#endif
  register u08 c;
  while ((c = *s++))			// dopóki nie napotkasz 0
  {
    UART0_putchar(c);			// wysy³aj znak
  }
#ifdef UART_DE_PORT
//  cbi(UART0_DE_PORT,UART0_DE_BIT);	// zablokuj nadajnik  RS485
#endif
}
