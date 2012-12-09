//*****************************************************************************
// Title		: Software Interrupt-driven UART function library
// Author		: Pascal Stang - Copyright (C) 2002-2003
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include "../uartsw.h"

//! returns the receive buffer structure 
cBuffer* UARTSW_GetRxBuffer(void)
{
	// return rx buffer pointer
	return &UARTSW_RxBuffer;
}
