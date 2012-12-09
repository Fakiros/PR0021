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


uint8_t OW_rom_search( uint8_t diff, uint8_t *id )
{
  uint8_t i, j, next_diff;
  uint8_t b;
	
  if ( OW_reset() ) 
    return OW_PRESENCE_ERR;			// error, no device found
  
  OW_byte_wr( OW_SEARCH_ROM );			// ROM search command
  
  next_diff = OW_LAST_DEVICE;			// unchanged on last device
  i = OW_ROMCODE_SIZE * 8;			// 8 bytes
  do 
  {
    j = 8;					// 8 bits
    do 
    {
      b = OW_bit_io( 1 );			// read bit
      if( OW_bit_io( 1 ) ) 
      {			// read complement bit
	if( b )					// 11
	  return OW_DATA_ERR;			// data error
      }
      else 
      {
	if( !b ) 
	{					// 00 = 2 devices
	  if( diff > i || ((*id & 1) && diff != i) ) 
	  {
	    b = 1;				// now 1
	    next_diff = i;			// next pass 0
	  }
	}
      }
      OW_bit_io( b );     			// write bit
      *id >>= 1;
      if( b ) *id |= 0x80;			// store bit
      i--;
    } 
    while( --j );
    id++;					// next byte
  } 
  while( i );
  return next_diff;				// to continue search
}
