#include "../adc.h"

//! configure ADC_ converter voltage reference
void ADC_set_reference(unsigned char ref)
{
	ADMUX = (ADMUX & ~ADC_REFERENCE_MASK) | (ref<<6);
}
