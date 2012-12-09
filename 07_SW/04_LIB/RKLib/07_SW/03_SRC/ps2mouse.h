#ifndef __MS_INCLUDED
#define __MS_INCLUDED

/*! \file ps2mouse.h \brief Obs³uga klawiatury PS2 i AT. */


/** \defgroup ps2MOUSE Obs³uga klawiatury PS2 i AT
 
 Obs³uga klawiatury PS2 i AT.
 
 \code
 #include "ps2mouse.h"
 \endcode 
 
 \ref config:
 
 W pliku \c "config.h" znajduj¹cym siê w katalogu projektu nale¿y umieœciæ nastêpuj¹c¹ zawartoœæ:
 
\code
#define PS2MOUSE_PORT	PORTD
#define PS2MOUSE_CLOCK	2
#define PS2MOUSE_DATA	3
\endcode
 
 \note Z uwagi na wykorzystywanie przez prez procedury przewañ zewnêtrzych 0 lub 1 nie nale¿y zmieniaæ
 przypisania PS2MOUSE_PORT natomiast PS2MOUSE_CLOCK mo¿e przyjmowaæ wartoœci 2 lub 3 - biblioteka automatycznie
 przypisze odpowiednie przerwanie. Jedyn¹ wartoœci¹, któr¹ mo¿na w miarê bezpiecznie zmieniaæ jest przypisanie
 koncówki PS2MOUSE_DATA.
 
 \par Wykorzytywane przerwania:
 \c SIG_INTERRUPT0 lub \c SIG_INTERRUPT1

*/

// PS2 mouse communication routines

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>		// definicje SIGNAL, INTERRUPT
#include <avr/pgmspace.h>
#include "global.h"
#include "delay.h"
#include "config.h"

#define PS2MOUSE_PIN	PIN(PS2MOUSE_PORT)
#define PS2MOUSE_DDR    DDR(PS2MOUSE_PORT)

/********************************************************************
**********  Registers definitions for different uC  *****************
*********************************************************************/

#if  defined(__AVR_ATmega128__)||defined(__AVR_ATmega64__)

	//#if (PS2MOUSE_PORT == 0x12)
		#if (PS2MOUSE_CLOCK == 0)
			#define PS2MOUSE_SIG_INTERRUPT	SIG_INTERRUPT0
			#define PS2MOUSE_ExtInterruptEnable_def		EIMSK |= (1 << INT0)       // enable INT0
			#define PS2MOUSE_ExtInterruptDisable_def	EIMSK &= ~(1 << INT0)        // disable INT0
			#define PS2MOUSE_ExtInterruptClearFlag_def	EIFR |= (1 << INTF0) // clear INT0 interrupt flag 
			#define PS2MOUSE_SetFallingEdgeInt0_def		EICRA &= ~(1 << ISC00) 
			#define PS2MOUSE_SetRisingEdgeInt0_def		EICRA |= (1 << ISC00) 
			#define PS2MOUSE_SetEdgeSensitivityInt0_def	EICRA |= (1 << ISC01)
		#elif (PS2MOUSE_CLOCK==3)
			#define PS2MOUSE_SIG_INTERRUPT SIG_INTERRUPT1
			#define PS2MOUSE_ExtInterruptEnable_def		EIMSK |= (1 << INT1)       // enable INT0
			#define PS2MOUSE_ExtInterruptDisable_def	EIMSK &= ~(1 << INT1)        // disable INT0
			#define PS2MOUSE_ExtInterruptClearFlag_def	EIFR |= (1 << INTF1) // clear INT0 interrupt flag 
			#define PS2MOUSE_SetFallingEdgeInt0_def		EICRA &= ~(1 << ISC10) 
			#define PS2MOUSE_SetRisingEdgeInt0_def		EICRA |= (1 << ISC10) 
			#define PS2MOUSE_SetEdgeSensitivityInt0_def	EICRA |= (1 << ISC11)
		#else
			#error "PS2MOUSE_CLOCK musi byc ustawiony 2 lub 3 (INT0 lub INT1)"
		#endif
	//#else
	//#  error "PS2MOUSE_PORT musi byc ustawiony na PORTD"
	//#endif


