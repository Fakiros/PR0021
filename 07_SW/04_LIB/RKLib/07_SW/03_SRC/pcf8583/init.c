#include "../pcf8583.h"

void PCF8583_init(void)
{
  PCF8583_status=0;
  PCF8583_alarm=0;
  PCF8583_write(0,0);
  PCF8583_write(4,PCF8583_read(4)&0x3f);
  PCF8583_write(8,0x90);
}
