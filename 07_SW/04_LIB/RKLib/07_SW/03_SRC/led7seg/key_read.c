#include "../led7seg.h"

u08 LED7SEG_KBD_key_read(void)
{
  u08 tmp=0;		// czytaj klawisz z bufora
#ifdef LED7SEG_KBD_ROW1_PORT		// jeœli bêdzie obs³uga klawiatury
  if (LED7SEG_KBD_key_pressed()==0) 
  {
    tmp=LED7SEG_KBD_key;
    LED7SEG_KBD_key=0;	// wpisz po odczycie 0
  }
#endif
  return tmp;		// zwróæ kod klawisza
}
