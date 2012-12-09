#include "../pcf8583.h"

void PCF8583_set_date(u08 day,u08 month,u16 year)
{
  PCF8583_write_word(16,year);
  PCF8583_stop();
  PCF8583_write_date(5,day,year);
  PCF8583_write_bcd(6,month);
  PCF8583_start();
}
