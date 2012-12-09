#include "../adc.h"

//! Perform a 8-bit conversion.
// starts conversion, waits until conversion is done, and returns result
unsigned char ADC_convert_8bit(unsigned char ch)
{
	// do 10-bit conversion and return highest 8 bits
	return ADC_convert_10bit(ch)>>2;			// return ADC MSB byte
}
