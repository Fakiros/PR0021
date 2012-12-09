#include "../spi.h"

u16 SPI_transfer_word(u16 data)
{
	u16 rxData = 0;

	// send MS byte of given data
	rxData = (SPI_transfer_byte((data>>8) & 0x00FF))<<8;
	// send LS byte of given data
	rxData |= (SPI_transfer_byte(data & 0x00FF));

	// return the received data
	return rxData;
}
