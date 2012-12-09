#include "../kbd.h"

void KBD_wait(void)
{
  while(KBD_read()>0);
  delayms(20);
}
