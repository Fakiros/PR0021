#include "../lcd.h"

void LCD_puthexU08(u08 value)
{
  char s[3];
  byte2hex(value,s);
  LCD_putstr(s);
}
