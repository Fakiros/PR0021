/** \file adc.h \brief Obs³uga wbudowanego przetwornika analogowo/cyfrowego. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
//*****************************************************************************
//
// Title		: Analog-to-digital converter functions
// Author		: Pascal Stang - Copyright (C) 2002
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

/** \defgroup adc Przetwornik ADC
 
 Zawiera funkcje obs³ugi wbudowanego przetwornika analogowo/cyfrowego.
 \code
 #include "adc.h"
 \endcode 
*/

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "config.h"
#include "global.h"

#ifndef ADC_H
#define ADC_H

/*@{*/

// defines

// ADC_clock prescaler select
//		*selects how much the CPU clock frequency is divided
//		to create the ADC_ clock frequency
//		*lower division ratios make conversion go faster
//		*higher division ratios make conversions more accurate
#define ADC_PRESCALE_DIV2		0x00	///< 0x01,0x00 -> CPU clk/2
#define ADC_PRESCALE_DIV4		0x02	///< 0x02 -> CPU clk/4
#define ADC_PRESCALE_DIV8		0x03	///< 0x03 -> CPU clk/8
#define ADC_PRESCALE_DIV16		0x04	///< 0x04 -> CPU clk/16
#define ADC_PRESCALE_DIV32		0x05	///< 0x05 -> CPU clk/32
#define ADC_PRESCALE_DIV64		0x06	///< 0x06 -> CPU clk/64
#define ADC_PRESCALE_DIV128		0x07	///< 0x07 -> CPU clk/128
/// Wartoœæ domyœlna
#define ADC_PRESCALE			ADC_PRESCALE_DIV64
/// Nie nale¿y zmieniaæ
#define ADC_PRESCALE_MASK		0x07

// ADC voltage reference select
//		*this determines what is used as the
//		full-scale voltage point for ADC_ conversions
#define ADC_REFERENCE_AREF		0x00	///< 0x00 -> AREF pin, wewnêtrzne VREF wy³¹czone
#define ADC_REFERENCE_AVCC		0x01	///< 0x01 -> AVCC pin, wewnêtrzne VREF wy³¹czone
#define ADC_REFERENCE_RSVD		0x02	///< 0x02 -> Zarezerwowane
#define ADC_REFERENCE_256V		0x03	///< 0x03 -> Wewnêtrzne 2.56V VREF
/// Wartoœæ domyœlna
#define ADC_REFERENCE			ADC_REFERENCE_AVCC
/// Nie nale¿y zmieniaæ
#define ADC_REFERENCE_MASK		0xC0

/// Maska bitowa multipleksera kana³ów
#define ADC_MUX_MASK			0x1F

// channel defines (for reference and use in code)
// these channels supported by all AVRs with ADC_
#define ADC_CH_ADC0				0x00
#define ADC_CH_ADC1				0x01
#define ADC_CH_ADC2				0x02
#define ADC_CH_ADC3				0x03
#define ADC_CH_ADC4				0x04
#define ADC_CH_ADC5				0x05
#define ADC_CH_ADC6				0x06
#define ADC_CH_ADC7				0x07
#define ADC_CH_122V				0x1E	///< Napiêcie odniesienia 1.22V
#define ADC_CH_AGND				0x1F	///< AGND
// these channels supported only in ATmega128
// differential with gain
#define ADC_CH_0_0_DIFF10X		0x08
#define ADC_CH_1_0_DIFF10X		0x09
#define ADC_CH_0_0_DIFF200X		0x0A
#define ADC_CH_1_0_DIFF200X		0x0B
#define ADC_CH_2_2_DIFF10X		0x0C
#define ADC_CH_3_2_DIFF10X		0x0D
#define ADC_CH_2_2_DIFF200X		0x0E
#define ADC_CH_3_2_DIFF200X		0x0F
// differential
#define ADC_CH_0_1_DIFF1X		0x10
#define ADC_CH_1_1_DIFF1X		0x11
#define ADC_CH_2_1_DIFF1X		0x12
#define ADC_CH_3_1_DIFF1X		0x13
#define ADC_CH_4_1_DIFF1X		0x14
#define ADC_CH_5_1_DIFF1X		0x15
#define ADC_CH_6_1_DIFF1X		0x16
#define ADC_CH_7_1_DIFF1X		0x17

#define ADC_CH_0_2_DIFF1X		0x18
#define ADC_CH_1_2_DIFF1X		0x19
#define ADC_CH_2_2_DIFF1X		0x1A
#define ADC_CH_3_2_DIFF1X		0x1B
#define ADC_CH_4_2_DIFF1X		0x1C
#define ADC_CH_5_2_DIFF1X		0x1D

// compatibility for new Mega processors
// ADCSR hack apparently no longer necessary in new AVR-GCC
#ifdef ADCSRA
#ifndef ADCSR
	#define ADCSR	ADCSRA
#endif
#endif
#ifdef ADATE
	#define ADFR	ADATE
#endif

//! Znacznik u¿ywany do wskazania, ¿e konwersja siê zakoñczy³a
extern volatile unsigned char ADC_complete_flag;

/** 
 Inicjalizacja ADC
*/
void ADC_init(void);

/** 
 Wy³¹cza ADC
*/
void ADC_off(void);

/** 
 Ustawia dzielnik zegara taktuj¹cego przetwornik ADC,
 ta funkcja jest automatycznie wywo³ywana przez ADC_init() z wartoœci¹ domyœln¹.
*/
void ADC_set_prescaler(unsigned char prescale);

/** 
 Wybiera Ÿród³o napiêcia odniesienia u¿ywane przez ADC,
 ta funkcja jest automatycznie wywo³ywana przez ADC_init() z wartoœci¹ domyœln¹.
*/
void ADC_set_reference(unsigned char ref);

/** 
 Wybiera kana³ wejœciowy przetwornika ADC.
*/
void ADC_set_channel(unsigned char ch);

/** 
 Rozpoczyna konwersjê z wyranego kana³u wejœciowego przetwornika ADC.
*/
void ADC_start_convert(void);

/** 
 Sprawdza czy konwersja siê zakoñczy³a.
 \return 1 jeœli siê zakoñczy³a
*/
u08 ADC_is_complete(void);

/** 
 Rozpoczyna konwersjê z wyranego kana³u wejœciowego przetwornika ADC.
 \param ch kana³ wejœciowy.
 \return 10 bitowa wartoœæ.
*/
unsigned short ADC_convert_10bit(unsigned char ch);

/** 
 Rozpoczyna konwersjê z wyranego kana³u wejœciowego przetwornika ADC.
 \param ch kana³ wejœciowy.
 \return 8 bitowa wartoœæ.
*/
unsigned char ADC_convert_8bit(unsigned char ch);

/*@}*/

#endif
