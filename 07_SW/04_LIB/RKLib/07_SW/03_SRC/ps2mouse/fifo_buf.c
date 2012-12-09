/*  
******************** FIFO buffer   ************************

Prepared by Rafal Zubkowski, r.zubkowski@poczta.fm

Place to header file (*.h) following code:

#ifndef PS2MOUSE_BuffSize_def
	#define PS2MOUSE_BuffSize_def 64
#endif
volatile unsigned char PS2MOUSE_ReadBuffer_char[PS2MOUSE_BuffSize_def];
volatile unsigned char *PS2MOUSE_BuffInput_pointer, *PS2MOUSE_BuffOutput_pointer;
volatile unsigned char PS2MOUSE_BuffCount_char;

void PS2MOUSE_SaveToBuff_proc (unsigned char c);
unsigned char PS2MOUSE_ReadFromBuff_func (void);

*/

#include "../ps2mouse.h"

void PS2MOUSE_SaveToBuff_proc (unsigned char c)
{
    if (PS2MOUSE_BuffCount_char <= PS2MOUSE_BuffSize_def)                        // If buffer not full
    {
        *PS2MOUSE_BuffInput_pointer = c;                                // Put character into buffer
        PS2MOUSE_BuffInput_pointer++;                                    // Increment pointer
        PS2MOUSE_BuffCount_char++;

        if (PS2MOUSE_BuffInput_pointer >= PS2MOUSE_ReadBuffer_char + PS2MOUSE_BuffSize_def)        // Pointer wrapping
            PS2MOUSE_BuffInput_pointer = PS2MOUSE_ReadBuffer_char;
		
	}
}


unsigned char PS2MOUSE_ReadFromBuff_func (void)
{
  int byte;
  if (PS2MOUSE_BuffCount_char > 0) 
  {     					
    byte = *PS2MOUSE_BuffOutput_pointer;                                // Get byte
    PS2MOUSE_BuffOutput_pointer++;                                    // Increment pointer

    if (PS2MOUSE_BuffOutput_pointer >= PS2MOUSE_ReadBuffer_char + PS2MOUSE_BuffSize_def)            // Pointer wrapping
        PS2MOUSE_BuffOutput_pointer = PS2MOUSE_ReadBuffer_char;
    
    PS2MOUSE_BuffCount_char--;                                    // Decrement buffer count
    return byte;
  }
  else
  {
   // PS2MOUSE_BuffOutput_pointer = PS2MOUSE_BuffInput_pointer;
    return 0;
  }
}
