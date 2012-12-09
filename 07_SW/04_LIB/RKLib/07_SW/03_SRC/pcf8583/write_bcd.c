#include "../pcf8583.h"

void PCF8583_write_bcd(u08 address,u08 data)
{
  PCF8583_write(address,bin2bcd(data));
}
