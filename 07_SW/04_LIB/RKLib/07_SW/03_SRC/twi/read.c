#include "../twi.h"

int TWI_read(unsigned char address, unsigned char *data, unsigned char bytes)
{
  while(TWI_stat & (1<<TWI_BUSY));  // Bus is busy wait (or exit with error code)

  TWI_address = address;
  TWI_data = data;
  TWI_bytes = bytes;
  TWI_ddr = TW_READ;

  TWI_retry_cnt = 0;
  
  /* Generate start condition, the remainder of the transfer is interrupt driven and
     will be performed in the background */
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
  
  TWI_stat |= (1<<TWI_BUSY);
  return 0;
}

