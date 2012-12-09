#include "../ps2mouse.h"


void PS2MOUSE_TransInit_proc(void)
{
    PS2MOUSE_BuffInput_pointer =  PS2MOUSE_ReadBuffer_char;		// Initialize buffer
    PS2MOUSE_BuffOutput_pointer = PS2MOUSE_ReadBuffer_char;
    PS2MOUSE_BuffCount_char = 0;
   // PORTD = 0;    

    PS2MOUSE_SetFallingEdgeInt0_def;
  //  GICR |= (1 << INT0);        // enable INT0
    PS2MOUSE_Edge_bit = 0;				// 0 = falling PS2MOUSE_edge  1 = rising PS2MOUSE_edge
    PS2MOUSE_SendBitCount_char = 11;
//  	PS2MOUSE_ReadBitCount_char = 11;
	
}


SIGNAL(PS2MOUSE_SIG_INTERRUPT)
{
   // PORTC = 0x01;
   // static unsigned char PS2MOUSE_SendDataByte_char;                // Holds the received scan code
//    LiczbaZboczy++;
//	if (LiczbaZboczy == 18)
//	    PORTC = 0x04;

	if (PS2MOUSE_Send_bit == 0) // Read Data
	{ 
	    if (!PS2MOUSE_Edge_bit)                                // Routine entered at falling PS2MOUSE_edge
	    {
	        if(PS2MOUSE_ReadBitCount_char < 11 && PS2MOUSE_ReadBitCount_char > 2)    // Bit 3 to 10 is data. Parity bit,
	        {                                    // start and stop bits are ignored.
	            PS2MOUSE_ReadDataByte_char = (PS2MOUSE_ReadDataByte_char >> 1);
	            if(PS2MOUSE_PIN & _BV(PS2MOUSE_DATA))
	                PS2MOUSE_ReadDataByte_char = PS2MOUSE_ReadDataByte_char | 0x80;            // Store a '1'
	        }
	
	        
			PS2MOUSE_SetRisingEdgeInt0_def;
	        PS2MOUSE_Edge_bit = 1;
        
	    } 
	    else 
	    {                                // Routine entered at rising PS2MOUSE_edge
	
			pomocnicza1--;
            PS2MOUSE_SetFallingEdgeInt0_def;
	        PS2MOUSE_Edge_bit = 0;
	
	        if(--PS2MOUSE_ReadBitCount_char == 0)    // All bits received
	        {
			   
	           // PS2MOUSE_ReadBuffer[PS2MOUSE_ReadByteCount++] = PS2MOUSE_ReadDataByte_char;
			//	if (PS2MOUSE_ReadByteCount > 2)
			//	                PS2MOUSE_ReadByteCount = 0; 
			   PS2MOUSE_SaveToBuff_proc(PS2MOUSE_ReadDataByte_char);
				PS2MOUSE_ReadBitCount_char = 11;
	        }
	    }
	}
	else  //Send data
	{ 
	    if (!PS2MOUSE_Edge_bit)          // Routine entered at falling PS2MOUSE_edge
	    {
	        if(PS2MOUSE_SendBitCount_char < 12 && PS2MOUSE_SendBitCount_char > 3)    // Bit 3 to 10 is data. Parity bit,
	        {                                    // start and stop bits are ignored.
	          if(PS2MOUSE_SendDataByte_char & 0x01)
			  	{
			       PS2MOUSE_DataSet1_def
				}
			  else
			  	{
			  	   PS2MOUSE_DataSet0_def
					if (PS2MOUSE_SendParityBit_bit == 0)
				   		PS2MOUSE_SendParityBit_bit = 1;
					else
					    PS2MOUSE_SendParityBit_bit = 0;
				}
			  PS2MOUSE_SendDataByte_char = (PS2MOUSE_SendDataByte_char >> 1);

	        }
			
			else if (PS2MOUSE_SendBitCount_char == 3)
			{	
				if (PS2MOUSE_SendParityBit_bit == 0)
				{ 				    
			  		PS2MOUSE_DataSet0_def	// bit parzystoœci
				}
				else
				{ 				    
			  		PS2MOUSE_DataSet1_def	// bit parzystoœci
				}
			}
			else if (PS2MOUSE_SendBitCount_char == 2)
			{
			  PS2MOUSE_DataSet1_def	// bit konczacy
			}
			
			else if ((PS2MOUSE_SendBitCount_char == 1)) 
			{
             PORTC = 2;
			 if (!(PS2MOUSE_PIN & ~(1 << PS2MOUSE_DATA)))   //sprawdzenie ACK
				PORTC = 6;
	        }
	        PS2MOUSE_SetRisingEdgeInt0_def;   // Set interrupt on rising PS2MOUSE_edge
	        PS2MOUSE_Edge_bit = 1;
        
	    } 
	    else 
	    {                                // Routine entered at rising PS2MOUSE_edge
	
	        PS2MOUSE_SetFallingEdgeInt0_def;
			PS2MOUSE_Edge_bit = 0;
	
	        if(--PS2MOUSE_SendBitCount_char == 0)    // All bits sent
	        {
			    PS2MOUSE_Send_bit = 0;
				PS2MOUSE_SendDataByte_char = 0;
//				PS2MOUSE_ReadBitCount_char = 0;
				
				//PS2MOUSE_put_buff(PS2MOUSE_SendDataByte_char);
	          //  PS2MOUSE_decode(PS2MOUSE_SendDataByte_char);
			    pomocnicza1 = 11;
	            PS2MOUSE_SendBitCount_char = 11;
				PS2MOUSE_ReadBitCount_char = 11; 
	        }
	    }
	}
	// PORTC = 0x00;
}


