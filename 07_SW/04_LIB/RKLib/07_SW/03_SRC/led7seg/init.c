#include "../led7seg.h"
#ifdef LED7SEG_SPI
#include "../spi.h"
#endif

u08 LED7SEG_commas;			// maska bitowa przecinków na wyœwietlaczu
u08 LED7SEG_position;           	// numer wyswietlanej cyfry
u08 LED7SEG_digits[LED7SEG_DIGITS];     // tablica cyfr do wyswietlenia

#ifdef LED7SEG_KBD_ROW1_PORT		// jeœli bêdzie obs³uga klawiatury
u08 LED7SEG_KBD_key;	           	// numer ostatnio nacisnietego przycisku
volatile u08 LED7SEG_KBD_key_timer;   	// czasomierz klawiatury
u08 LED7SEG_KBD_key_timer_old;   	// ostanio czytany czasomierz klawiatury

void LED7SEG_KBD_row_scan(u08 row)	// rz¹d od 0 wzwy¿ (o 1 mniejszy ni¿ w deklaracji)
{
  LED7SEG_KBD_key_timer++;	// inkrementuj czas dla klawiszy
  LED7SEG_KBD_key=1+LED7SEG_position+LED7SEG_DIGITS*row; // naciœniêto klawisz: wpisz kod
}

void LED7SEG_KBD_rows_scan(void)	// skanuj rzêdy klawiatury
{
  if (bit_is_clear(PIN(LED7SEG_KBD_ROW1_PORT),LED7SEG_KBD_ROW1_BIT))
    LED7SEG_KBD_row_scan(0);
#ifdef LED7SEG_KBD_ROW2_PORT		// jeœli bêdzie obs³uga 2 rzêdu klawiszy
  else if (bit_is_clear(PIN(LED7SEG_KBD_ROW2_PORT),LED7SEG_KBD_ROW2_BIT))
    LED7SEG_KBD_row_scan(1);
#endif
#ifdef LED7SEG_KBD_ROW2_PORT		// jeœli bêdzie obs³uga 3 rzêdu klawiszy
  else if (bit_is_clear(PIN(LED7SEG_KBD_ROW3_PORT),LED7SEG_KBD_ROW3_BIT))
    LED7SEG_KBD_row_scan(2);
#endif
#ifdef LED7SEG_KBD_ROW2_PORT		// jeœli bêdzie obs³uga 4 rzêdu klawiszy
  else if (bit_is_clear(PIN(LED7SEG_KBD_ROW4_PORT),LED7SEG_KBD_ROW4_BIT))
    LED7SEG_KBD_row_scan(3);
#endif
}
#endif

