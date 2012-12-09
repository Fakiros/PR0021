#include "../ds18x20.h"

uint8_t DS18X20_recall_E2( uint8_t id[] )
{
	OW_reset(); //**
	if( OW_input_pin_state() ) { // only send if bus is "idle" = high
		OW_command( DS18X20_RECALL_E2, id );
		// TODO: wait until status is "1" (then eeprom values
		// have been copied). here simple delay to avoid timeout 
		// handling
		delayms(DS18X20_COPYSP_DELAY);
		return DS18X20_OK;
	} 
	else { 
		return DS18X20_ERROR;
	}
}
