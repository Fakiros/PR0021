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

void OW_parasite_disable(void)
{
  OW_OUT_LOW();
  OW_DIR_IN();
}

