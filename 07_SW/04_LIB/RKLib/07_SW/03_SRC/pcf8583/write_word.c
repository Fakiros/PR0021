#include "../pcf8583.h"

void PCF8583_write_word(u08 address,u16 data)
{
  PCF8583_write(address,(u08) data&0xff);
  PCF8583_write(++address,(u08)(data>>8));
}
