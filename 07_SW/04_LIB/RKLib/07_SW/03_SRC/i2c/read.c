#include "../i2c.h"

u08 I2C_read(u08 ack)
{
  u08 i;
  u08 b = 0;
  I2C_SDA_RD();		// SDA na odczyt
  I2C_SDA_H();		// podci¹ganie SDA
  delay10us();
  for (i=0; i<8; i++)	// czytanie 8 bitów
  {
    I2C_SCL_H();	// SCL w stan wysoki
    delay10us();
    b <<= 1;		// przesuñ o 1 bit
    if (bit_is_set(I2C_PORT_I,I2C_SDA)) // jeœli SDA=1 
      b |= 1;		// dodaj odczytany bit z magistrali
    I2C_SCL_L();	// SCL w stan niski
    delay10us();
  }
  I2C_SDA_WR();		// SDA na zapis
  if (ack == 0)	// ustaw bit ACK na okreœlon¹ wartoœæ
    I2C_SDA_L();
  else
    I2C_SDA_H();
  delay10us();
  I2C_SCL_H();
  delay10us();
  I2C_SCL_L();
  delay10us();
  I2C_SDA_L();
  delay10us();
  return(b);
}
