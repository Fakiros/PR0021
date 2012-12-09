#include "../pcf8583.h"

void PCF8583_get_alarm_time(u08 *hour,u08 *min,u08 *sec,u08 *hsec)
{
  *hsec=PCF8583_read_bcd(0x9);
  *sec=PCF8583_read_bcd(0xa);
  *min=PCF8583_read_bcd(0xb);
  *hour=PCF8583_read_bcd(0xc);
}
