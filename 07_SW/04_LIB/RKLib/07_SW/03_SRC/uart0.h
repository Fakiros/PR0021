/** \file uart0.h \brief Sterownik interfejsu UART0. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** \defgroup uart Port szeregowy
 
 Obs³uga portu szeregowego UART0.
 
 \code
 #include "uart0.h"
 \endcode 
 
 \ref config:
 
 W pliku \c "config.h" znajduj¹cym siê w katalogu projektu nale¿y umieœciæ nastêpuj¹c¹ zawartoœæ:
    	     
\code
#define UART0_BAUD	19200		// prêdkoœæ transmisji w bit/s
#define UART0_MAX_GETSTR	8		// maksymalna d³ugoœæ tekstu dla UART0_getstr()
#define UART0_BUF_SIZE	8		// wielkoœæ buforów UART0 (musi byæ potêg¹ 2)
#define UART0_DE_PORT	PORTD		// port linii DE konwertera RS485
#define UART0_DE_BIT	7		// bit portu linii DE konwertera RS485
 \endcode
 
 \note Wartoœci z \c #define mo¿na zmieniaæ w celu dostosowania 
 biblioteki do tworzonego urz¹dzenia i oprogramowania.
 
 \note \c UART0_BUF_SIZE automatycznie w³¹cza obs³ugê UART0 na przerwaniach 
 (jeœli nie chcemy u¿ywaæ przerwañ to mo¿na po prostu nie definiowaæ \c UART0_BUF_SIZE)
 
 \note \c UART0_DE_PORT i \c UART0_DE_BIT s¹ opcjonalne - mog¹ siê przydaæ 
 w przypadku realizacji komunikacji na dwuprzewodowym interfejsie 
 RS485 i s³u¿¹ do prze³¹czania kierunku transmisji.

 \par Wykorzystywane przerwania (tylko gdy zdefiniowano UART0_BUF_SIZE):
 
 \c SIG_UART0_RECV, \c SIG_UART0_DATA, \c SIG_UART0_TRANS (gdy zdefiniowano \c UART0_DE_PORT)

\par Przyk³adowy program

\p "config.h"
\include uart0/config.h

\p program g³ówny:
\include uart0/testuart.c
*/

#ifndef __UART0_H__
#define __UART0_H__

/*@{*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>		// obs³uga uk³adu Watchdog
#include "config.h"
#include "global.h"
#include "conv.h"

#ifdef UART0_BUF_SIZE	// je¿eli UART0 ma pracowaæ na przerwaniach

#define UART0_TMASK		(UART0_BUF_SIZE-1)
#define UART0_RMASK		(UART0_BUF_SIZE-1)

#include <avr/signal.h>		// konieczne dla przerwañ
#include <avr/interrupt.h>	// konieczne dla przerwañ

extern volatile uint8_t UART0_tbuf[UART0_BUF_SIZE];  // TX buffer
extern volatile uint8_t UART0_rbuf[UART0_BUF_SIZE];  // RX buffer

extern volatile uint8_t UART0_t_in;            // TX buffer in index
extern volatile uint8_t UART0_t_out;           // TX buffer out index

extern volatile uint8_t UART0_r_in;            // RX buffer in index
extern volatile uint8_t UART0_r_out;           // RX buffer out index

/** 
 Istnieje gdy zdefiniowano \c UART0_BUF_SIZE
 \return iloœæ znaków w buforze odbiorczym.
*/
char UART0_rxlen(void); 

#else        // jeœli UART0 pracuje bez u¿ycia przerwañ

/** 
 Istnieje gdy nie zdefiniowano \c UART0_BUF_SIZE
 \return 0 gdy nie ma znaku w buforze odbiorczym.
*/
#define UART0_rxlen()	bit_is_set(_USR0_,RXC0)

#endif  //UART0_BUF_SIZE


// ---------------------------------------------------------------

/** 
 Inicjalizacja UART0
*/
void UART0_init(void);

/** 
 Wysy³a znak na UART0
 \param c znak (bajt) do wys³ania
*/
int UART0_putchar (char c);

/**
 Czeka i odbiera znak z UART0
 \return odebrany znak z UART0
*/
int UART0_getchar (void);

/**
 Wysy³a ³añcuch z SRAM na UART0
 \param s ³añcuch znaków w pamiêci SRAM
*/
void UART0_putstr (char *s);

/**
 Wysy³a ³añcuch s z FLASH na UART0
 \param s ³añcuch znaków w pamiêci FLASH 
*/
void UART0_putstr_P (const char *s);

/** 
 Wysy³a ³añcuch s z EEPROM na UART0
 \param s ³añcuch znaków w pamiêci EEPROM
*/
void UART0_putstr_E(u08 *s);

/** 
 Pobiera ³añcuch z UART0.
 £añcuch musi byæ zakoñczony znakiem "\n" lub "\r"
 i jego d³ugoœæ nie mo¿e przekroczyæ wartoœci
 zdefiniowanej w UART0_MAX_GETSTR.
 \param s wskaŸnik do tablicy gdzie ma byæ odebrany ³añcuch
*/
void UART0_getstr (char *s);

/** 
 Wysy³a na port szeregowy znakow¹ reprezentacjê liczby o wybranej postawie
 \note korzysta z funkcji \b itoa() co oznacza doœæ du¿e zapotrzebowanie na pamiêæ programu,
 jeœli nie ma specjalnej potrzeby aby j¹ zastosowaæ lepiej skorzystaæ z funkcji:
 UART0_putU08(), UART0_puthexU08(), UART0_puthexU16().
 \param value wartoœæ do wys³ania
 \param radix podstawa np 2, 8, 10, 16
*/
void UART0_putint(int value, u08 radix);

/** 
 Wysy³a na port szeregowy dziesiêtn¹ reprezentacjê liczby 8 bitowej
 \param value wartoœæ do wys³ania
*/
void UART0_putU08(u08 value);

/** 
 Wysy³a na port szeregowy szesnastkow¹ reprezentacjê liczby 8 bitowej
 \param value wartoœæ do wys³ania
*/
void UART0_puthexU08(u08 value);

/** 
 Wysy³a na port szeregowy szesnastkow¹ reprezentacjê liczby 16 bitowej
 \param value wartoœæ do wys³ania
*/
void UART0_puthexU16(u16 value);

/*@}*/

#endif //__UART0_H__
