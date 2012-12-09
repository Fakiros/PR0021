#include "../adc.h"

//! start a conversion on the current ADC_ input channel
void ADC_start_convert(void)
{
	sbi(ADCSR, ADIF);	// clear hardware "conversion complete" flag 
	sbi(ADCSR, ADSC);	// start conversion
}
