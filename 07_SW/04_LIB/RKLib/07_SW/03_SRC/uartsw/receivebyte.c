//*****************************************************************************
// Title		: Software Interrupt-driven UART function library
// Author		: Pascal Stang - Copyright (C) 2002-2003
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include "../uartsw.h"

//! gets a byte (if available) from the uart receive buffer
u08 UARTSW_ReceiveByte(u08* rxData)
{
	// make sure we have a receive buffer
	if(UARTSW_RxBuffer.size)
	{
		// make sure we have data
		if(UARTSW_RxBuffer.datalength)
		{
			// get byte from beginning of buffer
			*rxData = bufferGetFromFront(&UARTSW_RxBuffer);
			return TRUE;
		}
		else
		{
			// no data
			return FALSE;
		}
	}
	else
	{
		// no buffer
		return FALSE;
	}
}
