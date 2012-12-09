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

uint8_t OW_byte_wr( uint8_t b )
{
	uint8_t i = 8, j;
	
	do {
		j = OW_bit_io( b & 1 );
		b >>= 1;
		if( j ) b |= 0x80;
	} while( --i );
	
	return b;
}
