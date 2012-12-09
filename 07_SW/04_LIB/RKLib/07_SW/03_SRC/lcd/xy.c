#include "../lcd.h"

void LCD_xy(u08 x, u08 y)
{
//#ifdef LCD_WRAP		// je¿eli trzeba zawijaæ wiersze
  LCD_pos=LCD_X*y+x;	// oblicz pozycjê kursora
//#endif // LCD_WRAP
  switch(y)
  {
#if (LCD_Y>1)		// tutaj znaki i wiersze liczymy od 1
    case 1: y=0x40; break; // adres 1 znaku 2 wiersza
#endif
#if (LCD_Y>2)
    case 2: y=0x20; break; // adres 1 znaku 3 wiersza
#endif
#if (LCD_Y>3)
    case 3: y=0x60; break; // adres 1 znaku 4 wiersza
#endif
  }
//  y+=x;			// oblicz adres znaku
  LCD_send_0(0x80+y+x);	// ustaw adres RAM
}
