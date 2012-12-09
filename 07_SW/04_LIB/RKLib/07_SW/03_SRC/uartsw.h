//*****************************************************************************
// Title		: Software Interrupt-driven UART function library
// Author		: Pascal Stang - Copyright (C) 2002-2003
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef UARTSW_H
#define UARTSW_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

#include "config.h"
#include "global.h"


// constants/macros/typdefs
#ifndef UARTSW_RX_BUFFER_SIZE
#define UARTSW_RX_BUFFER_SIZE		32
#endif

// functions

//! enable and initialize the software uart
void UARTSW_init(void);
//! returns the receive buffer structure 
cBuffer* UARTSW_GetRxBuffer(void);

void UARTSW_SendByte(u08 data);
u08 UARTSW_ReceiveByte(u08* rxData);

void UARTSW_TxBitService(void);
void UARTSW_RxBitService(void);

#endif
