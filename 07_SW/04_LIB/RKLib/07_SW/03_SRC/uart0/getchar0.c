#include "../uart0.h"

int UART0_getchar (void)  // odbiera znak z UART
{
#ifdef UART0_BUF_SIZE    // je¿eli UART0 ma pracowaæ na przerwaniach

  unsigned char c;
  while(UART0_rxlen() == 0) WDR();    // Wait...
  c = UART0_rbuf[UART0_r_out & UART0_RMASK];
  UART0_r_out++;  
  return(c);

#else        // jeœli UART0 pracuje bez u¿ycia przerwañ
  while(bit_is_clear(_USR0_,RXC0)) WDR();  // czekaj na zakoñczenie odbioru
  cbi(_UcSR0_,RXC0);      // skasuj bit RXC0 w rej. USR
  return (_UDR0_);     // zwróæ zawartoœæ rejestru UDR

#endif  //UART0_BUF_SIZE
}
