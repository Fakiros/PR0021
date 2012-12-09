/*! \file led7seg.h \brief Multipleksowa obs³uga 7-segmentowych wyœwietlaczy LED. */
/*
  Copyright (c) 2004,5 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** \defgroup led7seg Wyœwietlacz LED
 
 Multipleksowa obs³uga 7-segmentowych wyœwietlaczy LED oraz klawiatury matrycowej.
 
 \code
 #include "led7seg.h"
 \endcode 
 
 \ref config:
 
 W pliku \c "config.h" znajduj¹cym siê w katalogu projektu nale¿y umieœciæ nastêpuj¹c¹ zawartoœæ:
 
 \code
//#define LED7SEG_DIGITS_H	// Odkomentowaæ gdy wybieranie cyfr stanem wysokim
//#define LED7SEG_SEGMENTS_H	// Odkomentowaæ gdy wybieranie segmentów stanem wysokim

// ----- definicje dotycz¹ce cyfr wyœwietlacza -----

#define LED7SEG_DIGIT_SHIFT		// je¿eli wybieranie cyfr przez rejestr przesuwny

    +----------------+
    |  ----------    |   ----------
    | |          |   |  |          |
CLK-*-|>         |   +--|>         |
      |   74164  |      |   74164  |
DATA--|          |   +--|          |   
      |          |   |  |          |
       ----------    |   ----------
        ||||||||-----+    ||||||||
        segmenty           cyfry


#define LED7SEG_DIGIT1_PORT	PORTD	// port cyfry 1
#define LED7SEG_DIGIT1_BIT	6	// bit cyfry 1
#define LED7SEG_DIGIT2_PORT	PORTD	// port cyfry 2
#define LED7SEG_DIGIT2_BIT	5	// bit cyfry 2
#define LED7SEG_DIGIT3_PORT	PORTD	// port cyfry 3
#define LED7SEG_DIGIT3_BIT	4	// bit cyfry 3
#define LED7SEG_DIGIT4_PORT	PORTD	// port cyfry 4
#define LED7SEG_DIGIT4_BIT	3	// bit cyfry 4
#define LED7SEG_DIGIT5_PORT	PORTD	// port cyfry 5
#define LED7SEG_DIGIT5_BIT	2	// bit cyfry 5
#define LED7SEG_DIGIT6_PORT	PORTD	// port cyfry 6
#define LED7SEG_DIGIT6_BIT	1	// bit cyfry 6
#define LED7SEG_DIGIT7_PORT	PORTD	// port cyfry 7
#define LED7SEG_DIGIT7_BIT	0	// bit cyfry 7
#define LED7SEG_DIGIT8_PORT	PORTD	// port cyfry 8
#define LED7SEG_DIGIT8_BIT	7	// bit cyfry 8

// ----- definicje dotycz¹ce segmentów wyœwietlacza -----

#define LED7SEG_SEGMENTS	PORTB	// port segmentów wyswietlacza

// je¿eli nie jest zdefiniowane LED7SEG_SEGMENTS; dane przeznaczone
// do sterowania segmentami s¹ kierowane na rejestr przesuwny
       ---------- 
      |          |
CLK---|>         |
      |   74164  |
DATA--|          |
      |          |
       ----------
        ||||||||
        segmenty

#define LED7SEG_DATA_PORT	PORTB	// port danych rejestru przesuwnego
#define LED7SEG_DATA_BIT	6	// bit danych rejestru przesuwnego

#define LED7SEG_CLK_PORT	PORTB	// port zegara rejestru przesuwnego
#define LED7SEG_CLK_BIT		5	// bit zegara rejestru przesuwnego

				// bity segmentów na wyœwietlaczu:
#define LED7SEG_A		1	// bit segmentu A
#define LED7SEG_B		2	// bit segmentu B
#define LED7SEG_C		6	// bit segmentu C
#define LED7SEG_D		4	// bit segmentu D
#define LED7SEG_E		3	// bit segmentu E
#define LED7SEG_F		0	// bit segmentu F
#define LED7SEG_G		5	// bit segmentu G
#define LED7SEG_H		7	// bit segmentu H (kropki)

// dodatkowo ewentualna obs³uga klawiatury
#define LED7SEG_KBD_ROW1_PORT	PORTB	// port odczytu 1 wiersza klawiatury
#define LED7SEG_KBD_ROW1_BIT	0	// bit portu odczytu 1 wiersza klawiatury

#define LED7SEG_F_MUX		400	// czêstotliwoœæ multipleksowania w Hz

\endcode
 
 \note Wartoœci z \c #define mo¿na zmieniaæ w celu dostosowania 
 biblioteki do tworzonego urz¹dzenia i oprogramowania.
 
 \par Wykorzytywane przerwania:
 \c SIG_OVERFLOW0
*/

