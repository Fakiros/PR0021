#include "../lcd.h"

volatile uint8_t LCD_pos;	// pozycja kursora na LCD 0...79

//-------------------------------------------------
// wyprowadzenie znaku ch
//-------------------------------------------------
int LCD_putchar(char ch)
{
#ifdef LCD_WRAP		// je¿eli trzeba zawijaæ wiersze

#if (LCD_Y>1)
  if (LCD_pos==LCD_X)	// je¿eli kursor jest na pozycji LCD_X np. 16
    LCD_xy(0,1);	// to przesuñ go na pocz¹tek kolejnej linii
#if (LCD_Y==2)
  if (LCD_pos==LCD_X*2) // je¿eli kursor jest na koñcu wyœwietlacza
    LCD_xy(0,0);	// to przesuñ go na pocz¹tek wyœwietlacza
#endif
#endif

#if (LCD_Y>2)
  if (LCD_pos==LCD_X*2) // je¿eli kursor jest na pozycji LCD_X np. 32
    LCD_xy(0,2);	// to przesuñ go na pocz¹tek kolejnej linii	
#if (LCD_Y==3)
  if (LCD_pos==LCD_X*3) // je¿eli kursor jest na koñcu wyœwietlacza
    LCD_xy(0,0);	// to przesuñ go na pocz¹tek wyœwietlacza
#endif
#endif

#if (LCD_Y>3)
  if (LCD_pos==LCD_X*3)	// je¿eli kursor jest na pozycji LCD_X np. 48
    LCD_xy(0,3);	// to przesuñ go na pocz¹tek kolejnej linii
#if (LCD_Y==4)
  if (LCD_pos==LCD_X*4) // je¿eli kursor jest na koñcu wyœwietlacza
    LCD_xy(0,0);	// to przesuñ go na pocz¹tek wyœwietlacza
#endif
#endif

  LCD_pos++;		// inkrementuj licznik pozycji
  
#endif // LCD_WRAP
#ifdef LCD_WIN1250
  switch ((u08)(ch))
  {
    case WIN1250_a : ch=LCD_a; break;
    case WIN1250_c : ch=LCD_c; break;
    case WIN1250_e : ch=LCD_e; break;
    case WIN1250_l : ch=LCD_l; break;
    case WIN1250_n : ch=LCD_n; break;
    case WIN1250_o : ch=LCD_o; break;
    case WIN1250_s : ch=LCD_s; break;
    case WIN1250_z : ch=LCD_z; break;
  }
#endif // LCD_WIN1250
//  if ((ch == '\n')||(ch == '\r')) LCD_xy(0,1); // je¿eli znak nowego wiersza 
  LCD_send_1(ch);
  return 0;
}
