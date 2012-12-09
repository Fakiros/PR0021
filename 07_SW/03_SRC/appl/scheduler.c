#include <avr/io.h>                // dostêp do rejestrów
#include <avr/interrupt.h>        // funkcje sei(), cli()
#include <avr/signal.h>                // definicje SIGNAL, INTERRUPT
#include <avr/interrupt.h>	// konieczne dla przerwañ
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>		// obs³uga uk³adu Watchdog
//#include "config.h"
//#include "global.h"
#include "appl/scheduler.h"

#define CPU_CLK_TICKS	16000



// clbk function
extern void FastTask(void);

static unsigned int CPU_clk_ticks = 0;

volatile static unsigned char tick;
volatile static unsigned char overflow;

volatile unsigned int loop_timer;

void SchedulerInit()
{
     TCCR0 = (0 
	        | (1<< FOC0)	// Force output compare
//	        | (1<< WGM00)	// Wafefrom generation mode: CTC
			| (1<< WGM01)   // 
//			| (1<< COM00)
//			| (1<< COM01)
/*
             CS		prescaler
			 1		1
			 2		8
			 3		64
			 4		256
			 5		1024	
*/
//			| (1<< CS02)    //prescaler f/8
			| (1<< CS01)
			| (1<< CS00)
			);
	  TIMSK |= (TIMSK
//	        | (1<< TOIE0)	// Force output compare
	        | (1<< OCIE0)	// Wafefrom generation mode: CTC
			);
      TIFR  |= (TIFR
//	        | (1<< TOV0)	// Force output compare
	        | (1<< OCF0)	// Wafefrom generation mode: CTC
			);
 
      OCR0 = 250;
	  CPU_clk_ticks = CPU_CLK_TICKS;
	  DDRB |= (1 << 0);
	  //  TIFR = _BV(OCF0);
//  TIMSK = _BV(OCIE0);        // w³¹cz obs³ugê przerwañ T/C0
//  TCNT0 = COUNTED_MAX_VALUE;         // wartoœæ pocz¹tkowa T/C0
//  TCCR0 = _BV(CS00)|_BV(CS02); // preskaler 1024
//  TIFR = _BV(OCF0);
//  sei ();
}

void WaitForLoopFinished()
{
	
	loop_timer = ((uint16_t)tick * 250u)+ TCNT0 ;
    OCR0 = 250;
//	PORTB &= ~(1<<0);
	while (overflow == 0){}; //stop execution until tick period finished
	overflow = 0;
//	PORTB |= (1<<0);
}




/*
void PeriodicTask()
{
	
	tick++;
	if (tick >= MAIN_LOOP_PERIOD)
	{
//		PORTB |= (1<<0);
		overflow = 1;
		tick = 0;
//		PORTB &= ~(1<<0);
	}
	//FastTask();
}
*/



SIGNAL (TIMER0_COMP_vect)
{

	tick++;
	if (tick >= MAIN_LOOP_PERIOD)
	{
//		PORTB |= (1<<0);
		overflow = 1;
		tick = 0;
//		PORTB &= ~(1<<0);
	}
	FastTask();

 	
 //	 PeriodicTask();                      // dekrementuj

}

