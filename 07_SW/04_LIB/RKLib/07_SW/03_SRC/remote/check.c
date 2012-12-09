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

u16 REMOTE_check(u08 std)
{
    u08 i, T2, T4, time, tmp = 0;
    union u16convert code;

    code.value = 0;
    TCCR0 = 4;        //update every 32us
    TCNT0 = 1;
    loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);  // skip leading signal

  if(std < 2)
  {  
    TCNT0 = 1;

    while (bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT))
    {
      T2 = TCNT0;
      if (T2 >= 140)        // max wait time
        return 0;
    }
  
    // measure time T

    TCNT0 = 1;
    loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);
  
    T2 = TCNT0;        // T is normally around 0E-10 hex = 15 -> 480 uS
    T2 = T2 * 2;
    // max time is 4T
    T4 = T2 * 2;
    
    for (i = 0; i < ((std == 0) ? 48 : 32); i++)
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
      if (std == 0)
      {
        if( i == 39)
          code.bytes.low = tmp;
        if( i == 47)
          code.bytes.high = tmp;
      }
      else
      {
        if( i == 15)
          code.bytes.high = tmp;
        if( i == 31)
          code.bytes.low = tmp;
      }

      // syncronize - wait for next hi flank
      loop_until_bit_is_set(PIN(REMOTE_PORT), REMOTE_BIT);
    }
    return (code.value);
  }
  else if(std < 4)          // Format used by SONY remote controllers
  {
    if (TCNT0 <= 60) 
      return 0;        // is it the leading Signal ?      

    // now looking for the Data-bits
    for(i=0; i < ((std == 3) ? 11 : 14); i++)
    {
      tmp = 0x01;
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
      if (TCNT0 >= 25)      // pulse longer than 1 ms? then logic "1"
        code.value += ((u16)tmp << i);  // the device code 
    }
    return (code.value);
  }

  else  // std=4 -> RC-5;

  {

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
    return(code.value);
  }
}
