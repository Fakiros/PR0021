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

#ifndef OW_PORT
void OW_set_bus(volatile uint8_t* port, uint8_t bit)
{
  OW_OUT=port;
  OW_DDR=port-1;
  OW_IN=port-2;
  OW_PIN_MASK=_BV(bit);
  OW_reset();
}
#endif

