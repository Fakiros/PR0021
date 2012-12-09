/** \file uart0.h \brief Sterownik interfejsu UART0. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** \defgroup uart Port szeregowy
 
 Obs�uga portu szeregowego UART0.
 
 \code
 #include "uart0.h"
 \endcode 
 
 \ref config:
 
 W pliku \c "config.h" znajduj�cym si� w katalogu projektu nale�y umie�ci� nast�puj�c� zawarto��:
    	     
\code
#define UART0_BAUD	19200		// pr�dko�� transmisji w bit/s
#define UART0_MAX_GETSTR	8		// maksymalna d�ugo�� tekstu dla UART0_getstr()
#define UART0_BUF_SIZE	8		// wielko�� bufor�w UART0 (musi by� pot�g� 2)
#define UART0_DE_PORT	PORTD		// port linii DE konwertera RS485
#define UART0_DE_BIT	7		// bit portu linii DE konwertera RS485
 \endcode
 
 \note Warto�ci z \c #define mo�na zmienia� w celu dostosowania 
 biblioteki do tworzonego urz�dzenia i oprogramowania.
 
 \note \c UART0_BUF_SIZE automatycznie w��cza obs�ug� UART0 na przerwaniach 
 (je�li nie chcemy u�ywa� przerwa� to mo�na po prostu nie definiowa� \c UART0_BUF_SIZE)
 
 \note \c UART0_DE_PORT i \c UART0_DE_BIT s� opcjonalne - mog� si� przyda� 
 w przypadku realizacji komunikacji na dwuprzewodowym interfejsie 
 RS485 i s�u�� do prze��czania kierunku transmisji.

 \par Wykorzystywane przerwania (tylko gdy zdefiniowano UART0_BUF_SIZE):
 
 \c SIG_UART0_RECV, \c SIG_UART0_DATA, \c SIG_UART0_TRANS (gdy zdefiniowano \c UART0_DE_PORT)

\par Przyk�adowy program

\p "config.h"
\include uart0/config.h

\p program g��wny:
\include uart0/testuart.c
*/

#ifndef __UART0_H__
#define __UART0_H__

/*@{*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>		// obs�uga uk�adu Watchdog
#include "config.h"
#include "global.h"
#include "conv.h"

#ifdef UART0_BUF_SIZE	// je�eli UART0 ma pracowa� na przerwaniach

#define UART0_TMASK		(UART0_BUF_SIZE-1)
#define UART0_RMASK		(UART0_BUF_SIZE-1)

#include <avr/signal.h>		// konieczne dla przerwa�
#include <avr/interrupt.h>	// konieczne dla przerwa�

extern volatile uint8_t UART0_tbuf[UART0_BUF_SIZE];  // TX buffer
extern volatile uint8_t UART0_rbuf[UART0_BUF_SIZE];  // RX buffer

extern volatile uint8_t UART0_t_in;            // TX buffer in index
extern volatile uint8_t UART0_t_out;           // TX buffer out index

extern volatile uint8_t UART0_r_in;            // RX buffer in index
extern volatile uint8_t UART0_r_out;           // RX buffer out index

/** 
 Istnieje gdy zdefiniowano \c UART0_BUF_SIZE
 \return ilo�� znak�w w buforze odbiorczym.
*/
char UART0_rxlen(void); 

#else        // je�li UART0 pracuje bez u�ycia przerwa�

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
 Wysy�a znak na UART0
 \param c znak (bajt) do wys�ania
*/
int UART0_putchar (char c);

/**
 Czeka i odbiera znak z UART0
 \return odebrany znak z UART0
*/
int UART0_getchar (void);

/**
 Wysy�a �a�cuch z SRAM na UART0
 \param s �a�cuch znak�w w pami�ci SRAM
*/
void UART0_putstr (char *s);

/**
 Wysy�a �a�cuch s z FLASH na UART0
 \param s �a�cuch znak�w w pami�ci FLASH 
*/
void UART0_putstr_P (const char *s);

/** 
 Wysy�a �a�cuch s z EEPROM na UART0
 \param s �a�cuch znak�w w pami�ci EEPROM
*/
void UART0_putstr_E(u08 *s);

/** 
 Pobiera �a�cuch z UART0.
 �a�cuch musi by� zako�czony znakiem "\n" lub "\r"
 i jego d�ugo�� nie mo�e przekroczy� warto�ci
 zdefiniowanej w UART0_MAX_GETSTR.
 \param s wska�nik do tablicy gdzie ma by� odebrany �a�cuch
*/
void UART0_getstr (char *s);

/** 
 Wysy�a na port szeregowy znakow� reprezentacj� liczby o wybranej postawie
 \note korzysta z funkcji \b itoa() co oznacza do�� du�e zapotrzebowanie na pami�� programu,
 je�li nie ma specjalnej potrzeby aby j� zastosowa� lepiej skorzysta� z funkcji:
 UART0_putU08(), UART0_puthexU08(), UART0_puthexU16().
 \param value warto�� do wys�ania
 \param radix podstawa np 2, 8, 10, 16
*/
void UART0_putint(int value, u08 radix);

/** 
 Wysy�a na port szeregowy dziesi�tn� reprezentacj� liczby 8 bitowej
 \param value warto�� do wys�ania
*/
void UART0_putU08(u08 value);

/** 
 Wysy�a na port szeregowy szesnastkow� reprezentacj� liczby 8 bitowej
 \param value warto�� do wys�ania
*/
void UART0_puthexU08(u08 value);

/** 
 Wysy�a na port szeregowy szesnastkow� reprezentacj� liczby 16 bitowej
 \param value warto�� do wys�ania
*/
void UART0_puthexU16(u16 value);

/*@}*/

#endif //__UART0_H__
