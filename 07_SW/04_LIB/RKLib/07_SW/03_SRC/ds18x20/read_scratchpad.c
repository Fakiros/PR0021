#include "../ds18x20.h"

uint8_t DS18X20_read_scratchpad( uint8_t id[], uint8_t sp[] )
{
	uint8_t i;
	
	OW_reset(); //**
	if( OW_input_pin_state() ) { // only send if bus is "idle" = high
		OW_command( DS18X20_READ, id );
		for ( i=0 ; i< DS18X20_SP_SIZE; i++ )	sp[i]=OW_byte_rd();
		return DS18X20_OK;
	} 
	else { 
		return DS18X20_ERROR;
	}
}
