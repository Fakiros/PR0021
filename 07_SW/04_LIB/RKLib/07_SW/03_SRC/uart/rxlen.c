#include "../uart.h"

#ifdef UART_BUF_SIZE  // je¿eli UART ma pracowaæ na przerwaniach
char UART_rxlen(void) 
{
  return(UART_r_in - UART_r_out);
}
#endif  //UART_BUF_SIZE
