#include "./ds18x20.h"

/* start measurement (CONVERT_T) for all sensors if input id==NULL 
   or for single sensor. then id is the rom-code */
uint8_t DS18X20_start_meas( uint8_t with_power_extern, uint8_t id[])
{
	OW_reset(); //**
	if( OW_input_pin_state() ) { // only send if bus is "idle" = high
		OW_command( DS18X20_CONVERT_T, id );
		if (with_power_extern != DS18X20_POWER_EXTERN)
			OW_parasite_enable();
		return DS18X20_OK;
	} 
	else { 
		return DS18X20_START_FAIL;
	}
}
