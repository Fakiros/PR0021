#include "../pcf8583.h"

void PCF8583_alarm_on(void)
{
  PCF8583_get_status();
  PCF8583_status|=4;
  PCF8583_write(0,PCF8583_status);
}
