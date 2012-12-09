#include "../ps2kbd.h"

int PS2KBD_getchar(void)
{
  int byte;
  if (PS2KBD_buffcnt > 0) 
  {     					
    byte = *PS2KBD_outpt;                                // Get byte
    PS2KBD_outpt++;                                    // Increment pointer

    if (PS2KBD_outpt >= PS2KBD_buffer + PS2KBD_BUFF_SIZE)            // Pointer wrapping
        PS2KBD_outpt = PS2KBD_buffer;
    
    PS2KBD_buffcnt--;                                    // Decrement buffer count
    return byte;
  }
  else
    return 0;
}
