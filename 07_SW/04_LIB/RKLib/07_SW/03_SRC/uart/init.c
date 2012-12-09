#include "../uart.h"

#ifdef UART_BUF_SIZE  // je¿eli UART ma pracowaæ na przerwaniach

// Static variables
//
volatile uint8_t UART_tbuf[UART_BUF_SIZE];  // TX buffer
volatile uint8_t UART_rbuf[UART_BUF_SIZE];  // RX buffer

volatile uint8_t UART_t_in;            // TX buffer in index
volatile uint8_t UART_t_out;           // TX buffer out index

volatile uint8_t UART_r_in;            // RX buffer in index
volatile uint8_t UART_r_out;           // RX buffer out index
#endif  //UART_BUF_SIZE

void UART_init(void)  // inicjalizacja UART
{
#ifdef UART_DE_PORT
  sbi(DDR(UART_DE_PORT),UART_DE_BIT);	// ustaw kierunek linii DE konwertera RS485
#endif

  _UBRR_ = (u08)UART_CONST;     // ustaw prêdkoœæ transmisji

#ifdef _UBRRH_
  _UBRRH_ = UART_CONST >> 8;    // ustaw prêdkoœæ transmisji
#endif

#ifdef UART_BUF_SIZE  // je¿eli UART ma pracowaæ na przerwaniach

#ifdef UART_DE_PORT
  _UCR_ = _BV(RXEN)|_BV(TXEN)|_BV(RXCIE)|_BV(TXCIE);
#else
  _UCR_ = _BV(RXEN)|_BV(TXEN)|_BV(RXCIE);
#endif
        // w³¹cz odbiór, nadawanie i przerwania
  sei();    // w³¹cz przerwania

#else  // jeœli UART pracuje bez u¿ycia przerwañ

  _UCR_ = _BV(RXEN)|_BV(TXEN);  // w³¹cz odbiór i nadawanie

#endif  //UART_BUF_SIZE

}

#ifdef UART_BUF_SIZE    // je¿eli UART ma pracowaæ na przerwaniach
//------------------------------------------------------------------------

SIGNAL(SIG_UART_RECV)
{
  char c;
  c = _UDR_;              // Get received char
  UART_rbuf[UART_r_in & UART_RMASK] = c;
  UART_r_in++;
}

SIGNAL(SIG_UART_DATA)
{
  if(UART_t_in != UART_t_out)
  {
    _UDR_ = UART_tbuf[UART_t_out & UART_TMASK];
    UART_t_out++;
  }
  else
  {
    _UCR_ &= ~(1<<UDRIE);
  }
}

#ifdef UART_DE_PORT
SIGNAL(SIG_UART_TRANS)
{
  cbi(UART_DE_PORT,UART_DE_BIT);	// zablokuj nadajnik RS485
}
#endif

//---------------------------------------------------------------------------
#endif  //UART_BUF_SIZE




