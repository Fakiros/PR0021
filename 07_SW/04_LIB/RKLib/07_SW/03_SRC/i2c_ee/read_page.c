#include "../i2c_ee.h"

void I2C_EE_read_page(u16 address, u08* dat, u08 cnt)
{
  u08 n;
  u08 device=(address>>8);
  cnt--;
  I2C_EE_send_adr(address);
  if (I2C_EE_TYPE>0)
    I2C_start();
  if (I2C_EE_TYPE==1)
    I2C_write((I2C_EE_BASE|1) | (device<<1));  /* 24C01A to 24C16 */
  if (I2C_EE_TYPE==2)
    I2C_write(I2C_EE_BASE|1);                /* 24C64 to 24C512 */
  for(n=0; n<cnt; n++)	
    dat[n]=I2C_read(0);	
  dat[cnt]=I2C_read(1);	
  I2C_stop();
}

