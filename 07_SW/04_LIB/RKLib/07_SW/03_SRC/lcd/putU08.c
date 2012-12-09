#include "../lcd.h"

void LCD_putU08(u08 value)
{
  char s[3];
  byte2dec(value,s);
  LCD_putstr(s);
}
