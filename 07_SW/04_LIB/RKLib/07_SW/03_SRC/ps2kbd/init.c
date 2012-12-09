#include "../ps2kbd.h"

void PS2KBD_init(void)
{
    PS2KBD_inpt =  PS2KBD_buffer;		// Initialize buffer
    PS2KBD_outpt = PS2KBD_buffer;
    PS2KBD_buffcnt = 0;

    MCUCR = 2;					// INT0 interrupt on falling PS2KBD_edge
    PS2KBD_edge = 0;				// 0 = falling PS2KBD_edge  1 = rising PS2KBD_edge
    PS2KBD_bitcount = 11;
}

SIGNAL(PS2KBD_SIG_INTERRUPT)
{
    static unsigned char data;                // Holds the received scan code

    if (!PS2KBD_edge)                                // Routine entered at falling PS2KBD_edge
    {
        if(PS2KBD_bitcount < 11 && PS2KBD_bitcount > 2)    // Bit 3 to 10 is data. Parity bit,
        {                                    // start and stop bits are ignored.
            data = (data >> 1);
            if(PS2KBD_PIN & _BV(PS2KBD_DATA))
                data = data | 0x80;            // Store a '1'
        }

        MCUCR = PS2KBD_rising_edge;   // Set interrupt on rising PS2KBD_edge
        PS2KBD_edge = 1;
        
    } 
    else 
    {                                // Routine entered at rising PS2KBD_edge

        MCUCR = PS2KBD_falling_edge;    // Set interrupt on falling PS2KBD_edge
        PS2KBD_edge = 0;

        if(--PS2KBD_bitcount == 0)    // All bits received
        {
            PS2KBD_decode(data);
            PS2KBD_bitcount = 11;
        }
    }
}