#elif defined(__AVR_ATmega162__)
	//#if (PS2MOUSE_PORT == 0x12)
		#if (PS2MOUSE_CLOCK == 2)
			#define PS2MOUSE_SIG_INTERRUPT SIG_INTERRUPT0
			#define PS2MOUSE_rising_edge	3
			#define PS2MOUSE_falling_edge	2
			#define PS2MOUSE_ExtInterruptEnable_def		GICR |= (1 << INT0)        // enable INT0
			#define PS2MOUSE_ExtInterruptDisable_def	GICR &= ~(1 << INT0)        // disable INT0
			#define PS2MOUSE_ExtInterruptClearFlag_def	GIFR |= (1 << INTF0) // clear INT0 interrupt flag 
		#elif (PS2MOUSE_CLOCK==3)
			#define PS2MOUSE_SIG_INTERRUPT SIG_INTERRUPT1
			#define PS2MOUSE_rising_edge	8
			#define PS2MOUSE_falling_edge	12
			#define PS2MOUSE_ExtInterruptEnable_def		GICR |= (1 << INT1)        // enable INT0
			#define PS2MOUSE_ExtInterruptDisable_def	GICR &= ~(1 << INT1)        // disable INT0
			#define PS2MOUSE_ExtInterruptClearFlag_def	GIFR |= (1 << INTF1) // clear INT0 interrupt flag
		#else
			#error "PS2MOUSE_CLOCK musi byc ustawiony 2 lub 3 (INT0 lub INT1)"
		#endif
	//#else
	//#  error "PS2MOUSE_PORT musi byc ustawiony na PORTD"
	//#endif
	#define PS2MOUSE_SetFallingEdgeInt0_def		MCUCR |= PS2MOUSE_falling_edge; MCUCR &= ~PS2MOUSE_rising_edge
	#define PS2MOUSE_SetRisingEdgeInt0_def		MCUCR |= PS2MOUSE_rising_edge; MCUCR &= ~PS2MOUSE_falling_edge
	#define PS2MOUSE_SetEdgeSensitivityInt0_def

#else
    #error  "Zdefiniuj poprawne przerwania dla procesora innego niz ATMEga 128/64/162, plik: ps2mouse.h"
#endif


// PORT.data = 0// DDR.data = 1	
#define PS2MOUSE_DataSet0_def	PS2MOUSE_PORT &=  ~(1 << PS2MOUSE_DATA); PS2MOUSE_DDR |= (1 << PS2MOUSE_DATA);
#define PS2MOUSE_DataSet1_def	PS2MOUSE_DDR &= ~(1 << PS2MOUSE_DATA); PS2MOUSE_PORT |= (1 << PS2MOUSE_DATA); 
#define PS2MOUSE_ClockSet0_def	PS2MOUSE_PORT &=  ~(1 << PS2MOUSE_CLOCK); PS2MOUSE_DDR |= (1 << PS2MOUSE_CLOCK);
#define PS2MOUSE_ClockSet1_def	PS2MOUSE_DDR &= ~(1 << PS2MOUSE_CLOCK); PS2MOUSE_PORT |= (1 << PS2MOUSE_CLOCK);

// #define PS2MOUSE_CLOCK_set_0  PS2MOUSE_DDR|=(1<<PS2MOUSE_CLOCK);PS2MOUSE_DDR|=(1<<PS2MOUSE_CLOCK);
// #define PS2MOUSE_CLOCK_set_1  PS2MOUSE_DDR&=~(1<<PS2MOUSE_CLOCK);PS2MOUSE_DDR&=~(1<<PS2MOUSE_CLOCK);




//unsigned char  PS2MOUSE_bitcount;                // 0 = neg.  1 = pos.



//volatile unsigned char PS2MOUSE_send;
//volatile unsigned char PS2MOUSE_databyte;
//volatile unsigned char PS2MOUSE_ReadDataByte, PS2MOUSE_ReadBitCount, PS2MOUSE_ReadByteCount;
//volatile unsigned char LiczbaZboczy;
//volatile unsigned char PS2MOUSE_ReadBuffer[3];

/* --------------------          PS2MOUSE Flags Byte      -------------------------- */
typedef  struct 
{
	 unsigned char flag0:1;
	 unsigned char flag1:1;
	 unsigned char flag2:1;
	 unsigned char flag3:1;
	 unsigned char flag4:1;
	 unsigned char flag5:1;
	 unsigned char flag6:1;
	 unsigned char flag7:1;
}PS2MOUSE_Bit_type;

typedef union PS2MOUSE_ByteBit_union
{
	PS2MOUSE_Bit_type bit;
	unsigned char byte;
}PS2MOUSE_ByteBit_type;

