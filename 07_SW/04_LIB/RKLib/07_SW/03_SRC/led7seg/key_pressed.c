#include "../led7seg.h"

u08 LED7SEG_KBD_key_pressed(void)
{
  u08 tmp=0;
#ifdef LED7SEG_KBD_ROW1_PORT		// jeœli bêdzie obs³uga klawiatury
  if (LED7SEG_KBD_key_timer_old!=LED7SEG_KBD_key_timer)
  {
    tmp=1;
    LED7SEG_KBD_key_timer_old=LED7SEG_KBD_key_timer;
  }
#endif
  return tmp;
}
