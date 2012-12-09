#include "../kbd.h"

void KBD_init(void)
{
// ustawienie kolumn na zapis
  sbi(KBD_PORT_D_col1,KBD_BIT_col1);  
  sbi(KBD_PORT_D_col2,KBD_BIT_col2);
  sbi(KBD_PORT_D_col3,KBD_BIT_col3);
  sbi(KBD_PORT_D_col4,KBD_BIT_col4);
}
