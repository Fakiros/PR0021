#ifndef __KB_INCLUDED
#define __KB_INCLUDED

/*! \file ps2kbd.h \brief Obs³uga klawiatury PS2 i AT. */
/*
  Copyright (c) 2004,5 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** \defgroup ps2kbd Obs³uga klawiatury PS2 i AT
 
 Obs³uga klawiatury PS2 i AT.
 
 \code
 #include "ps2kbd.h"
 \endcode 
 
 \ref config:
 
 W pliku \c "config.h" znajduj¹cym siê w katalogu projektu nale¿y umieœciæ nastêpuj¹c¹ zawartoœæ:
 
\code
#define PS2KBD_PORT	PORTD
#define PS2KBD_CLOCK	2
#define PS2KBD_DATA	3
\endcode
 
 \note Z uwagi na wykorzystywanie przez prez procedury przewañ zewnêtrzych 0 lub 1 nie nale¿y zmieniaæ
 przypisania PS2KBD_PORT natomiast PS2KBD_CLOCK mo¿e przyjmowaæ wartoœci 2 lub 3 - biblioteka automatycznie
 przypisze odpowiednie przerwanie. Jedyn¹ wartoœci¹, któr¹ mo¿na w miarê bezpiecznie zmieniaæ jest przypisanie
 koncówki PS2KBD_DATA.
 
 \par Wykorzytywane przerwania:
 \c SIG_INTERRUPT0 lub \c SIG_INTERRUPT1

*/

// Keyboard communication routines

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>		// definicje SIGNAL, INTERRUPT
#include <avr/pgmspace.h>
#include "config.h"
#include "global.h"

#define PS2KBD_PIN	PIN(PS2KBD_PORT)

#ifndef PS2KBD_BUFF_SIZE
	#define PS2KBD_BUFF_SIZE 64
#endif

//#if (PS2KBD_PORT == 0x12)
	#if (PS2KBD_CLOCK == 2)
		#define PS2KBD_SIG_INTERRUPT SIG_INTERRUPT0
		#define PS2KBD_rising_edge 3
		#define PS2KBD_falling_edge 2
	#elif (PS2KBD_CLOCK==3)
		#define PS2KBD_SIG_INTERRUPT SIG_INTERRUPT1
		#define PS2KBD_rising_edge 8
		#define PS2KBD_falling_edge 12
	#else
		#error "PS2KBD_CLOCK musi byc ustawiony 2 lub 3 (INT0 lub INT1)"
	#endif
//#else
//#  error "PS2KBD_PORT musi byc ustawiony na PORTD"
//#endif

extern prog_char PS2KBD_unshifted[][2];
extern prog_char PS2KBD_shifted[][2];

unsigned char PS2KBD_edge, PS2KBD_bitcount;                // 0 = neg.  1 = pos.

unsigned char PS2KBD_buffer[PS2KBD_BUFF_SIZE];
unsigned char *PS2KBD_inpt, *PS2KBD_outpt;
unsigned char PS2KBD_buffcnt;

void PS2KBD_init(void);
void PS2KBD_decode(unsigned char sc);
void PS2KBD_put_buff(unsigned char c);
int PS2KBD_getchar(void);
#endif

