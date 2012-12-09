#include "../led7seg.h"
#ifdef LED7SEG_SPI
#include "../spi.h"
#endif

void LED7SEG_out (char kod)	
{
#ifdef LED7SEG_DATA_PORT	// wysy³anie na rejestr przesuwny
  int n;
  for(n=7; n>=0; n--)		// dla bitu n
  {
    if (((kod >> n) & 0x01)==0)	// jezeli wartosc bitu == 0 
      LED7SEG_DATA_LO();	// to wpisz 0 na wej. rejestru 74164
    else
      LED7SEG_DATA_HI();	// w przeciwnym wypadku wpisz 1
    LED7SEG_CLK_LO();		// zegar opada
    LED7SEG_CLK_HI();		// zegar narasta
  }
#elif LED7SEG_SPI		// wysy³anie na rejestr przesuwny przez SPI
  SPI_transfer_byte(kod);
#else				// wysy³anie bezpoœrednie
  LED7SEG_SEGMENTS=kod;
#endif
}
