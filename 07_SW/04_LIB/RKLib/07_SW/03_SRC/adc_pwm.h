/** \file adc_pwm.h \brief Obs³uga programowego przetwornika analogowo/cyfrowego. */
/*-------------------------------------------------------------------------- */
/*	Przetwornik analogowo/cyfrowy					     */
/*	wykorzystuj¹cy wbudowany komparator analogowy i wyjœcie PWM	     */
/*	UWAGA: czas przetwarzania ok. 560 ms !!!			     */
/*	Autor: Robert Krysztof						     */
/* 	WWW:	http://www.8051.prv.pl 					     */
/*	E-mail: robert.krysztof@wp.pl					     */
/* ------------------------------------------------------------------------- */

#ifndef __ADC_PWM_H__
#define __ADC_PWM_H__

#include <inttypes.h>
#include <avr/io.h>
#include "config.h"
#include "global.h"
#include "delay.h"

/*@{*/

#ifdef __AVR_AT90S2313__
#define PWM_out(value)	OCR1=value
#else
#define PWM_out(value)	OCR1A=value
#endif

/**
	Inicjalizacja przetwornika
*/
void ADC_PWM_init(void);

/**
	Odczyt wartoœci z przetwornika
	UWAGA: zajmuje czas ok. 560ms !!!
*/
u08 ADC_PWM_read(void);

/*@}*/

#endif // __ADC_PWM_H__
