/************************************************************************/
/*                                                                      */
/*        Access Dallas 1-Wire Device with ATMEL AVRs                   */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/* modified by Martin Thomas <eversmith@heizung-thomas.de> 9/2004       */
/************************************************************************/

#include "../onewire.h"


/* Timing issue when using runtime-bus-selection (!OW_ONE_BUS):
   The master should sample at the end of the 15-slot after initiating
   the read-time-slot. The variable bus-settings need more
   cycles than the constant ones so the delays had to be shortened 
   to achive a 15uS overall delay 
   Setting/clearing a bit in I/O Register needs 1 cyle in OW_ONE_BUS
   but around 14 cyles in configureable bus (us-Delay is 4 cyles per uS) */
uint8_t OW_bit_io ( uint8_t b )
{
  uint8_t sreg;

  sreg=SREG;
  cli();
	
  OW_DIR_OUT(); // drive bus low
	
  delayus(1); // Recovery-Time wuffwuff was 1
  if ( b ) 
    OW_DIR_IN(); // if bit is 1 set bus high (by ext. pull-up)
	
  // wuffwuff delay was 15uS-1 see comment above
  delayus(15-1-OW_CONF_DELAYOFFSET);
	
  if( OW_GET_IN() == 0 ) 
    b = 0;  // sample at end of read-timeslot
	
  delayus(60-15);
  OW_DIR_IN();

  SREG=sreg; // sei();
	
  return b;
}
