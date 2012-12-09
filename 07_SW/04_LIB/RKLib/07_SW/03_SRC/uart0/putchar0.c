#include "../uart0.h"

int UART0_putchar(char c)    // wysy³a znak c na UART0
{
#ifdef UART0_DE_PORT
//  cbi(_UCR_,RXEN0);			// zablokuj odbior 
  sbi(UART0_DE_PORT,UART0_DE_BIT);	// odblokuj nadajnik RS485
#endif
#ifdef UART0_BUF_SIZE  // je¿eli UART0 ma pracowaæ na przerwaniach

  while((UART0_BUF_SIZE - (UART0_t_in - UART0_t_out)) <= 2);  // Wait...
  // Add data to the transmit buffer, enable TXCIE
  UART0_tbuf[UART0_t_in & UART0_TMASK] = c;
  UART0_t_in++;
  sbi(_UCR_, UDRIE0);      // w³¹cz przerwanie od pustego bufora

#else  // jeœli UART0 pracuje bez u¿ycia przerwañ
  _UDR0_ = c;        // wpisz c do rejestru UDR
  loop_until_bit_is_set(_USR0_,TXC0);  // czekaj na zakoñczenie transmisji
  sbi(_USR0_,TXC0);      // ustaw bit TXC w rej. USR
/*
  loop_until_bit_is_set(_USR0_, UDRE0);
  _UDR0_=c;
*/
#ifdef UART0_DE_PORT
  cbi(UART0_DE_PORT,UART0_DE_BIT);	// zablokuj nadajnik  RS485
//  sbi(_UCR1_,RXEN0);			// odblokuj odbior 
#endif

#endif  //UART0_BUF_SIZE
  return 0;
}


