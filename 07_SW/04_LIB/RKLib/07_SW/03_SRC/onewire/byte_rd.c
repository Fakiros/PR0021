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


uint8_t OW_byte_rd( void )
{
  // read by sending 0xff (a dontcare?)
  return OW_byte_wr( 0xFF ); 
}

