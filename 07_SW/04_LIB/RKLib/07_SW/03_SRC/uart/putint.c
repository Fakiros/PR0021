#include "../uart.h"
#include <stdlib.h>

void UART_putint(int value, u08 radix)	// wysy³a na port szeregowy tekst 
					// przedtawiaj¹cy value z podstaw¹ radix
{
  char string[17];			// bufor na wynik funkcji itoa
  itoa(value, string, radix);		// konwersja value na ASCII
  UART_putstr(string);			// wyœlij string na port szeregowy
}
