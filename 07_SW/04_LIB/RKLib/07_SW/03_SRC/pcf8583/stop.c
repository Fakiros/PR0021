#include "../pcf8583.h"

void PCF8583_stop(void)
{
  PCF8583_get_status();
  PCF8583_status|=0x80;
  PCF8583_write(0,PCF8583_status);
}
