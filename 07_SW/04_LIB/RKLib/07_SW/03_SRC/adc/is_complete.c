#include "../adc.h"

//! return TRUE if conversion is complete
u08 ADC_is_complete(void)
{
	return bit_is_set(ADCSR, ADSC);
}
