#include "../ps2mouse.h"

void PS2MOUSE_SaveToBuff_proc (unsigned char c)
{
    if (PS2MOUSE_buffcnt<PS2MOUSE_BUFF_SIZE)                        // If buffer not full
    {
        *PS2MOUSE_inpt = c;                                // Put character into buffer
        PS2MOUSE_inpt++;                                    // Increment pointer

        PS2MOUSE_buffcnt++;

        if (PS2MOUSE_inpt >= PS2MOUSE_buffer + PS2MOUSE_BUFF_SIZE)        // Pointer wrapping
            PS2MOUSE_inpt = PS2MOUSE_buffer;
    }
}
