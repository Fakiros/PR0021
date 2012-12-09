#include "../pcf8583.h"

void PCF8583_hold_off(void)
{
  PCF8583_get_status();
  PCF8583_status&=0xbf;
  PCF8583_write(0,PCF8583_status);
}
