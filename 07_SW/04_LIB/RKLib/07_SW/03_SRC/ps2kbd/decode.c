#include "../ps2kbd.h"

void PS2KBD_decode(unsigned char sc)
{
    static unsigned char is_up=0, shift = 0, mode = 0;
    unsigned char i;

    if (!is_up)                // Last data received was the up-key identifier
    {
        switch (sc)
        {
          case 0xF0 :        // The up-key identifier
            is_up = 1;
            break;

          case 0x12 :        // Left SHIFT
            shift = 1;
            break;

          case 0x59 :        // Right SHIFT
            shift = 1;
            break;

          case 0x05 :        // F1
            if(mode == 0)
                mode = 1;    // Enter scan code mode
            if(mode == 2)
                mode = 3;    // Leave scan code mode
            break;

          default:
            if(mode == 0 || mode == 3)        // If ASCII mode
            {
                if(!shift)                    // If shift not pressed,
                {                            // do a table look-up
                    for(i = 0; pgm_read_byte(&PS2KBD_unshifted[i][0])!=sc && pgm_read_byte(&PS2KBD_unshifted[i][0]); i++);
                    if (pgm_read_byte(&PS2KBD_unshifted[i][0]) == sc) {
                        PS2KBD_put_buff(pgm_read_byte(&PS2KBD_unshifted[i][1]));
                    }
                } else {                    // If shift pressed
                    for(i = 0; pgm_read_byte(&PS2KBD_shifted[i][0])!=sc && pgm_read_byte(&PS2KBD_shifted[i][0]); i++);
                    if (pgm_read_byte(&PS2KBD_shifted[i][0]) == sc) {
                        PS2KBD_put_buff(pgm_read_byte(&PS2KBD_shifted[i][1]));
                    }
                }
            } else{                            // Scan code mode
                PS2KBD_put_buff(' ');
                PS2KBD_put_buff(' ');
            }
            break;
            
        }
    } else {
        is_up = 0;                            // Two 0xF0 in a row not allowed
        switch (sc)
        {
          case 0x12 :                        // Left SHIFT
            shift = 0;
            break;
            
          case 0x59 :                        // Right SHIFT
            shift = 0;
            break;

          case 0x05 :                        // F1
            if(mode == 1)
                mode = 2;
            if(mode == 3)
                mode = 0;
            break;
          case 0x06 :                        // F2
//            clr();
            break;
        } 
    }    
} 

