#include "../lcd.h"

void LCD_puthexU16(u16 value)
{
  char s[5];
  word2hex(value,s);
  LCD_putstr(s);
}
