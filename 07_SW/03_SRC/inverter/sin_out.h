#ifndef _sin_out_h_
#define _sin_out_h_

#include "pwm.h"

#define SIN_FULL_PERIOD PWM_FULL_PERIOD

extern unsigned int Get_sample(unsigned int sample);
extern unsigned int Get_sample_next(unsigned int sample_shift);

#endif /* _sin_out_h_ */