#ifndef __LED7SEG_H__
#define __LED7SEG_H__

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "config.h"
#include "global.h"

/*@{*/

#define LED7SEG_DATA_HI()	sbi(LED7SEG_DATA_PORT,LED7SEG_DATA_BIT)	///< linia danych rejestru przesuwnego
#define LED7SEG_DATA_LO()	cbi(LED7SEG_DATA_PORT,LED7SEG_DATA_BIT)	///< linia danych rejestru przesuwnego
#define LED7SEG_CLK_HI()	sbi(LED7SEG_CLK_PORT,LED7SEG_CLK_BIT)	///< linia zegara rejestru przesuwnego
#define LED7SEG_CLK_LO()	sbi(LED7SEG_CLK_PORT,LED7SEG_CLK_BIT)	///< linia zegara rejestru przesuwnego

#ifdef LED7SEG_DIGITS_H		///< Wybieranie cyfr stanem wysokim

#define LED7SEG_DIGIT1_ON()	sbi(LED7SEG_DIGIT1_PORT,LED7SEG_DIGIT1_BIT)	///< cyfra 1 - zapalona
#define LED7SEG_DIGIT1_OFF()	cbi(LED7SEG_DIGIT1_PORT,LED7SEG_DIGIT1_BIT)	///< cyfra 1 - zgaszona
#define LED7SEG_DIGIT2_ON()	sbi(LED7SEG_DIGIT2_PORT,LED7SEG_DIGIT2_BIT)	///< cyfra 2 - zapalona
#define LED7SEG_DIGIT2_OFF()	cbi(LED7SEG_DIGIT2_PORT,LED7SEG_DIGIT2_BIT)	///< cyfra 2 - zgaszona
#define LED7SEG_DIGIT3_ON()	sbi(LED7SEG_DIGIT3_PORT,LED7SEG_DIGIT3_BIT)	///< cyfra 3 - zapalona
#define LED7SEG_DIGIT3_OFF()	cbi(LED7SEG_DIGIT3_PORT,LED7SEG_DIGIT3_BIT)	///< cyfra 3 - zgaszona
#define LED7SEG_DIGIT4_ON()	sbi(LED7SEG_DIGIT4_PORT,LED7SEG_DIGIT4_BIT)	///< cyfra 4 - zapalona
#define LED7SEG_DIGIT4_OFF()	cbi(LED7SEG_DIGIT4_PORT,LED7SEG_DIGIT4_BIT)	///< cyfra 4 - zgaszona
#define LED7SEG_DIGIT5_ON()	sbi(LED7SEG_DIGIT5_PORT,LED7SEG_DIGIT5_BIT)	///< cyfra 5 - zapalona
#define LED7SEG_DIGIT5_OFF()	cbi(LED7SEG_DIGIT5_PORT,LED7SEG_DIGIT5_BIT)	///< cyfra 5 - zgaszona
#define LED7SEG_DIGIT6_ON()	sbi(LED7SEG_DIGIT6_PORT,LED7SEG_DIGIT6_BIT)	///< cyfra 6 - zapalona
#define LED7SEG_DIGIT6_OFF()	cbi(LED7SEG_DIGIT6_PORT,LED7SEG_DIGIT6_BIT)	///< cyfra 6 - zgaszona
#define LED7SEG_DIGIT7_ON()	sbi(LED7SEG_DIGIT7_PORT,LED7SEG_DIGIT7_BIT)	///< cyfra 7 - zapalona
#define LED7SEG_DIGIT7_OFF()	cbi(LED7SEG_DIGIT7_PORT,LED7SEG_DIGIT7_BIT)	///< cyfra 7 - zgaszona
#define LED7SEG_DIGIT8_ON()	sbi(LED7SEG_DIGIT8_PORT,LED7SEG_DIGIT8_BIT)	///< cyfra 8 - zapalona
#define LED7SEG_DIGIT8_OFF()	cbi(LED7SEG_DIGIT8_PORT,LED7SEG_DIGIT8_BIT)	///< cyfra 8 - zgaszona
#define LED7SEG_OFF		0x00						///< wygaszenie wyœwietlacza

