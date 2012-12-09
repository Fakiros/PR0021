#include "../uart.h"

int UART_getchar (void)  // odbiera znak z UART
{
#ifdef UART_BUF_SIZE    // je¿eli UART ma pracowaæ na przerwaniach

  unsigned char c;
  while(UART_rxlen() == 0) WDR();    // Wait...
  c = UART_rbuf[UART_r_out & UART_RMASK];
  UART_r_out++;  
  return(c);

#else        // jeœli UART pracuje bez u¿ycia przerwañ
  while(bit_is_clear(_USR_,RXC)) WDR();  // czekaj na zakoñczenie odbioru
  cbi(_USR_,RXC);      // skasuj bit RXC w rej. USR
  return (_UDR_);     // zwróæ zawartoœæ rejestru UDR

#endif  //UART_BUF_SIZE
}
