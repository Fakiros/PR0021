#include "../spi.h"

// global variables
volatile u08 SPI_transfer_complete;

// SPI interrupt service handler
#ifdef SPI_USEINT
SIGNAL(SIG_SPI)
{
	SPI_transfer_complete = TRUE;
}
#endif

// access routines
void SPI_init()
{
int tmp;
#ifdef __AVR_ATmega128__
	// setup SPI I/O pins
	sbi(PORTB, 1);	// set SCK hi
	sbi(DDRB, 1);	// set SCK as output
	cbi(DDRB, 3);	// set MISO as input
	sbi(DDRB, 2);	// set MOSI as output
	sbi(DDRB, 0);	// SS must be output for Master mode to work
#else
	// setup SPI I/O pins
	sbi(PORTB, 7);	// set SCK hi
	sbi(DDRB, 7);	// set SCK as output
	cbi(DDRB, 6);	// set MISO as input
	sbi(DDRB, 5);	// set MOSI as output
	sbi(DDRB, 4);	// SS must be output for Master mode to work
#endif
	
	// setup SPI interface :
	// clock = f/16
	// select clock phase positive-going in middle of data
	// master mode
	// enable SPI
	SPCR = (1<<MSTR)|(1<<SPE)|(1<<SPR0);
	
	// some other possible configs
	//outp((1<<CPHA)|(1<<CPOL)|(1<<MSTR)|(1<<SPE)|(1<<SPR0)|(1<<SPR1), SPCR );
	//outp((1<<CPHA)|(1<<MSTR)|(1<<SPE)|(1<<SPR0), SPCR );
	
	// clear status
	tmp=SPSR;
	SPI_transfer_complete = 1;

	// enable SPI interrupt
	#ifdef SPI_USEINT
	sbi(SPCR, SPIE);
	#endif
}
