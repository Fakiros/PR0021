#include "../i2c_ee.h"

void I2C_EE_send_adr(u16 address)
{
  u08 device=(address>>8);
  I2C_start();
  if (I2C_EE_TYPE==1)
    I2C_write(I2C_EE_BASE | (device<<1));  /* send preface 1010 a2 a1 a0 0 */
  if (I2C_EE_TYPE==2)
  {
    I2C_write(I2C_EE_BASE);	/* send preface 1010 a2 a1 a0 0 */
    I2C_write(address>>8);	/* send high byte of address */
  }
  I2C_write(address&0xff);	/* send address */
}
