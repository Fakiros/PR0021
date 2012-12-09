//*****************************************************************************
// Title		: Software Interrupt-driven UART function library
// Author		: Pascal Stang - Copyright (C) 2002-2003
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include "../uartsw.h"

// UARTSW_ transmit status and data variables
volatile u08 UARTSW_TxBusy;
volatile u16 UARTSW_BaudRateDiv;
volatile u08 UARTSW_TxData;
volatile u08 UARTSW_TxBitNum;

// receive buffer
cBuffer UARTSW_RxBuffer;               ///< UARTSW_ receive buffer
// automatically allocate space in ram for each buffer
static char UARTSW_RxData[UARTSW__RX_BUFFER_SIZE];
// UARTSW_ receive status and data variables
volatile u08 UARTSW_RxStartBit;
volatile u08 UARTSW_RxData;
volatile u08 UARTSW_RxBitNum;

// functions

void UARTSW_TxBitService(void)
{
	if(UARTSW_TxBitNum)
	{
		// there are bits still waiting to be transmitted
		if(UARTSW_TxBitNum > 1)
		{
			// transmit data bits (inverted, LSB first)
			if( !(UARTSW_TxData & 0x01) )
				sbi(PORTD, PD5);
			else
				cbi(PORTD, PD5);
			// shift bits down
			UARTSW_TxData = UARTSW_TxData>>1;
		}
		else
		{
			// transmit stop bit
			cbi(PORTD, PD5);
		}
		// schedule the next bit
		outw(OCR1A, inw(OCR1A) + UARTSW_BaudRateDiv);
		// count down
		UARTSW_TxBitNum--;
	}
	else
	{
		// transmission is done
		// clear busy flag
		UARTSW_TxBusy = FALSE;
	}
}

void UARTSW_RxBitService(void)
{
	// this function runs on either:
	// - a rising edge interrupt
	// - OC1B
	if(!UARTSW_RxStartBit)
	{
		// this is a start bit
		// disable ICP interrupt
		cbi(TIMSK, TICIE1);
		// schedule data bit sampling 1.5 bit periods from now
		outw(OCR1B, inw(TCNT1) + UARTSW_BaudRateDiv + UARTSW_BaudRateDiv/2);
		// clear OC1B interrupt flag
		sbi(TIFR, OCF1B);
		// enable OC1B interrupt
		sbi(TIMSK, OCIE1B);
		// set start bit flag
		UARTSW_RxStartBit = TRUE;
		// reset bit counter
		UARTSW_RxBitNum = 0;
		// reset data
		UARTSW_RxData = 0;
	}
	else
	{
		// start bit has already been received
		// we're in the data bits
		
		// shift data byte to make room for new bit
		UARTSW_RxData = UARTSW_RxData>>1;

		// sample the data line
		if( !(inb(PIND) & (1<<6)) )
		{
			// serial line is low
			// record '1' bit (data inverted)
			UARTSW_RxData |= 0x80;
		}

		// increment bit counter
		UARTSW_RxBitNum++;
		// schedule next bit sample
		outw(OCR1B, inw(OCR1B) + UARTSW_BaudRateDiv);

		// check if we have a full byte
		if(UARTSW_RxBitNum >= 8)
		{
			// save data in receive buffer
			bufferAddToEnd(&UARTSW_RxBuffer, UARTSW_RxData);
			// disable OC1B interrupt
			cbi(TIMSK, OCIE1B);
			// clear ICP interrupt flag
			sbi(TIFR, ICF1);
			// enable ICP interrupt
			sbi(TIMSK, TICIE1);
			// clear start bit flag
			UARTSW_RxStartBit = FALSE;
		}
	}
}

//! enable and initialize the software uart
void UARTSW_init()
{
    // initialize the buffers
	bufferInit(&UARTSW_RxBuffer, UARTSW_RxData, UARTSW__RX_BUFFER_SIZE);
	// initialize the ports
	sbi(DDRD, 5);
	cbi(DDRD, 6);
	cbi(PORTD, 6);
	// initialize baud rate
	UARTSW_BaudRateDiv = (u16)((F_CPU+(UART_BAUD/2L))/(UART_BAUD*1L));
	// set timer prescaler
	timer1SetPrescaler(TIMER_CLK_DIV1);
	// attach TxBit service routine to OC1A
	timerAttach(TIMER1OUTCOMPAREA_INT, UARTSW_TxBitService);
	// attach RxStart service routine to ICP
	timerAttach(TIMER1INPUTCAPTURE_INT, UARTSW_RxBitService);
	// attach RxBit service routine to OC1B
	timerAttach(TIMER1OUTCOMPAREB_INT, UARTSW_RxBitService);
	// enable OC1A interrupt
	sbi(TIMSK, OCIE1A);
	// enable ICP interrupt
	sbi(TIMSK, TICIE1);
	// trigger on rising edge
	sbi(TCCR1B, ICES1);
	// turn on interrupts
	sei();
}

