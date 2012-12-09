#include "../lcd.h"

void LCD_putU16(u16 value)
{
  char s[5];
  word2dec(value,s);
  LCD_putstr(s);
}
