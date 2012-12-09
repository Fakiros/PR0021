#include "../ds18x20.h"

uint8_t DS18X20_copy_scratchpad( uint8_t with_power_extern, 
	uint8_t id[] )
{
	OW_reset(); //**
	if( OW_input_pin_state() ) { // only send if bus is "idle" = high
		OW_command( DS18X20_COPY_SCRATCHPAD, id );
		if (with_power_extern != DS18X20_POWER_EXTERN)
			OW_parasite_enable();
		delayms(DS18X20_COPYSP_DELAY); // wait for 10 ms 
		if (with_power_extern != DS18X20_POWER_EXTERN)
			OW_parasite_disable();
		return DS18X20_OK;
	} 
	else { 
		return DS18X20_START_FAIL;
	}
}
