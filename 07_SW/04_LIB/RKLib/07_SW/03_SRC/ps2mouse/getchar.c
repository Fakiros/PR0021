#include "../ps2mouse.h"

int PS2MOUSE_ReadFromBuff_func (void)
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
    return 0;
}
