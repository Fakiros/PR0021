#include "../adc.h"

//! sets the ADC_ input channel
void ADC_set_channel(unsigned char ch)
{
	ADMUX = (ADMUX & ~ADC_MUX_MASK) | (ch & ADC_MUX_MASK);	// set channel
}
