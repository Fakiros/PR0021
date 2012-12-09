#include "./ds18x20.h"

/* get power status of DS18x20 
   input  : id = rom_code 
   returns: DS18X20_POWER_EXTERN or DS18X20_POWER_PARASITE */
uint8_t	DS18X20_get_power_status(uint8_t id[])
{
	uint8_t pstat;
    OW_reset();
    OW_command(DS18X20_READ_POWER_SUPPLY, id);
    pstat=OW_bit_io(1); // pstat 0=is parasite/ !=0 ext. powered
    OW_reset();
	return (pstat) ? DS18X20_POWER_EXTERN:DS18X20_POWER_PARASITE;
}