#else				///< wspólna anoda - wybieranie cyfr stanem niskim

#define LED7SEG_DIGIT1_ON()	cbi(LED7SEG_DIGIT1_PORT,LED7SEG_DIGIT1_BIT)	///< cyfra 1 - zapalona
#define LED7SEG_DIGIT1_OFF()	sbi(LED7SEG_DIGIT1_PORT,LED7SEG_DIGIT1_BIT)	///< cyfra 1 - zgaszona
#define LED7SEG_DIGIT2_ON()	cbi(LED7SEG_DIGIT2_PORT,LED7SEG_DIGIT2_BIT)	///< cyfra 2 - zapalona
#define LED7SEG_DIGIT2_OFF()	sbi(LED7SEG_DIGIT2_PORT,LED7SEG_DIGIT2_BIT)	///< cyfra 2 - zgaszona
#define LED7SEG_DIGIT3_ON()	cbi(LED7SEG_DIGIT3_PORT,LED7SEG_DIGIT3_BIT)	///< cyfra 3 - zapalona
#define LED7SEG_DIGIT3_OFF()	sbi(LED7SEG_DIGIT3_PORT,LED7SEG_DIGIT3_BIT)	///< cyfra 3 - zgaszona
#define LED7SEG_DIGIT4_ON()	cbi(LED7SEG_DIGIT4_PORT,LED7SEG_DIGIT4_BIT)	///< cyfra 4 - zapalona
#define LED7SEG_DIGIT4_OFF()	sbi(LED7SEG_DIGIT4_PORT,LED7SEG_DIGIT4_BIT)	///< cyfra 4 - zgaszona
#define LED7SEG_DIGIT5_ON()	cbi(LED7SEG_DIGIT5_PORT,LED7SEG_DIGIT5_BIT)	///< cyfra 5 - zapalona
#define LED7SEG_DIGIT5_OFF()	sbi(LED7SEG_DIGIT5_PORT,LED7SEG_DIGIT5_BIT)	///< cyfra 5 - zgaszona
#define LED7SEG_DIGIT6_ON()	cbi(LED7SEG_DIGIT6_PORT,LED7SEG_DIGIT6_BIT)	///< cyfra 6 - zapalona
#define LED7SEG_DIGIT6_OFF()	sbi(LED7SEG_DIGIT6_PORT,LED7SEG_DIGIT6_BIT)	///< cyfra 6 - zgaszona
#define LED7SEG_DIGIT7_ON()	cbi(LED7SEG_DIGIT7_PORT,LED7SEG_DIGIT7_BIT)	///< cyfra 7 - zapalona
#define LED7SEG_DIGIT7_OFF()	sbi(LED7SEG_DIGIT7_PORT,LED7SEG_DIGIT7_BIT)	///< cyfra 7 - zgaszona
#define LED7SEG_DIGIT8_ON()	cbi(LED7SEG_DIGIT8_PORT,LED7SEG_DIGIT8_BIT)	///< cyfra 8 - zapalona
#define LED7SEG_DIGIT8_OFF()	sbi(LED7SEG_DIGIT8_PORT,LED7SEG_DIGIT8_BIT)	///< cyfra 8 - zgaszona
#define LED7SEG_OFF		0xff						///< wygaszenie wyœwietlacza
#endif	

