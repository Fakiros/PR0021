#include "../pcf8583.h"

u08 PCF8583_read_bcd(u08 address)
{
  return bcd2bin(PCF8583_read(address));
}
