/** \file spi.h \brief Sterownik interfejsu SPI. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
//*****************************************************************************
//
// File Name	: 'spi.h'
// Title	: SPI interface driver
// Author	: Pascal Stang - Copyright (C) 2000-2002
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
/** \defgroup spi Interfejs SPI
 
 Zawiera funkcje obs³ugi wbudowanego interfejsu SPI
 \code
 #include "spi.h"
 \endcode 
*/

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "global.h"

/*@{*/

// Define the SPI_USEINT key if you want SPI bus operation to be
// interrupt-driven.  The primary reason for not using SPI in
// interrupt-driven mode is if the SPI send/transfer commands
// will be used from within some other interrupt service routine
// or if interrupts might be globally turned off due to of other
// aspects of your program
//
// Comment-out or uncomment this line as necessary
//#define SPI_USEINT


/// Zanacznik wskazuj¹cy, ¿e zakoñczy³ siê transfer na SPI
extern volatile u08 SPI_transfer_complete;

// function prototypes

/// Inicjuje interfejs SPI
void SPI_init(void);

/**
 Czeka na gotowoœæ interfejsu SPI i wysy³a pojedynczy bajt.
 \param data dane do wys³ania
*/
void SPI_send_byte(u08 data);

/**
 Czeka na gotowoœæ interfejsu SPI i wysy³a pojedynczy bajt.
 \param data dane do wys³ania
 \return odebrany bajt podczas transmisji
*/
u08 SPI_transfer_byte(u08 data);

/**
 Czeka na gotowoœæ interfejsu SPI i wysy³a s³owo 16 bitowe.
 \param data dane do wys³ania
 \return odebrane s³owo 16 bitowe podczas transmisji
*/
u16 SPI_transfer_word(u16 data);

/*@}*/

#endif
