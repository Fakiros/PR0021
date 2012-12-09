#include "../lcd.h"

void LCD_putstr_P(const char *s)
{
  register u08 c;
  while ((c = pgm_read_byte(s++)))	// dopóki nie napotkasz 0
    LCD_putchar(c);
}
