#include <avr/io.h>                // dostêp do rejestrów
#include <avr/interrupt.h>        // funkcje sei(), cli()
#include <avr/signal.h>                // definicje SIGNAL, INTERRUPT
#include <avr/interrupt.h>	// konieczne dla przerwañ
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>		// obs³uga uk³adu Watchdog
//#include "config.h"
//#include "global.h"
//#include "conv.h"

#define COUNTED_MAX_VALUE 255-156
volatile uint16_t overflow;

struct {
		volatile uint16_t t1;
		volatile uint16_t t2;
		}
		timer;




void init_timer0(void)
{
  
      TCCR0 = (0 
	        | (1<< FOC0)	// Force output compare
//	        | (1<< WGM00)	// Wafefrom generation mode: CTC
			| (1<< WGM01)   // 
//			| (1<< COM00)
//			| (1<< COM01)
			| (1<< CS02)    //prescaler f/1024
//			| (1<< CS01)
			| (1<< CS00)
			);
	  TIMSK |= (0 
//	        | (1<< TOIE0)	// Force output compare
	        | (1<< OCIE0)	// Wafefrom generation mode: CTC
			);
      TIFR  |= (0 
//	        | (1<< TOV0)	// Force output compare
	        | (1<< OCF0)	// Wafefrom generation mode: CTC
			);
      OCR0 = 160;
      sei ();
	  //  TIFR = _BV(OCF0);
//  TIMSK = _BV(OCIE0);        // w³¹cz obs³ugê przerwañ T/C0
  TCNT0 = 16;         // wartoœæ pocz¹tkowa T/C0
//  TCCR0 = _BV(CS00)|_BV(CS02); // preskaler 1024
//  TIFR = _BV(OCF0);
//  sei ();
}


SIGNAL (TIMER0_COMP_vect)
{
 // PORTB |= (1 << 0);
  
  //TCNT0 = 1;                // prze³aduj timer 0
  if (overflow>0)
    overflow--;                        // dekrementuj
  if (timer.t1>0)
    timer.t1--;                        // dekrementuj
  if (timer.t2>0)
    timer.t2--;                        // dekrementuj
  
  // PORTB &= ~(1 << 0);
}
