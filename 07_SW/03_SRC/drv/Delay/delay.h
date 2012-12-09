/** \file delay.h \brief Funkcje opóŸniaj¹ce. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/**
 \defgroup delay Funkcje opóŸniaj¹ce

 Funkcje wstrzymuj¹ce dzia³anie programu na okreœlony czas.

 \code
 #include "delay.h"
 \endcode
*/
#ifndef __DELAY_H__
#define __DELAY_H__

#include <inttypes.h>

#include "appl/config.h"
//#include <avr/delay.h>
#include "drv/global.h"

/*@{*/

/* delay function for microsec
   4 cpu cycles per loop + 1 cycles(?) overhead 
   when a constant is passed. */
static inline void delayloop16(uint16_t count)
{
	asm volatile (  "cp  %A0,__zero_reg__ \n\t"  \
                     "cpc %B0,__zero_reg__ \n\t"  \
                     "breq L_Exit_%=       \n\t"  \
                     "L_LOOP_%=:           \n\t"  \
                     "sbiw %0,1            \n\t"  \
                     "brne L_LOOP_%=       \n\t"  \
                     "L_Exit_%=:           \n\t"  \
                     : "=w" (count)
					 : "0"  (count)
                   );                            
}
// delayloop16(x) eats 4 cycles per x
#define DELAY_US_CONV(us) ((uint16_t)(((((us)*1000L)/(1000000000/F_CPU))-1)/4))

/* delay function for millisec
  (6 cycles per x + 20(?) overhead) */
void delayloop32( uint32_t l); // not inline
#define DELAY_MS_CONV(ms) ( (uint32_t) (ms*(F_CPU/6000L)) ) 

/* mth 9/04:
   Remark uSeconds:
   Main Oscillator Clock given by F_OSC (makefile) in Hz
   one CPU-Cycle takes 1/F_OSC seconds => 1000000/F_OSC uSeconds
   so: 1 uSecond takes F_OSC/1000000 CPU-Cyles. The following code
   is inspired by the avr-libc delay_loop2 function.
   This it not "that precise" since it takes at least 4 cycles
   but should be o.k. with any parameter (even 0).
   Call function with delayloop(DELAYUS(dt [in uSeconds])).
*/ 


/**
 Wstrzymuje dzia³anie programu na count*us
 Zakres od 1us do 65,535ms.
 \param count opóŸnienie w us
*/
//void delayus(u16 count);
//#define delayus(count) _delay_us(count)
#define delayus(us)	  delayloop16(DELAY_US_CONV(us))

/**
 Wstrzymuje dzia³anie programu na 10us.
 Czêsto u¿ywane opóŸnienie - minimalizuje u¿ycie
 pamiêci programu z uwagi na nie przekazywanie parametrów do funkcji.
*/
void delay10us(void);

/**
 Wstrzymuje dzia³anie programu na 20us.
 Czêsto u¿ywane opóŸnienie - minimalizuje u¿ycie
 pamiêci programu z uwagi na nie przekazywanie parametrów do funkcji.
*/
void delay20us(void);

/**
 Wstrzymuje dzia³anie programu na count*ms
 Zakres od 1ms do 65,535s.
 \param count opóŸnienie w ms
*/
//void delayms(u32 count);
//#define delayms(count) _delay_ms(count)
#define delayms(ms)  delayloop32(DELAY_MS_CONV(ms))

/*@}*/

#endif // __DELAY_H__

