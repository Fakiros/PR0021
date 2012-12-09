/* **********************************************************************
**
**  Copyright (C) 2003  Jesper Hansen <jesperh@telia.com> and 
**      Romuald Bialy (MIS) <romek_b@o2.pl>.
**
*************************************************************************
**
**   This file is part of the yampp system.
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software Foundation, 
**  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
*********************************************************************** */

#include "../remote.h"

#if (REMOTE_STD == 1)
/*
  The REC-80 format used by Panasonic is a space coded 48 bit code consisting 
  of a 32 bit group id, followed by a 16 bit commmand word.
  Leading this is a header consisting of a 10T signal and a 4T pause.
  All bits start with a pulse of length T. The length of the pause following
  indicates the bit value. A T pause for a 0-bit and a 3T pause for a 1-bit.
*/

u16 REMOTE_get(void)
{
  u08 i, tmp = 0;
  u08 time;
  u08 T2,T4;
  union u16convert code;

  loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);  // skip leading signal

  TCCR0=4;        //update every 32us
  TCNT0 = 1;

  while (bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT))
  {
    T2 = TCNT0;
    if (T2 >= 100)    // max wait time
      return 0;
  }
  
  // measure time T

  TCNT0 = 1;
  loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);
  
  T2 = TCNT0;    // T is normally around 0E-10 hex = 15 -> 480 uS
  T2 = T2 * 2;
  // max time is 4T
  T4 = T2 * 2;    

  for (i = 0; i < 48; i++)
  {
    TCNT0 = 1;
    while(1)
    {
      time = TCNT0;
      if (time > T4)
        return 0;
      
      // measure time on the lo flank
      if (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT))
      {
        tmp <<= 1;
        if (time >= T2)
          tmp++;
        break;
      }
    }

    // save command data as we go
    if( i == 39)
      code.bytes.low = tmp;

    if( i == 47)
      code.bytes.high = tmp;
    
    // syncronize - wait for next hi flank
    loop_until_bit_is_set(REMOTE_PORT - 2, REMOTE_BIT);
  }
  return code.value;
}
#endif // (REMOTE_STD == 1)

//---------------------------------------------------------------------------------------

#if (REMOTE_STD == 2)
/*
  The NEC format is a space coded 32 bit code consisting 
  of a 16 bit group id, followed by a 16 bit commmand word.
  Leading this is a header consisting of a 16T signal and a 8T pause.
  All bits start with a pulse of length T. The length of the pause following
  indicates the bit value. A T pause for a 0-bit and a 3T pause for a 1-bit.

  Modified REC80 format by Will Jenkins, wdj@cus.org.uk
*/

u16 REMOTE_get(void)
{
  u08 i, tmp = 0;
  u08 time;
  u08 T2,T4;
  union u16convert code;

  loop_until_bit_is_set  (PIN(REMOTE_PORT), REMOTE_BIT);    // skip leading signal

  TCCR0 = (4);  //update every 32us
  TCNT0 = 1;

  while (bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT))
  {
    T2 = TCNT0;
    if (T2 >= 140)  // max wait time was 100
      return 0;
  }
  
  // measure time T
  TCNT0 = 1;
  loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);
  
  T2 = TCNT0;    // T is normally around 0E-10 hex = 15 -> 480 uS
  T2 = T2 * 2;
  // max time is 4T
  T4 = T2 * 2;    

  for (i = 0; i < 32; i++)
  {
    TCNT0 = 1;
    while(1)
    {
      time = TCNT0;
    
      if (time > T4)
        return 0;
      
      // measure time on the lo flank
      if (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT))
      {
        tmp <<= 1;
        if (time >= T2)
          tmp += 1;
        break;
      }
    }

    // save command data as we go
    if( i == 15)
      code.bytes.high = tmp;

     if( i == 31)
      code.bytes.low = tmp;
    
    // syncronize - wait for next hi flank
    loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);
   }

  return code.value;
 }
#endif // (REMOTE_STD == 2)

//---------------------------------------------------------------------------------------

#if (REMOTE_STD == 3) || (REMOTE_STD == 4)
/*
  The SONY format is a pulse coded 12 or 15 bit code consisting 
  of a 6 bit device code, followed by a 6 or 9 bit commmand word.
  Leading this is a header consisting of a 4T signal pulse.
  All bits are composed of a pulse of either 1T or 2T in length followed by a
  1T pause. A 2T pulse indicates a 1 bit, and a 1T pulse is a 0 bit.
*/ 

u16 REMOTE_get(void)
{
  u08  d, T2;
  u16 tmp;
  union u16convert code;

  code.value = 0;
  TCCR0 = (4);  //update every 32us
  TCNT0 = 1;
  loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);  // leading signal
  if (TCNT0 <= 60) return 0;      // is it the leading Signal ?      

  // now looking for the Data-bits
#if (REMOTE_STD == 3)
  for(d=0; d < 14; d++)
#else
  for(d=0; d < 11; d++)
#endif
  {
    tmp = 1;
    while (bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT))
    {
      T2 = TCNT0;
      if (T2 >= 140)      // max wait time
        return 0;
    }
    TCNT0 = 1;
    while (bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT))
    {
      T2 = TCNT0;
      if (T2 >= 140)      // max wait time
        return 0;
    }
    if (TCNT0 >= 35)      // pulse longer than 1 ms? then logic "1"
      code.value += (tmp << d);  // the device code 

  }

  return code.value;
}
#endif // (REMOTE_STD == 3) || (REMOTE_STD == 4)

//---------------------------------------------------------------------------------------

#if (REMOTE_STD == 5)
/*
  The RC-5 format used by Philips is bi-phase coded 13 bit code consisting 
  of a 7 bit device code, followed by 6 bit commmand word. Highest two bits of
  device code is startbit and troggle bit. Troggle bit must be masked out 
  because  its change after any keypress.
*/ 

u16 REMOTE_get(void)
{
  u08 i, time, T2, tmp = 0;
  union u16convert code;

  TCCR0 = (4);        //update every 32us
  loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);  // skip leading signal
  for(i=0; i<13; i++)
  {
    if(bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT) )
      T2 = 0;
    else
      T2 = 1;

    TCNT0 = 1;
    while(1)
    {
      time=TCNT0;
      if(time > 0x21)
        return 0;

      if(bit_is_clear(PIN(REMOTE_PORT), REMOTE_BIT) && (T2==1) )
      {
        tmp <<= 1;
        tmp++;
        break;
      }
      else if(bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT) && (T2==0) )
      {
        tmp <<= 1;
        break;
      }
    }

    //save address data
    if(i == 6)
    {
      code.bytes.high = (tmp & 0x5f);    // save address and cut troggle bit
      tmp=0;
    }

    //delay
    TCNT0 = 1;
    while(1)
    {
      time=TCNT0;
      if(time > 0x21)
        break;
    }
  }
  code.bytes.low = tmp;

  return code.value;
}

#endif // (REMOTE_STD == 5)

//---------------------------------------------------------------------------------------

