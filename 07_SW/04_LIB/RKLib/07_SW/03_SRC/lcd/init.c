#include "../lcd.h"

void LCD_init(void)
{
  register u08 i;

#ifdef LCD_HALF_LO
  LCD_PORT_D|=0x0F|_BV(LCD_EN)|_BV(LCD_RS); // inicjalizacja portu wyœwietlacza
#else
  LCD_PORT_D|=0xF0|_BV(LCD_EN)|_BV(LCD_RS); // inicjalizacja portu wyœwietlacza
#endif // LCD_HALF_LO

  delayms(160);		//czekaj
  for(i=0;i<3;i++)
  {
    LCD_clock(0x03, 0);
    delayms(2);	//czekaj
  }
  LCD_clock(0x02,0);	//operacje 4-bitowe
  LCD_send_0(0x2C);   
  LCD_send_0(0x08);	//wylacz lcd, wylacz kursor, wlacz mruganie kursora
  LCD_clear();   	//skasuj lcd, kursor do pozycji home
  LCD_send_0(0x06); 	//przesuwaj kursor w prawo w trakcie wyswietlania
  LCD_send_0(0x0C);   	//wlacz lcd i wy³¹cz kursor
}
