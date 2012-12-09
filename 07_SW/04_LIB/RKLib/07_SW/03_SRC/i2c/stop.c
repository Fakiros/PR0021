#include "../i2c.h"

void I2C_stop(void)
{
  I2C_SDA_WR();		// SDA na zapis
  I2C_SCL_H();
  delay10us();
  I2C_SDA_H();
  delay10us();
}