void LED7SEG_init(void)
{
#ifdef LED7SEG_SEGMENTS  		// je¿eli zdefiniowano SEGMENTS
  DDR(LED7SEG_SEGMENTS)=0xFF;		// ustaw port segmentów na wyjœcie
#endif
#if !defined (LED7SEG_SEGMENTS) | defined(LED7SEG_DIGIT_SHIFT)
  sbi(DDR(LED7SEG_DATA_PORT),LED7SEG_DATA_BIT); // ustaw jako wyjœcie dwa bity
  sbi(DDR(LED7SEG_CLK_PORT),LED7SEG_CLK_BIT);	// dla rejestru przesuwnego
#endif
#ifdef LED7SEG_KBD_ROW1_PORT
  sbi(LED7SEG_KBD_ROW1_PORT,LED7SEG_KBD_ROW1_BIT); // podci¹gnij wejœcie 1 rzêdu klawiatury
#endif
#ifdef LED7SEG_KBD_ROW2_PORT
  sbi(LED7SEG_KBD_ROW2_PORT,LED7SEG_KBD_ROW2_BIT); // podci¹gnij wejœcie 2 rzêdu klawiatury
#endif
#ifdef LED7SEG_KBD_ROW3_PORT
  sbi(LED7SEG_KBD_ROW3_PORT,LED7SEG_KBD_ROW3_BIT); // podci¹gnij wejœcie 3 rzêdu klawiatury
#endif
#ifdef LED7SEG_KBD_ROW4_PORT
  sbi(LED7SEG_KBD_ROW4_PORT,LED7SEG_KBD_ROW4_BIT); // podci¹gnij wejœcie 4 rzêdu klawiatury
#endif
#ifndef LED7SEG_DIGIT_SHIFT
#if (LED7SEG_DIGITS>=1)
  sbi(DDR(LED7SEG_DIGIT1_PORT),LED7SEG_DIGIT1_BIT); // bit cyfry 1 jako wyjœcie
#endif
#if (LED7SEG_DIGITS>=2)
  sbi(DDR(LED7SEG_DIGIT2_PORT),LED7SEG_DIGIT2_BIT); // bit cyfry 2 jako wyjœcie
#endif
#if (LED7SEG_DIGITS>=3)
  sbi(DDR(LED7SEG_DIGIT3_PORT),LED7SEG_DIGIT3_BIT); // bit cyfry 3 jako wyjœcie
#endif
#if (LED7SEG_DIGITS>=4)
  sbi(DDR(LED7SEG_DIGIT4_PORT),LED7SEG_DIGIT4_BIT); // bit cyfry 4 jako wyjœcie
#endif
#if (LED7SEG_DIGITS>=5)
  sbi(DDR(LED7SEG_DIGIT5_PORT),LED7SEG_DIGIT5_BIT); // bit cyfry 5 jako wyjœcie
#endif
#if (LED7SEG_DIGITS>=6)
  sbi(DDR(LED7SEG_DIGIT6_PORT),LED7SEG_DIGIT6_BIT); // bit cyfry 6 jako wyjœcie
#endif
#if (LED7SEG_DIGITS>=7)
  sbi(DDR(LED7SEG_DIGIT7_PORT),LED7SEG_DIGIT7_BIT); // bit cyfry 7 jako wyjœcie
#endif
#if (LED7SEG_DIGITS>=8)
  sbi(DDR(LED7SEG_DIGIT8_PORT),LED7SEG_DIGIT8_BIT); // bit cyfry 8 jako wyjœcie
#endif
#endif // LED7SEG_DIGIT_SHIFT
  TCCR0=LED7SEG_TIMER_CS;   	// Timer/Counter 0 taktowany z odpowiedniego preskalera
  TCNT0=LED7SEG_TIMER_INIT;	// Za³adowanie odpowiedniej wartoœci
  TIMSK=_BV(TOIE0);     	// Uaktywnienie przerwania z T0
#ifdef LED7SEG_SPI
  SPI_init();
#endif
}

#ifdef LED7SEG_SEGMENTS
#define LED7SEG_SEGMENTS_out(kod) LED7SEG_SEGMENTS=kod
#else
#define LED7SEG_SEGMENTS_out(kod) LED7SEG_out(kod)
#endif

