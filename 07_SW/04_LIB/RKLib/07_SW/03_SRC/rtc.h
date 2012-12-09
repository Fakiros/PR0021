/*! \file rtc.h \brief Zegar czasu rzeczywistego. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** \defgroup rtc Zegar czasu rzeczywistego
 
 Realizacja zegara czasu rzeczywistego w opaciu o Timer2 
 lub w przypadku ATmega103 i ATmega128 o Timer0 (nie testowane!).
 \code
 #include "rtc.h"
 \endcode 
 \note
 U¿ywa przerwania SIG_OVERFLOW2 (wiêkszoœæ przypadków)
\n
 U¿ywa przerwania SIG_OVERFLOW0 (ATmega103 i ATmega128)
 
*/
#ifndef RTC_H
#define RTC_H

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "config.h"
#include "global.h"

/*@{*/

extern struct_time RTC_time;  ///< Zmienna przechowuj¹ca czas

/// Inicjuje zegar czasu rzeczywistego
void RTC_init(void);

/*@}*/

#endif
