#include "../adc.h"

//! Perform a 10-bit conversion
// starts conversion, waits until conversion is done, and returns result
unsigned short ADC_convert_10bit(unsigned char ch)
{
	ADC_complete_flag = 0;				// clear conversion complete flag
	ADMUX = (ADMUX & ~ADC_MUX_MASK) | (ch & ADC_MUX_MASK);	// set channel
	sbi(ADCSR, ADIF);						// clear hardware "conversion complete" flag 
	sbi(ADCSR, ADSC);						// start conversion
	//while(!ADC_complete_flag);				// wait until conversion complete
	//while( bit_is_clear(ADCSR, ADIF) );		// wait until conversion complete
	while( bit_is_set(ADCSR, ADSC) );		// wait until conversion complete

	// CAUTION: MUST READ ADCL BEFORE ADCH!!!
	return (ADCL | (ADCH<<8));	// read ADC (full 10 bits);
}
