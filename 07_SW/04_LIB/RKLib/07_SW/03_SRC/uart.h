/** \file uart.h \brief Sterownik interfejsu UART. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** \defgroup uart Port szeregowy
 
 Obs³uga portu szeregowego UART0.
 
 \code
 #include "uart.h"
 \endcode 
 
 \ref config:
 
 W pliku \c "config.h" znajduj¹cym siê w katalogu projektu nale¿y umieœciæ nastêpuj¹c¹ zawartoœæ:
    	     
\code
#define UART_BAUD	19200		// prêdkoœæ transmisji w bit/s
#define UART_MAX_GETSTR	8		// maksymalna d³ugoœæ tekstu dla UART_getstr()
#define UART_BUF_SIZE	8		// wielkoœæ buforów UART (musi byæ potêg¹ 2)
#define UART_DE_PORT	PORTD		// port linii DE konwertera RS485
#define UART_DE_BIT	7		// bit portu linii DE konwertera RS485
 \endcode
 
 \note Wartoœci z \c #define mo¿na zmieniaæ w celu dostosowania 
 biblioteki do tworzonego urz¹dzenia i oprogramowania.
 
 \note \c UART_BUF_SIZE automatycznie w³¹cza obs³ugê UART na przerwaniach 
 (jeœli nie chcemy u¿ywaæ przerwañ to mo¿na po prostu nie definiowaæ \c UART_BUF_SIZE)
 
 \note \c UART_DE_PORT i \c UART_DE_BIT s¹ opcjonalne - mog¹ siê przydaæ 
 w przypadku realizacji komunikacji na dwuprzewodowym interfejsie 
 RS485 i s³u¿¹ do prze³¹czania kierunku transmisji.

 \par Wykorzystywane przerwania (tylko gdy zdefiniowano UART_BUF_SIZE):
 
 \c SIG_UART_RECV, \c SIG_UART_DATA, \c SIG_UART_TRANS (gdy zdefiniowano \c UART_DE_PORT)

\par Przyk³adowy program

\p "config.h"
\include uart/config.h

\p program g³ówny:
\include uart/testuart.c
*/

#ifndef __UART_H__
#define __UART_H__

/*@{*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>		// obs³uga uk³adu Watchdog
#include "config.h"
#include "global.h"
#include "conv.h"

#ifdef UART_BUF_SIZE	// je¿eli UART ma pracowaæ na przerwaniach

#define UART_TMASK		(UART_BUF_SIZE-1)
#define UART_RMASK		(UART_BUF_SIZE-1)

#include <avr/signal.h>		// konieczne dla przerwañ
#include <avr/interrupt.h>	// konieczne dla przerwañ

extern volatile uint8_t UART_tbuf[UART_BUF_SIZE];  // TX buffer
extern volatile uint8_t UART_rbuf[UART_BUF_SIZE];  // RX buffer

extern volatile uint8_t UART_t_in;            // TX buffer in index
extern volatile uint8_t UART_t_out;           // TX buffer out index

extern volatile uint8_t UART_r_in;            // RX buffer in index
extern volatile uint8_t UART_r_out;           // RX buffer out index

/** 
 Istnieje gdy zdefiniowano \c UART_BUF_SIZE
 \return iloœæ znaków w buforze odbiorczym.
*/
char UART_rxlen(void); 

#else        // jeœli UART pracuje bez u¿ycia przerwañ

/** 
 Istnieje gdy nie zdefiniowano \c UART_BUF_SIZE
 \return 0 gdy nie ma znaku w buforze odbiorczym.
*/
#define UART_rxlen()	bit_is_set(_USR_,RXC)

#endif  //UART_BUF_SIZE


// ---------------------------------------------------------------

/** 
 Inicjalizacja UART
*/
void UART_init(void);

/** 
 Wysy³a znak na UART
 \param c znak (bajt) do wys³ania
*/
int UART_putchar (char c);

/**
 Czeka i odbiera znak z UART
 \return odebrany znak z UART
*/
int UART_getchar (void);

/**
 Wysy³a ³añcuch z SRAM na UART
 \param s ³añcuch znaków w pamiêci SRAM
*/
void UART_putstr (char *s);

/**
 Wysy³a ³añcuch s z FLASH na UART
 \param s ³añcuch znaków w pamiêci FLASH 
*/
void UART_putstr_P (const char *s);

/** 
 Wysy³a ³añcuch s z EEPROM na UART
 \param s ³añcuch znaków w pamiêci EEPROM
*/
void UART_putstr_E(u08 *s);

/** 
 Pobiera ³añcuch z UART.
 £añcuch musi byæ zakoñczony znakiem "\n" lub "\r"
 i jego d³ugoœæ nie mo¿e przekroczyæ wartoœci
 zdefiniowanej w UART_MAX_GETSTR.
 \param s wskaŸnik do tablicy gdzie ma byæ odebrany ³añcuch
*/
void UART_getstr (char *s);

/** 
 Wysy³a na port szeregowy znakow¹ reprezentacjê liczby o wybranej postawie
 \note korzysta z funkcji \b itoa() co oznacza doœæ du¿e zapotrzebowanie na pamiêæ programu,
 jeœli nie ma specjalnej potrzeby aby j¹ zastosowaæ lepiej skorzystaæ z funkcji:
 UART_putU08(), UART_puthexU08(), UART_puthexU16().
 \param value wartoœæ do wys³ania
 \param radix podstawa np 2, 8, 10, 16
*/
void UART_putint(int value, u08 radix);

/** 
 Wysy³a na port szeregowy dziesiêtn¹ reprezentacjê liczby 8 bitowej
 \param value wartoœæ do wys³ania
*/
void UART_putU08(u08 value);

/** 
 Wysy³a na port szeregowy szesnastkow¹ reprezentacjê liczby 8 bitowej
 \param value wartoœæ do wys³ania
*/
void UART_puthexU08(u08 value);

/** 
 Wysy³a na port szeregowy szesnastkow¹ reprezentacjê liczby 16 bitowej
 \param value wartoœæ do wys³ania
*/
void UART_puthexU16(u16 value);

/*@}*/

#endif //__UART_H__
