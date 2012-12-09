#include <avr/io.h>                // dostàp do rejestr¡w
#include <avr/interrupt.h>        // funkcje sei(), cli()
//#include <avr/signal.h>                // definicje SIGNAL, INTERRUPT

//#include <avr/pgmspace.h>
//#include <avr/wdt.h>		// obs-uga uk-adu Watchdog
//#include "config.h"
//#include "global.h"
//#include "conv.h"

#include "sin_out.h"
#include "..\..\03_SRC\drv\encoder.h"


typedef enum 
{
				PWM_OFF,
				PWM_SIN_ON,
				PWM_SET_ON
} PWM_statuss_t;			

static PWM_statuss_t PWM_Statuss = PWM_OFF;
static unsigned int  PWM_Set = 0;
static unsigned char  PWM_DeadTime = 0;



static void init_timer1(void);

//static unsigned int (*func)(void);



void PWM_init(void)
{
	OCR1AH = 0;
	OCR1AL = 0; 
	OCR1BH = 0;
	OCR1BL = 0; 
	init_timer1();
}

void PWM_SetValue(unsigned int value)
{
	PWM_Statuss = PWM_SET_ON;
	PWM_Set = value;
}

void PWM_SetSin(unsigned int freq)
{
	PWM_Statuss = PWM_SIN_ON;
	PWM_Set = freq;
}

void PWM_SetDeadTime(unsigned char deadtime)
{
	PWM_DeadTime = deadtime;
}


void PWM_Stop(void)
{
	PWM_Statuss = PWM_SIN_ON;
	PWM_Set = 0;
}

void init_timer1(void)
{

  // Initial Port I/O
  DDRD |= (1 << 4 | 1 << 5);           // Set PORTB as Output

  TCNT1 	= 0;           // Reset TCNT0
  
  TCCR1A 	= ( 0
  			|(1 << COM1A1)
			|(1 << COM1A0)   // set on compare match
			|(1 << COM1B1)
//			|(1 << COM1B0)   // clear on compare match
//			|(1 << FOC1A)
//			|(1 << FOC1B)
			|(1 << WGM11)
//			|(1 << WGM10)
            ); // Fast PWM 8 Bit

   TCCR1B 	= ( 0
// 			| (1 << INC1)
//			|(1 << ICES1)
			|(1 << WGM13)
			|(1 << WGM12)
//			|(1 << CS12)
//			|(1 << CS11)
			|(1 << CS10)

           ); // Fast PWM 8 Bit

   TIMSK 	= ( TIMSK
//			|(1 << TICIE)
//			|(1 << OCIE1A)
//			|(1 << OCIE1B)
			|(1 << TOIE1)
   		   );
  ICR1H = 0x03;
  ICR1L = 0xE7;
 }


SIGNAL (TIMER1_OVF_vect)
{
	unsigned int data;
	unsigned int data_hd;
	unsigned int data_ld;
	unsigned char data_l;
	unsigned char data_h;
	if (PWM_Statuss == PWM_SIN_ON)
	{
		data =   Get_sample_next(PWM_Set);
	}

	if (PWM_Statuss == PWM_SET_ON)
	{
		data  = PWM_Set;
	}


    if (data > PWM_DeadTime)
    {
		data_hd = data - PWM_DeadTime;
    }
	else
	{
		data_hd = 0;
	}


	if ((data + PWM_DeadTime) < 1024)
	{
		data_ld = data + PWM_DeadTime;
	}
	else
	{
		data_ld = 1024;
	}

	data_ld = data + PWM_DeadTime;
	
	
	data_l = (unsigned char)(data_hd & 0x00FFu);
	data_h = (unsigned char)(data_hd >> 8);	
	OCR1BH = data_h;
	OCR1BL = data_l; 

	data_l = (unsigned char)(data_ld & 0x00FFu);
	data_h = (unsigned char)(data_ld >> 8);	
	OCR1AH = data_h;
	OCR1AL = data_l;

//	encoder_debounce();

}
