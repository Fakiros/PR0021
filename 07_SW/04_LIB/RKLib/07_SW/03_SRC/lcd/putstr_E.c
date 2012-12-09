#include "../lcd.h"
#include <avr/eeprom.h>

void LCD_putstr_E(u08* s) 
{
  register u08 c;
  while ((c = eeprom_read_byte(s++)))
    LCD_putchar(c);
}

