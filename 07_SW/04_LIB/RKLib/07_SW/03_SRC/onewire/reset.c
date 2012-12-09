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

uint8_t OW_reset(void)
{
  uint8_t err;
  uint8_t sreg;

  OW_OUT_LOW(); // disable internal pull-up (maybe on from parasite)
  OW_DIR_OUT(); // pull OW-Pin low for 480us
	
  delayus(480);
	
  sreg=SREG;
  cli();
	
  // set Pin as input - wait for clients to pull low
  OW_DIR_IN(); // input
	
  delayus(66);
  err = OW_GET_IN();		// no presence detect
// nobody pulled to low, still high
	
  SREG=sreg; // sei()
	
// after a delay the clients should release the line
// and input-pin gets back to high due to pull-up-resistor
  delayus(480-66);
  if( OW_GET_IN() == 0 )		// short circuit
    err = 1;
	
  return err;
}
