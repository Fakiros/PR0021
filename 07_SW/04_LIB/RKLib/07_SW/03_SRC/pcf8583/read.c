#include "../pcf8583.h"

u08 PCF8583_read(u08 address)
{
  u08 a;
  a=(PCF8583_A0<<1)|0xa0;
  I2C_start();
  I2C_write(a);
  I2C_write(address);
  I2C_start();
  I2C_write(a|1);
  a=I2C_read(1);
  I2C_stop();
  return a;
}
