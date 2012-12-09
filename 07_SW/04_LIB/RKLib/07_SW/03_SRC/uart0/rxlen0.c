#include "../uart0.h"

#ifdef UART0_BUF_SIZE  // je¿eli UART ma pracowaæ na przerwaniach
char UART0_rxlen(void) 
{
  return(UART0_r_in - UART0_r_out);
}
#endif  //UART0_BUF_SIZE
