#include "../pcf8583.h"

void PCF8583_write_date(u08 address,u08 day,u16 year)
{
  PCF8583_write(address,bin2bcd(day)|(((u08) year&3)<<6));
}
