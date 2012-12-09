#include "../ps2kbd.h"

void PS2KBD_put_buff(unsigned char c)
{
    if (PS2KBD_buffcnt<PS2KBD_BUFF_SIZE)                        // If buffer not full
    {
        *PS2KBD_inpt = c;                                // Put character into buffer
        PS2KBD_inpt++;                                    // Increment pointer

        PS2KBD_buffcnt++;

        if (PS2KBD_inpt >= PS2KBD_buffer + PS2KBD_BUFF_SIZE)        // Pointer wrapping
            PS2KBD_inpt = PS2KBD_buffer;
    }
}
