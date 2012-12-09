#include "../pcf8583.h"

volatile u08 PCF8583_status;
volatile u08 PCF8583_alarm;

u08 PCF8583_get_status(void)
{
  PCF8583_status=PCF8583_read(0);
  PCF8583_alarm=(PCF8583_status&2);
  return PCF8583_status;
}
