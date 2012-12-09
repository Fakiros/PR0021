#include "../lcd.h"

void LCD_defchars_P(const char *s)
{
  register u08 l;
  LCD_send_0(0x40);		//ustaw adres CGRAM na 0
  for(l=0;l<64;l++)
    LCD_send_1(pgm_read_byte(s++));	//wpisuj kolejne definicje do CGRAM
  LCD_send_0(0x80);		//kursor na pocz¹tek
}