SIGNAL(SIG_OVERFLOW0)		// przewanie od przepe³nienia TIMERA 0
{
  u08 tmp;
///////////////////////////////////////////////////////////////////////////////
// WYSWIETLANIE MULTIPLEKSOWANE
///////////////////////////////////////////////////////////////////////////////
  TCNT0=LED7SEG_TIMER_INIT;
#ifdef LED7SEG_DIGIT_SHIFT
#ifdef LED7SEG_DIGIT_SEL_H	// wybieranie cyfr stanem wysokim
  LED7SEG_out(_BV(LED7SEG_position));		// wys³anie cyfry do prze³¹czenia
#else				// wybieranie cyfr stanem niskim
  LED7SEG_out(~_BV(LED7SEG_position));		// wys³anie cyfry do prze³¹czenia
#endif // LED7SEG_DIGIT_SEL_H
#endif // LED7SEG_DIGIT_SHIFT
#ifndef LED7SEG_DIGIT_SHIFT
  LED7SEG_SEGMENTS_out(LED7SEG_OFF);	// eliminacja przeswitow miedzy cyframi
  switch (LED7SEG_position)		// przelaczanie cyfr
  {
#if (LED7SEG_DIGITS>=1)
    case 0:			// pozycja 0
    {
#if (LED7SEG_DIGITS==2)
      LED7SEG_DIGIT2_OFF();	// wy³¹cz cyfrê 2
#endif
#if (LED7SEG_DIGITS==3)
      LED7SEG_DIGIT3_OFF();	// wy³¹cz cyfrê 3
#endif
#if (LED7SEG_DIGITS==4)
      LED7SEG_DIGIT4_OFF();	// wy³¹cz cyfrê 4
#endif
#if (LED7SEG_DIGITS==5)
      LED7SEG_DIGIT5_OFF();	// wy³¹cz cyfrê 5
#endif
#if (LED7SEG_DIGITS==6)
      LED7SEG_DIGIT6_OFF();	// wy³¹cz cyfrê 6
#endif
#if (LED7SEG_DIGITS==7)
      LED7SEG_DIGIT7_OFF();	// wy³¹cz cyfrê 7
#endif
#if (LED7SEG_DIGITS==8)
      LED7SEG_DIGIT8_OFF();	// wy³¹cz cyfrê 8
#endif
      LED7SEG_DIGIT1_ON(); 	// za³¹cz cyfrê 1
#ifdef LED7SEG_KBD_ROW1_PORT
      LED7SEG_KBD_rows_scan();  // skanuj rzêdy klawiatury
#endif
    } break;
#endif
#if (LED7SEG_DIGITS>=2)
    case 1:			// pozycja 1
    {
      LED7SEG_DIGIT1_OFF();	// wy³¹cz cyfrê 1
      LED7SEG_DIGIT2_ON(); 	// za³¹cz cyfrê 2
#ifdef LED7SEG_KBD_ROW1_PORT
      LED7SEG_KBD_rows_scan();  // skanuj rzêdy klawiatury
#endif
    } break;
#endif
#if (LED7SEG_DIGITS>=3)
    case 2:			// pozycja 2
    {
      LED7SEG_DIGIT2_OFF();	// wy³¹cz cyfrê 2
      LED7SEG_DIGIT3_ON(); 	// za³¹cz cyfrê 3
#ifdef LED7SEG_KBD_ROW1_PORT
      LED7SEG_KBD_rows_scan();  // skanuj rzêdy klawiatury
#endif
    } break;
#endif
#if (LED7SEG_DIGITS>=4)
    case 3:			// pozycja 3
    {
      LED7SEG_DIGIT3_OFF();	// wy³¹cz cyfrê 3
      LED7SEG_DIGIT4_ON(); 	// za³¹cz cyfrê 4
#ifdef LED7SEG_KBD_ROW1_PORT
      LED7SEG_KBD_rows_scan();  // skanuj rzêdy klawiatury
#endif
    } break;
#endif
#if (LED7SEG_DIGITS>=5)
    case 4:			// pozycja 4
    {
      LED7SEG_DIGIT4_OFF();	// wy³¹cz cyfrê 4
      LED7SEG_DIGIT5_ON(); 	// za³¹cz cyfrê 5
#ifdef LED7SEG_KBD_ROW1_PORT
      LED7SEG_KBD_rows_scan();  // skanuj rzêdy klawiatury
#endif
    } break;
#endif
#if (LED7SEG_DIGITS>=6)
    case 5:			// pozycja 5
    {
      LED7SEG_DIGIT5_OFF();	// wy³¹cz cyfrê 5
      LED7SEG_DIGIT6_ON(); 	// za³¹cz cyfrê 6
#ifdef LED7SEG_KBD_ROW1_PORT
      LED7SEG_KBD_rows_scan();  // skanuj rzêdy klawiatury
#endif
    } break;
#endif
#if LED7SEG_DIGITS>=7
    case 6:			// pozycja 6
    {
      LED7SEG_DIGIT6_OFF();	// wy³¹cz cyfrê 6
      LED7SEG_DIGIT7_ON(); 	// za³¹cz cyfrê 7
#ifdef LED7SEG_KBD_ROW1_PORT
      LED7SEG_KBD_rows_scan();  // skanuj rzêdy klawiatury
#endif
    } break;
#endif
#if (LED7SEG_DIGITS>=8)
    case 7:			// pozycja 7
    {
      LED7SEG_DIGIT7_OFF();	// wy³¹cz cyfrê 7
      LED7SEG_DIGIT8_ON(); 	// za³¹cz cyfrê 8
#ifdef LED7SEG_KBD_ROW1_PORT
      LED7SEG_KBD_rows_scan();  // skanuj rzêdy klawiatury
#endif
    } break;
#endif
  }
#endif // LED7SEG_DIGIT_SHIFT
  tmp=pgm_read_byte(&LED7SEG_CHARS[LED7SEG_digits[LED7SEG_position]]);
  if (_BV(LED7SEG_position)&(LED7SEG_commas))	
    tmp=tmp^_BV(LED7SEG_H);	// zapal przecinek na odpowiednich pozycjach
  LED7SEG_SEGMENTS_out(tmp);	// kod cyfry na segmenty
  if (++LED7SEG_position>=LED7SEG_DIGITS) LED7SEG_position=0;		// nastepna cyfra
  LED7SEG_SIG_OVERFLOW0;	// funkcja u¿ytkownika obs³ugiwana przez przerwanie od T/C0
}

