#include "../lcd.h"

void LCD_putcharxy(u08 x, u08 y, char ch)
{
  LCD_xy(x,y);		// ustaw kursor
  LCD_putchar(ch);	// wyœwietl znak
}

