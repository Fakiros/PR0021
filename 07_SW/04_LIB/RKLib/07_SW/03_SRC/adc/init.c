#include "../adc.h"

// global variables

//! software flag used to indicate when
//! the ADC_ conversion is complete
volatile unsigned char ADC_complete_flag;

// functions

//! initialize ADC_ converter
void ADC_init(void)
{
	sbi(ADCSR, ADEN);				// enable ADC (turn on ADC power)
	cbi(ADCSR, ADFR);				// default to single sample convert mode
	ADC_set_prescaler(ADC_PRESCALE);	// set default prescaler
	ADC_set_reference(ADC_REFERENCE);	// set default reference
	cbi(ADMUX, ADLAR);				// set to right-adjusted result

	sbi(ADCSR, ADIE);				// enable ADC interrupts

	ADC_complete_flag = 0;		// clear conversion complete flag
	sei();							// turn on interrupts (if not already on)
}

//! interrupt handler for ADC complete interrupt
SIGNAL(SIG_ADC)
{
	// set the ADC_ conversion flag to indicate "complete"
	ADC_complete_flag = 1;
}

