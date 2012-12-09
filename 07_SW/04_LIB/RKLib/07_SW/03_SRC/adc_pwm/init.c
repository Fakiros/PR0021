/*-------------------------------------------------------------------------- */
/*	Przetwornik analogowo/cyfrowy					     */
/*	wykorzystuj¹cy wbudowany komparator analogowy i wyjœcie PWM	     */
/*	UWAGA: czas przetwarzania ok. 560 ms !!!			     */
/*	Autor: Robert Krysztof						     */
/* 	WWW:	http://www.8051.prv.pl 					     */
/*	E-mail: robert.krysztof@wp.pl					     */
/* ------------------------------------------------------------------------- */

#include "../adc_pwm.h"

void ADC_PWM_init(void)
{
#ifdef __AVR_AT90S2313__
  sbi(DDRB,3); 			//ustawienie wyjscia PWM
  cbi(PORTB,3); 
  OCR1=0;
#else
  sbi(DDRD,5); 			//ustawienie wyjscia PWM
  cbi(PORTD,5); 
  OCR1A=0;
#endif
  TCCR1A=_BV(COM1A1)|_BV(PWM10);	//ustawienie trybu pracy licznika 1
  TCCR1B=_BV(CS10);	//ustawienie czestotliwosci pracy licznika 1
  TCNT1=0x00;		//ustawienie wartoœci pocz¹tkowej licznika 1
}
