#include "../ps2mouse.h"

void PS2MOUSE_SendByte_proc (unsigned char byte)
{
	PS2MOUSE_ExtInterruptDisable_def;
	PS2MOUSE_ClockSet0_def; // PORT.clock = 0// DDR.clock = 1	
	//PS2MOUSE_Init_proc();
	delayus(100);
	PS2MOUSE_TransInit_proc();
	PS2MOUSE_SendDataByte_char = byte;
	//LiczbaZboczy = 0;
	PS2MOUSE_DataSet0_def; // PORT.data = 0// DDR.data = 1	
	delayus(30);
	PS2MOUSE_SetEdgeSensitivityInt0_def;
	PS2MOUSE_ClockSet1_def; // PORT.clock = 1// DDR.clock = 0	
	PS2MOUSE_ExtInterruptClearFlag_def;
	PS2MOUSE_ExtInterruptEnable_def;
	PS2MOUSE_Send_bit = 1; 
	PS2MOUSE_SendParityBit_bit = 1;
}


/*
PS2MOUSE_State_Iddle,
PS2MOUSE_State_SendReset, 
PS2MOUSE_State_WaitForPacket1, PS2MOUSE_State_WaitForPacket2, PS2MOUSE_State_WaitForPacket3,
PS2MOUSE_State_WaitForPacket4, PS2MOUSE_State_WaitForPacket5, PS2MOUSE_State_WaitForPacket6,
PS2MOUSE_State_WaitForPacket7, PS2MOUSE_State_WaitForPacket8, PS2MOUSE_State_WaitForPacket9,
PS2MOUSE_State_WaitForPacket10, PS2MOUSE_State_WaitForPacket11, PS2MOUSE_State_WaitForPacket12,
PS2MOUSE_State_WaitForPacket13, PS2MOUSE_State_WaitForPacket14, PS2MOUSE_State_WaitForPacket15,
PS2MOUSE_State_WaitForPacket16
*/
void PS2MOUSE_InitProcedure_proc()
{
	//PORTC = 0x01;
	if (PS2MOUSE_StartInitialize_bit)
		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;

	switch (PS2MOUSE_InitializeState_enum)
	{
		case PS2MOUSE_State_Iddle :
			break;
		
		case PS2MOUSE_State_SendReset :
			PS2MOUSE_StartInitialize_bit = 0;
			PS2MOUSE_SendByte_proc(0xFF);
			PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket1;
			break;

		case PS2MOUSE_State_WaitForPacket1 :
			pomocnicza1 = PS2MOUSE_ReadBitCount_char;
			if (   PS2MOUSE_BuffCount_char >= 3           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
			   	   PS2MOUSE_ReadFromBuff_func() == 0xAA   && 
				   PS2MOUSE_ReadFromBuff_func() == 0x00   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0xFF);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket2;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;

		case PS2MOUSE_State_WaitForPacket2 :
			pomocnicza1 = PS2MOUSE_ReadBitCount_char;
			if (   PS2MOUSE_BuffCount_char >= 3           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
			   	   PS2MOUSE_ReadFromBuff_func() == 0xAA   && 
				   PS2MOUSE_ReadFromBuff_func() == 0x00   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0xFF);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket3;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;

		case PS2MOUSE_State_WaitForPacket3 :
			pomocnicza1 = PS2MOUSE_ReadBitCount_char;
			if (   PS2MOUSE_BuffCount_char >= 3           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
			   	   PS2MOUSE_ReadFromBuff_func() == 0xAA   && 
				   PS2MOUSE_ReadFromBuff_func() == 0x00   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0xF3);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket4;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;
		
		case PS2MOUSE_State_WaitForPacket4 :
			pomocnicza1 = PS2MOUSE_ReadBitCount_char;
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0xC8);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket5;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;
		
		case PS2MOUSE_State_WaitForPacket5 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0xF3);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket6;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;
		
		case PS2MOUSE_State_WaitForPacket6 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0x64);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket7;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;

		case PS2MOUSE_State_WaitForPacket7 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0xF3);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket8;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;

		case PS2MOUSE_State_WaitForPacket8 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0x50);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket9;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;

		case PS2MOUSE_State_WaitForPacket9 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0xF2);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket10;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;

		case PS2MOUSE_State_WaitForPacket10 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   )
				{
					unsigned char temp1;
					temp1 = PS2MOUSE_ReadFromBuff_func();
					if (	PS2MOUSE_BuffCount_char == 0x00    &&
							(temp1 == 0x03 || temp1 == 0x00)  )
						{
							if ( temp1 == 0x03 )
								PS2MOUSE_ScrollingType_bit = 1;
							else   
								PS2MOUSE_ScrollingType_bit = 0;
							PS2MOUSE_SendByte_proc(0xE8);
							PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket11;	
						}
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;

		case PS2MOUSE_State_WaitForPacket11 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0x03);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket12;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;

		case PS2MOUSE_State_WaitForPacket12 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0xE6);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket13;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;	
			
		case PS2MOUSE_State_WaitForPacket13 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0xF3);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket14;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;	

		case PS2MOUSE_State_WaitForPacket14 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0x28);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket15;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;

		case PS2MOUSE_State_WaitForPacket15 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_SendByte_proc(0xF4);
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_WaitForPacket16;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;

		case PS2MOUSE_State_WaitForPacket16 :
			if (   PS2MOUSE_BuffCount_char >= 1           &&
				   PS2MOUSE_ReadFromBuff_func() == 0xFA   &&
				   PS2MOUSE_BuffCount_char == 0            )
				{ 
					PS2MOUSE_Initialized_bit = 1;
					PS2MOUSE_InitializeState_enum = PS2MOUSE_State_Iddle;
				}
//			else
//			   		PS2MOUSE_InitializeState_enum = PS2MOUSE_State_SendReset;
			break;


	} // END Switch
// PORTC = 0x00;
  
} 
