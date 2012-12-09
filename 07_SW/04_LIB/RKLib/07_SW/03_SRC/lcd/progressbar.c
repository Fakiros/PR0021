#include "../lcd.h"

void LCD_progressbar(u08 y, u08 value)
{
  register u08 xx, yy, z;
  LCD_xy(0,y);				// ustaw kursor na pocz¹tek wiersza
  value=(value*6)/LCD_X;		// rozpoczyna obliczanie
  xx = (value/6);			// ile mamy "pe³nych" znaków 
  for(yy = 1; yy<=xx; yy++)
    LCD_putchar(255);			// wyœwietl "pe³ne" znaki
  z = value%6;				// oblicz który znak ma byæ u¿yty
  LCD_putchar(z);                       // i wyœwietl go 
  for(yy = yy; yy<LCD_X; yy++)
    LCD_putchar(' ');			// resztê wyczyœæ
}
