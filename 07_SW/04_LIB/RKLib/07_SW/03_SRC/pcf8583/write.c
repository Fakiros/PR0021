#include "../pcf8583.h"

void PCF8583_write(u08 address,u08 data)
{
  I2C_start();
  I2C_write((PCF8583_A0<<1)|0xa0);
  I2C_write(address);
  I2C_write(data);
  I2C_stop();
}
