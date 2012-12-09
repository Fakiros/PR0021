#include "../spi.h"

void SPI_send_byte(u08 data)
{
	// send a byte over SPI and ignore reply
	if(SPI_transfer_complete)
	{
		SPI_transfer_complete = 0;
		SPDR = data;
	}
}
