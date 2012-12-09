#include "../i2c_ee.h"

u08 I2C_EE_write_page(u16 address, u08* dat, u08 cnt)
{
  u08 ack_bit=0, n;
  I2C_EE_send_adr(address);
  for(n=0; n<cnt; n++)	 /* write page */
    ack_bit=I2C_write(dat[n]);	 /* and finally the actual data */
  delayms(5);
  I2C_stop();
  return(ack_bit);   		 /* 0 if successful */
}
