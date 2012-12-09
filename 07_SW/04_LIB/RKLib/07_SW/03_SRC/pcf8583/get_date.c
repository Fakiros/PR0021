#include "../pcf8583.h"

void PCF8583_get_date(u08 *day,u08 *month,u16 *year)
{
  u08 dy;
  u16 y1;
  PCF8583_hold_on();
  dy=PCF8583_read(5);
  *month=bcd2bin(PCF8583_read(6)&0x1f);
  PCF8583_hold_off();
  *day=bcd2bin(dy&0x3f);
  dy>>=6;
  y1=PCF8583_read(16)|((u16) PCF8583_read(17)<<8);
  if (((u08) y1&3)!=dy) PCF8583_write_word(16,++y1);
  *year=y1;
}
