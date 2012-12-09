#include "../pcf8583.h"

void PCF8583_set_alarm_date(u08 day,u08 month)
{
  PCF8583_write_date(0xd,day,0);
  PCF8583_write_bcd(0xe,month);
}
