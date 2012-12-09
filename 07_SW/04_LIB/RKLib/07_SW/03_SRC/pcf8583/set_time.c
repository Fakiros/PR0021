#include "../pcf8583.h"

void PCF8583_set_time(u08 hour,u08 min,u08 sec,u08 hsec)
{
//  if (hour>23) hour=0;
//  if (min>59) min=0;
//  if (sec>59) sec=0;
//  if (hsec>100) hsec=0;
  PCF8583_stop();
  PCF8583_write_bcd(1,hsec);
  PCF8583_write_bcd(2,sec);
  PCF8583_write_bcd(3,min);
  PCF8583_write_bcd(4,hour);
  PCF8583_start();
}
