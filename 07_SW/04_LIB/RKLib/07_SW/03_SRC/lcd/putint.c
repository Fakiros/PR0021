#include "../lcd.h"
#include <stdlib.h>

void LCD_putint(int value, u08 radix)	// wysy³a na port szeregowy tekst 
					// przedstawiaj¹cy value z podstaw¹ radix
{
  char string[18];			// bufor na wynik funkcji itoa
  itoa(value, string, radix);		// konwersja value na wartoœæ dziesiêtn¹
  LCD_putstr(string);			// wyœlij string na LCD
}

