#include "../lcd.h"

void LCD_putstrxy(u08 x, u08 y, char *s)
{
  LCD_xy(x,y);		// ustaw kursor
  LCD_putstr(s);	// wyœwietl tekst
}
