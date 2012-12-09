#include "../adc.h"

//! turn off ADC_ converter
void ADC_off(void)
{
	cbi(ADCSR, ADIE);				// disable ADC interrupts
	cbi(ADCSR, ADEN);				// disable ADC (turn off ADC power)
}
