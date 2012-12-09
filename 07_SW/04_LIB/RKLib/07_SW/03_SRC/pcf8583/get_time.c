#include "../pcf8583.h"

void PCF8583_get_time(u08 *hour,u08 *min,u08 *sec,u08 *hsec)
{
  PCF8583_hold_on();
  *hsec=PCF8583_read_bcd(1);
  *sec=PCF8583_read_bcd(2);
  *min=PCF8583_read_bcd(3);
  *hour=PCF8583_read_bcd(4);
  PCF8583_hold_off();
}
