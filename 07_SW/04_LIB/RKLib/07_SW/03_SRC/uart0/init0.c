#include "../uart0.h"

#ifdef UART0_BUF_SIZE  // je¿eli UART0 ma pracowaæ na przerwaniach

// Static variables
//
volatile uint8_t UART0_tbuf[UART0_BUF_SIZE];  // TX buffer
volatile uint8_t UART0_rbuf[UART0_BUF_SIZE];  // RX buffer

volatile uint8_t UART0_t_in;            // TX buffer in index
volatile uint8_t UART0_t_out;           // TX buffer out index

volatile uint8_t UART0_r_in;            // RX buffer in index
volatile uint8_t UART0_r_out;           // RX buffer out index
#endif  //UART0_BUF_SIZE

void UART0_init(void)  // inicjalizacja UART0
{
#ifdef UART0_DE_PORT
  sbi(DDR(UART0_DE_PORT),UART0_DE_BIT);	// ustaw kierunek linii DE konwertera RS485
#endif

  _UBRR_ = (u08)UART_CONST;     // ustaw prêdkoœæ transmisji

#ifdef _UBRR0H_
  _UBRR0H_ = UART0_CONST >> 8;    // ustaw prêdkoœæ transmisji
#endif

#ifdef UART0_BUF_SIZE  // je¿eli UART0 ma pracowaæ na przerwaniach

#ifdef UART0_DE_PORT
  _UCR_ = _BV(RXEN0)|_BV(TXEN0)|_BV(RXCIE0)|_BV(TXCIE0);
#else
  _UCR_ = _BV(RXEN0)|_BV(TXEN0)|_BV(RXCIE0);
#endif
        // w³¹cz odbiór, nadawanie i przerwania
  sei();    // w³¹cz przerwania

#else  // jeœli UART0 pracuje bez u¿ycia przerwañ

  _UCR_ = _BV(RXEN0)|_BV(TXEN0);  // w³¹cz odbiór i nadawanie

#endif  //UART0_BUF_SIZE

}

#ifdef UART0_BUF_SIZE    // je¿eli UART0 ma pracowaæ na przerwaniach
//------------------------------------------------------------------------

SIGNAL(SIG_USART0_RECV)
{
  char c;
  c = _UDR_;              // Get received char
  UART0_rbuf[UART0_r_in & UART0_RMASK] = c;
  UART0_r_in++;
}

SIGNAL(SIG_USART0_DATA)
{
  if(UART0_t_in != UART0_t_out)
  {
    _UDR_ = UART0_tbuf[UART0_t_out & UART0_TMASK];
    UART0_t_out++;
  }
  else
  {
    _UCR_ &= ~(1<<UDRIE0);
  }
}

#ifdef UART0_DE_PORT
SIGNAL(SIG_USART0_TRANS)
{
  cbi(UART0_DE_PORT,UART0_DE_BIT);	// zablokuj nadajnik RS485
}
#endif

//---------------------------------------------------------------------------
#endif  //UART0_BUF_SIZE




