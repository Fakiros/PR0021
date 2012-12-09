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

void OW_command( uint8_t command, uint8_t *id )
{
  uint8_t i;

  OW_reset();

  if( id ) 
  {
    OW_byte_wr( OW_MATCH_ROM );			// to a single device
    i = OW_ROMCODE_SIZE;
    do 
    {
      OW_byte_wr( *id );
      id++;
    } 
    while( --i );
  } 
  else 
  {
    OW_byte_wr( OW_SKIP_ROM );			// to all devices
  }

  OW_byte_wr( command );
}
