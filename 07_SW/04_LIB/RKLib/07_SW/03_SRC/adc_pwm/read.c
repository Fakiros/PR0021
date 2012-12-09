/*-------------------------------------------------------------------------- */
/*	Przetwornik analogowo/cyfrowy					     */
/*	wykorzystuj¹cy wbudowany komparator analogowy i wyjœcie PWM	     */
/*	UWAGA: czas przetwarzania ok. 560 ms !!!			     */
/*	Autor: Robert Krysztof						     */
/* 	WWW:	http://www.8051.prv.pl 					     */
/*	E-mail: robert.krysztof@wp.pl					     */
/* ------------------------------------------------------------------------- */

#include "../adc_pwm.h"

u08 ADC_PWM_read(void)		// przetwornik AC
{
  int8_t i;
  uint8_t analog=0;		// inicjalizuj zmienn¹ analog na 0
  for(i=7; i>=0; i--)		// zacznij od najstarszego bitu
  {
    analog|=_BV(i);		// ustaw i-ty bit zmiennej analog
    PWM_out(analog);		// wyprowadŸ wartoœæ na wyjœcie PWM
    delayms(70);		// poczekaj na ustabilizowan¹ wartoœæ napiêcia
    if (bit_is_clear(ACSR,ACO))	// jeœli wartoœæ napiêcia by³a za niska
      analog&=~_BV(i);		// to wyzeruj ustawiony wszeœniej bit
  }
  return analog; // zwraca wartosc nap. w zakr. od 0 do 255
}
