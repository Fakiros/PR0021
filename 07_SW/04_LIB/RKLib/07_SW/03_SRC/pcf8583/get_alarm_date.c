#include "../pcf8583.h"

void PCF8583_get_alarm_date(u08 *day,u08 *month)
{
  *day=bcd2bin(PCF8583_read(0xd)&0x3f);
  *month=bcd2bin(PCF8583_read(0xe)&0x1f);
}