volatile PS2MOUSE_ByteBit_type PS2MOUSE_Status_byte;

#define PS2MOUSE_Edge_bit PS2MOUSE_Status_byte.bit.flag0
#define PS2MOUSE_Send_bit PS2MOUSE_Status_byte.bit.flag1
#define PS2MOUSE_Initialized_bit PS2MOUSE_Status_byte.bit.flag2
#define PS2MOUSE_StartInitialize_bit PS2MOUSE_Status_byte.bit.flag3
#define PS2MOUSE_SendParityBit_bit PS2MOUSE_Status_byte.bit.flag4
#define PS2MOUSE_ScrollingType_bit PS2MOUSE_Status_byte.bit.flag5 // 1 - scroling type mouse detected , 0 -standard mouse detected

/*  ----     PS2MOUSE  state variable for mouse Initialization    ----   */

typedef enum  {

PS2MOUSE_State_Iddle,
PS2MOUSE_State_SendReset, 
PS2MOUSE_State_WaitForPacket1, PS2MOUSE_State_WaitForPacket2, PS2MOUSE_State_WaitForPacket3,
PS2MOUSE_State_WaitForPacket4, PS2MOUSE_State_WaitForPacket5, PS2MOUSE_State_WaitForPacket6,
PS2MOUSE_State_WaitForPacket7, PS2MOUSE_State_WaitForPacket8, PS2MOUSE_State_WaitForPacket9,
PS2MOUSE_State_WaitForPacket10, PS2MOUSE_State_WaitForPacket11, PS2MOUSE_State_WaitForPacket12,
PS2MOUSE_State_WaitForPacket13, PS2MOUSE_State_WaitForPacket14, PS2MOUSE_State_WaitForPacket15,
PS2MOUSE_State_WaitForPacket16

} PS2MOUSE_InitializeState_type;

PS2MOUSE_InitializeState_type PS2MOUSE_InitializeState_enum;

/*  --------  PS2MOUSE read packed declaration  ---------    */

typedef  struct 
{
	 unsigned char LeftBtn_bit:1;
	 unsigned char RightBtn_bit:1;
	 unsigned char MiddleBtn_bit:1;
	 unsigned char Always1_bit:1;
	 unsigned char XSignBit_bit:1;
	 unsigned char YSignBit_bit:1;
	 unsigned char XOverflow_bit:1;
	 unsigned char YOverflow_bit:1;
	 signed char XMovement_byte;
	 signed char YMovement_byte;
	 signed char ZMovement_byte;
}PS2MOUSE_DecodePacket_type;

typedef union PS2MOUSE_ReadPacket_union
{
	PS2MOUSE_DecodePacket_type Decode;
	unsigned char Byte[4];
}PS2MOUSE_ReadPacket_type;

PS2MOUSE_ReadPacket_type PS2MOUSE_ReadPacket_pack;


/* ----------------------------------------------------------------------------- */
volatile unsigned char PS2MOUSE_ReadDataByte_char, PS2MOUSE_ReadBitCount_char;
//volatile unsigned char PS2MOUSE_SendDataByte_char, PS2MOUSE_SendBitCount_char;

#define PS2MOUSE_SendDataByte_char PS2MOUSE_ReadDataByte_char
#define PS2MOUSE_SendBitCount_char PS2MOUSE_ReadBitCount_char

int PS2MOUSE_XPosition_int, PS2MOUSE_YPosition_int, PS2MOUSE_ZPosition_int;

volatile unsigned char pomocnicza1;

void PS2MOUSE_TransInit_proc(void);
void PS2MOUSE_SendByte_proc (unsigned char byte);
void PS2MOUSE_InitProcedure_proc(void);




/************   FIFO Buffer declarations  ******************************/

#ifndef PS2MOUSE_BuffSize_def
	#define PS2MOUSE_BuffSize_def 32
#endif

volatile unsigned char PS2MOUSE_ReadBuffer_char[PS2MOUSE_BuffSize_def];
volatile unsigned char *PS2MOUSE_BuffInput_pointer, *PS2MOUSE_BuffOutput_pointer;
volatile unsigned char PS2MOUSE_BuffCount_char; // quantity words in buffer

void PS2MOUSE_SaveToBuff_proc (unsigned char c);
unsigned char PS2MOUSE_ReadFromBuff_func (void);
/*--------------------------------------------------------------------- */

 
#endif /* __MS_INCLUDED */ 