#define LED7SEG_BLANK		46	///< Kod znaku wygaszonego wyswietlacza
#define LED7SEG_AF		10	///< Kod znaku ; (|~ segmenty: af)
#define LED7SEG_AB		11	///< Kod znaku : (~| segmenty: ab)
#define LED7SEG_BG		27	///< Kod znaku K (_| segmenty: bg)
#define LED7SEG_FG		29	///< Kod znaku M (|_ segmenty: fg)
#define LED7SEG_BFG		44	///< Kod znaku M (|_| segmenty: bfg)
#define LED7SEG_ABFG		39	///< Kod znaku W (o  segmenty: abfg)

/// Funkcja u¿ytkownika obs³ugiwana przez przerwanie od T/C0
#ifndef LED7SEG_SIG_OVERFLOW0
#define LED7SEG_SIG_OVERFLOW0		// Przypisz pusty ci¹g
#endif

#if   defined(LED7SEG_DIGIT8_PORT)
#define LED7SEG_DIGITS		8	///< Liczba cyfr na wyswietlaczu
#elif defined(LED7SEG_DIGIT7_PORT)
#define LED7SEG_DIGITS		7	///< Liczba cyfr na wyswietlaczu
#elif defined(LED7SEG_DIGIT6_PORT)
#define LED7SEG_DIGITS		6	///< Liczba cyfr na wyswietlaczu
#elif defined(LED7SEG_DIGIT5_PORT)
#define LED7SEG_DIGITS		5	///< Liczba cyfr na wyswietlaczu
#elif defined(LED7SEG_DIGIT4_PORT)
#define LED7SEG_DIGITS		4	///< Liczba cyfr na wyswietlaczu
#elif defined(LED7SEG_DIGIT3_PORT)
#define LED7SEG_DIGITS		3	///< Liczba cyfr na wyswietlaczu
#elif defined(LED7SEG_DIGIT2_PORT)
#define LED7SEG_DIGITS		2	///< Liczba cyfr na wyswietlaczu
#elif defined(LED7SEG_DIGIT1_PORT)
#define LED7SEG_DIGITS		1	///< Liczba cyfr na wyswietlaczu
#elif !defined(LED7SEG_DIGIT_SHIFT)
#define LED7SEG_DIGITS		8	///< Liczba cyfr na wyswietlaczu
#define LED7SEG_DIGIT_SHIFT		///< Stosuj rejestr przesuwny
#endif

#ifndef LED7SEG_F_MUX
#define LED7SEG_F_MUX		400
#endif

#define LED7SEG_TIMER_PRE1	(F_CPU/LED7SEG_F_MUX)/256

#if (LED7SEG_TIMER_PRE1<1)
#define LED7SEG_TIMER_PRE	1
#elif (LED7SEG_TIMER_PRE1<8)
#define LED7SEG_TIMER_PRE	8
#elif (LED7SEG_TIMER_PRE1<64)
#define LED7SEG_TIMER_PRE	64
#elif (LED7SEG_TIMER_PRE1<256)
#define LED7SEG_TIMER_PRE	256
#elif (LED7SEG_TIMER_PRE1<1024)
#define LED7SEG_TIMER_PRE	1024
#endif

#define LED7SEG_TIMER_INIT	(256-((F_CPU/LED7SEG_TIMER_PRE)/LED7SEG_F_MUX))

