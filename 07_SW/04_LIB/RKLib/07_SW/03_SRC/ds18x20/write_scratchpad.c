#include "./ds18x20.h"

uint8_t DS18X20_write_scratchpad( uint8_t id[], 
	uint8_t th, uint8_t tl, uint8_t conf)
{
	OW_reset(); //**
	if( OW_input_pin_state() ) { // only send if bus is "idle" = high
		OW_command( DS18X20_WRITE_SCRATCHPAD, id );
		OW_byte_wr(th);
		OW_byte_wr(tl);
		if (id[0] == DS18B20_ID) OW_byte_wr(conf); // config avail. on B20 only
		return DS18X20_OK;
	} 
	else { 
		return DS18X20_ERROR;
	}
}
