#include "../i2c.h"

u08 I2C_write(u08 b)
{
  u08 i;
  I2C_SDA_WR();		// SDA na zapis
  for (i=0; i<8; i++)	// zapis 8 bitów
  {
    if (b & 0x80)
      I2C_SDA_H();
    else
      I2C_SDA_L();
    delay10us();
    I2C_SCL_H();
    delay10us();
    I2C_SCL_L();
    b <<= 1;		// przesuñ o 1 bit
  }
  I2C_SDA_RD();		// w³¹cz czytanie SDA
  I2C_SDA_H();		// podci¹gnij SDA
  delay10us();
  I2C_SCL_H();		// SCL=1
  delay10us();
  i=0xFF;
  do
  {
    if (bit_is_clear(I2C_PORT_I,I2C_SDA)) break;	// je¿eli jest potwierdzenie
    delay10us();
  }
  while(--i>0);
  I2C_SCL_L();		// SCL=0
  delay10us();
  return(i);
}