#if (LED7SEG_TIMER_PRE==1)
#define LED7SEG_TIMER_CS	_BV(CS00)
#elif (LED7SEG_TIMER_PRE==8)
#define LED7SEG_TIMER_CS	_BV(CS01)
#elif (LED7SEG_TIMER_PRE==64)
#define LED7SEG_TIMER_CS	_BV(CS00)|_BV(CS01)
#elif (LED7SEG_TIMER_PRE==256)
#define LED7SEG_TIMER_CS	_BV(CS02)
#elif (LED7SEG_TIMER_PRE==1024)
#define LED7SEG_TIMER_CS	_BV(CS00)|_BV(CS02)
#endif

/*--------------------------------------------------------------------- */
/*			Deklaracje tablic i zmiennych			*/
/*--------------------------------------------------------------------- */
/**
 Tablica kszta³tów znaków zdefiniowana w pliku ./led7seg/chars.c
*/
extern prog_char LED7SEG_CHARS[];

/**
 Numer aktualnie wyswietlanej cyfry zmieniany w procedurze obs³ugi przerwania
*/
extern u08 LED7SEG_position;

/**
 Maska bitowa cyfr wyswietlacza z zapalonym przecinkiem
*/
u08 LED7SEG_commas;

/**
 Tablica cyfr do wyswietlenia
*/
extern u08 LED7SEG_digits[LED7SEG_DIGITS];

/**
 Numer ostatnio naciœniêtego przycisku
*/
extern u08 LED7SEG_KBD_key;

/**
 Czasomierz klawiatury
*/
extern volatile u08 LED7SEG_KBD_key_timer;
extern u08 LED7SEG_KBD_key_timer_old;   	// ostanio czytany czasomierz klawiatury

/**
 Wysy³a dane na rejestr przesuwny
 \param kod kod 7 segmentowy
*/
void LED7SEG_out(char kod);

/**
 Inicjalizacja timera 0 potrzebnego do prze³¹czania cyfr
 W celu uruchomienia prze³¹czania nale¿y u¿yæ po tym instrucji sei()
 */
void LED7SEG_init(void);

/**
 Wygasza wyswietlacz
*/
void LED7SEG_clear(void);

/**
 Wyswietla tekst na wyswietlaczu
 \param s tekst (cyfry oraz du¿e litery: kody ASCII od 48 do 95)
*/
void LED7SEG_putstr(char* s);

/**
 Wyswietla 8 bitow¹ liczbe na wyswietlaczu
 \param bin liczba do wyœwietlenia
*/
void LED7SEG_putU08(u08 bin);

/**
 Wyswietla 16 bitow¹ liczbe na wyswietlaczu
 \param bin liczba do wyœwietlenia
*/
void LED7SEG_putU16(u16 bin);

/**
 Wyswietla 32 bitow¹ liczbe na wyswietlaczu
 \param bin liczba do wyœwietlenia
*/
void LED7SEG_putU32(u32 bin);

/**
 Przesuwa zawartoœæ wyœwietlacza o jedn¹ pozycjê w lewo.
*/
void LED7SEG_shift_left(void);

/**
 Przesuwa zawartoœæ wyœwietlacza o jedn¹ pozycjê w prawo.
*/
void LED7SEG_shift_right(void);

/**
 Zwraca kod klawisza po jego zwolnieniu. Zeruje zmienn¹ LED7SEG_KBD_key.
 \note Wymagane jest kilkumilisekundowe opóŸnienie pomiêdzy
 kolejnymi wywo³aniami tej funkcji (wa¿ne gdy pracuje w pêtli).
 Korzysta z LED7SEG_KBD_key_pressed.
 \return kod (numer) naciœniêtego klawisza lub zero jeœli ci¹gle naciœniêty.
*/
u08 LED7SEG_KBD_key_read(void);

/**
 Sprawdza czy klawisz jest naciœniêty.
 \note Wymagane jest kilkumilisekundowe opóŸnienie pomiêdzy
 kolejnymi wywo³aniami tej funkcji (wa¿ne gdy pracuje w pêtli).
 \return 1 jeœli klawisz naciœniêty, 0 w przeciwnym wypadku.
*/
u08 LED7SEG_KBD_key_pressed(void);

/*@}*/

#endif
