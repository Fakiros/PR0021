#include "../debug.h"

void DEBUG_hextable(u08 *buffer, u16 length)
{
  u08 i;
  u16 j;
  u08 s;

  // print the low order address indicies and ASCII header
  UART_putstr_P(PSTR("\r\n     "));
  for(i=0; i<0x10; i++)
  {
    UART_puthexU08(i);
    UART_putchar(' ');
  }
  UART_putchar(' ');
  for(i=0; i<0x0a; i++) UART_putchar(i+'0');
  for(i=0; i<6; i++)    UART_putchar(i+'A');
  UART_putstr_P(PSTR("\r\n     "));
  for(i=0; i<47; i++)   UART_putchar('-');
  UART_putstr_P(PSTR("  ---- ASCII -----\r\n"));  

  // print the data
  for(j=0; j<((length+15)>>4); j++)
  {
    // print the high order address index for this line
    UART_puthexU16(j<<4);
    UART_putchar(' ');

    // print the hex data
    for(i=0; i<0x10; i++)
    {
      // be nice and print only up to the exact end of the data
      if( ((j<<4)+i) < length)
      {
        // print hex byte
        UART_puthexU08(buffer[(j<<4)+i]);
        UART_putchar(' ');
      }
      else
      {
        // we're past the end of the data's length
        // print spaces
        UART_putstr_P(PSTR("   "));
      }
    }
    
    // leave some space
    UART_putchar(' ');

    // print the ascii data
    for(i=0; i<0x10; i++)
    {
      // be nice and print only up to the exact end of the data
      if( ((j<<4)+i) < length)
      {
        // get the character
        s = buffer[(j<<4)+i]; 
        // make sure character is printable
        if(s >= 0x20)
          UART_putchar(s);
        else
          UART_putchar('.');
      }
      else
      {
        // we're past the end of the data's length
        // print a space
        UART_putchar(' ');
      }
    }
    UART_putstr_P(PSTR("\r\n"));
  }
  UART_putstr_P(PSTR("\r\n"));
}
