#include "../pcf8583.h"

void PCF8583_start(void)
{
  PCF8583_get_status();
  PCF8583_status&=0x7f;
  PCF8583_write(0,PCF8583_status);
}
