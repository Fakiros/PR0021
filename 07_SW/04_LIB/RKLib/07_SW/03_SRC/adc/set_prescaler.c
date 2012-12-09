#include "../adc.h"

//! configure ADC_ converter clock division (prescaling)
void ADC_set_prescaler(unsigned char prescale)
{
	ADCSR =  (ADCSR & ~ADC_PRESCALE_MASK) | prescale;
}
