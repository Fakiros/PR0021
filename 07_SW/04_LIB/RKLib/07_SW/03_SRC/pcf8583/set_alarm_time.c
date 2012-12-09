#include "../pcf8583.h"

void PCF8583_set_alarm_time(u08 hour,u08 min,u08 sec,u08 hsec)
{
  PCF8583_write_bcd(0x9,hsec);
  PCF8583_write_bcd(0xa,sec);
  PCF8583_write_bcd(0xb,min);
  PCF8583_write_bcd(0xc,hour);
}
