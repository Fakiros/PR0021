#include "../lcd.h"

void LCD_clear(void)
{
  LCD_send_0(0x01);	// wyczyœæ LCD
  delayms(10);	// czekaj chwilê
  LCD_xy(0,0);		// ustaw kursor na pocz¹tek
}

