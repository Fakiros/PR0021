#ifndef _sin_table_h_
#define _sin_table_h_

#include <avr/pgmspace.h>

#define SIN_TABLE_SAMPLES_1Q	4000
#define SIN_TABLE_SAMPLES_2Q	(2*SIN_TABLE_SAMPLES_1Q)
#define SIN_TABLE_SAMPLES_4Q	(2*SIN_TABLE_SAMPLES_2Q)

extern PROGMEM  unsigned int  sin_table_q[SIN_TABLE_SAMPLES_1Q];

#endif /*_sin_table_h_ */
