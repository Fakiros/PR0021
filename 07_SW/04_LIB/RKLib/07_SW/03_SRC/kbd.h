/*! \file kbd.h \brief Programowa obs³uga klawiatury matrycowej 4x4. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** 
 \defgroup kbd Klawiatura 4x4
 
 Programowa obs³uga klawiatury matrycowej 4x4.
 
 \code
 #include "kbd.h"
 \endcode 
 
 Klawiatura matrycowa jest czêsto spotykanym urz¹dzeniem 
 wejœciowym w uk³adach mikroprocesorowych. 
 W zwi¹zku z tym powsta³a biblioteka, która pozwala na 
 wykorzystanie klawiatury 16 przyciskowej o organizacji 
 4x4 pod³¹czonej do dowolnych linii mikrokontrolera
 a nawet ich wspó³dzielenie np. przez \ref lcd.
 
 \ref config:

 W pliku \c "config.h" znajduj¹cym siê w katalogu projektu nale¿y umieœciæ nastêpuj¹c¹ zawartoœæ:
    	     
 \code
// linie kolumn klawiatury
#define KBD_PORT_col1	PORTB
#define KBD_BIT_col1	7
#define KBD_PORT_col2	PORTB
#define KBD_BIT_col2	6
#define KBD_PORT_col3	PORTB
#define KBD_BIT_col3	5
#define KBD_PORT_col4	PORTB
#define KBD_BIT_col4	4

// linie wierszy klawiatury
#define KBD_PORT_row1	PORTD
#define KBD_BIT_row1	0
#define KBD_PORT_row2	PORTD
#define KBD_BIT_row2	1
#define KBD_PORT_row3	PORTD
#define KBD_BIT_row3	2
#define KBD_PORT_row4	PORTD
#define KBD_BIT_row4	3
// linie wierszy klawiatury mo¿na wspó³dzieliæ
// z liniami danych wyœwietlacza LCD
 \endcode
 
 \note Wartoœci z \c #define mo¿na zmieniaæ w celu dostosowania 
 biblioteki do tworzonego urz¹dzenia i oprogramowania.
 
 \par Wykorzytywane biblioteki: 

 \ref delay
*/
  	     
#ifndef __KBD_H__
#define __KBD_H__

#include <avr/io.h>
#include "config.h"
#include "global.h"
#include "delay.h"

/*@{*/

#define KBD_PORT_D_col1 DDR(KBD_PORT_col1)
#define KBD_PORT_D_col2 DDR(KBD_PORT_col2)
#define KBD_PORT_D_col3 DDR(KBD_PORT_col3)
#define KBD_PORT_D_col4 DDR(KBD_PORT_col4)
#define KBD_PORT_I_col1 PIN(KBD_PORT_col1)
#define KBD_PORT_I_col2 PIN(KBD_PORT_col2)
#define KBD_PORT_I_col3 PIN(KBD_PORT_col3)
#define KBD_PORT_I_col4 PIN(KBD_PORT_col4)

#define KBD_PORT_D_row1 DDR(KBD_PORT_row1)
#define KBD_PORT_D_row2 DDR(KBD_PORT_row2)
#define KBD_PORT_D_row3 DDR(KBD_PORT_row3)
#define KBD_PORT_D_row4 DDR(KBD_PORT_row4)
#define KBD_PORT_I_row1 PIN(KBD_PORT_row1)
#define KBD_PORT_I_row2 PIN(KBD_PORT_row2)
#define KBD_PORT_I_row3 PIN(KBD_PORT_row3)
#define KBD_PORT_I_row4 PIN(KBD_PORT_row4)

#define KBD_row1	bit_is_clear(KBD_PORT_I_row1,KBD_BIT_row1)
#define KBD_row2	bit_is_clear(KBD_PORT_I_row2,KBD_BIT_row2)
#define KBD_row3	bit_is_clear(KBD_PORT_I_row3,KBD_BIT_row3)
#define KBD_row4	bit_is_clear(KBD_PORT_I_row4,KBD_BIT_row4)
#define KBD_col1_0	cbi(KBD_PORT_col1,KBD_BIT_col1)
#define KBD_col2_0	cbi(KBD_PORT_col2,KBD_BIT_col2)
#define KBD_col3_0	cbi(KBD_PORT_col3,KBD_BIT_col3)
#define KBD_col4_0	cbi(KBD_PORT_col4,KBD_BIT_col4)
#define KBD_col1_1	sbi(KBD_PORT_col1,KBD_BIT_col1)
#define KBD_col2_1	sbi(KBD_PORT_col2,KBD_BIT_col2)
#define KBD_col3_1	sbi(KBD_PORT_col3,KBD_BIT_col3)
#define KBD_col4_1	sbi(KBD_PORT_col4,KBD_BIT_col4)

/**
 Makro sprawdzaj¹ce czy przyciœniêto klawisz
*/
#define KBD_pressed()	KBD_read()>0

/**
 Zmienna przechowuj¹ca kod ostatnio przyciœniêtego klawisza.
*/
extern u08 KBD_key;

/* ------------------------------------------------------- */
/*	      Procedury obs³ugi klawiatury matrycowej	   */
/* ------------------------------------------------------- */

/**
 Inicjuje klawiaturê (ustawia kierunki w portach, "podci¹ga" wejœcia)
 zgodnie z ustawieniami w pliku "config.h".
*/
void KBD_init(void);

/**
 Czyta klawiaturê i zwraca kod naciœniêtego klawisza 1-16.
 Jeœli nie naciœniêto ¿adnego klawisza zwraca 0
 \return nr naciœniêtego klawisza (1-16), 0 gdy nienaciœniêty
*/
u08 KBD_read(void);

/**
 Oczekiwanie na puszczenie klawisza
*/
void KBD_wait(void);

/*@}*/

#endif // __KBD_H__
