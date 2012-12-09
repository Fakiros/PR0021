#include "sin_table.h"
#include "sin_out.h"


unsigned int Get_sample(unsigned int sample)
{
	unsigned int out;
	if (sample > (SIN_TABLE_SAMPLES_2Q - 1))
	{
		sample = sample - (SIN_TABLE_SAMPLES_2Q );
		if (sample > (SIN_TABLE_SAMPLES_1Q - 1))
		{
			out = pgm_read_word	(	&sin_table_q[(SIN_TABLE_SAMPLES_2Q - 1) - sample] );
		}
		else
		{
			out = pgm_read_word	(	&sin_table_q[sample] );
		}
#if SIN_FULL_PERIOD == 1
		out = 500 - (out >> 1);
#endif
	}
	else
	{
		if (sample > (SIN_TABLE_SAMPLES_1Q - 1))
		{
			out = pgm_read_word	(	&sin_table_q[(SIN_TABLE_SAMPLES_2Q - 1) - sample] );
		}
		else
		{
			out = pgm_read_word	(	&sin_table_q[sample] );
		}
#if SIN_FULL_PERIOD == 1
		out = 500 + (out >> 1);
#endif
	}

	return out;
}

unsigned int Get_sample_next(unsigned int sample_shift)
{
	static volatile unsigned int sample_ptr = 0;
	unsigned int sample_ptr_temp;
	unsigned int out;
	out = Get_sample(sample_ptr);
	sample_ptr_temp = sample_ptr + sample_shift;
	if (sample_ptr_temp < SIN_TABLE_SAMPLES_4Q)
	{
		sample_ptr = sample_ptr_temp;

	}
	else
	{
		sample_ptr = sample_ptr_temp - SIN_TABLE_SAMPLES_4Q;
	}
	return out;
}


