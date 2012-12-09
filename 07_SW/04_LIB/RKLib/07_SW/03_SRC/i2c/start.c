#include "../i2c.h"

void I2C_start(void)
{
  I2C_SCL_H();
  I2C_SDA_H();
  I2C_SDA_WR();
  I2C_SCL_WR();	//inicjalizacja kierunków portów
  delay10us();
  I2C_SDA_L();
  delay10us();
  I2C_SCL_L();
  delay10us();
}
