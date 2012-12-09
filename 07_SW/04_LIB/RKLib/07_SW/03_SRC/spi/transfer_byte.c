#include "../spi.h"

u08 SPI_transfer_byte(u08 data)
{
	// send the given data
	SPI_send_byte(data);
	// wait for transfer to complete
	#ifdef SPI_USEINT
		while(!SPI_transfer_complete);
	#else
		while(!(SPSR) & (1<<SPIF));
		// *** reading of the SPSR and SPDR are crucial
		// *** to the clearing of the SPIF flag
		// *** in non-interrupt mode
		//inp(SPDR);
		// set flag
		SPI_transfer_complete = 1;
	#endif
	// return the received data
	return SPDR;
}
