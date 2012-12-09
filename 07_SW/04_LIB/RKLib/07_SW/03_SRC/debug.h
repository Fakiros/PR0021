/*! \file debug.h \brief Funkcje debuguj¹ce. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
//*****************************************************************************
//
// File Name	: 'debug.h'
// Title	: Helpful debugging functions
// Author	: Pascal Stang - Copyright (C) 2003
//
// Description	: This file contains a set of functions which may be useful
//		for general debugging.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

/** \defgroup debug Funkcje debuguj¹ce
 
 Zawiera ró¿ne funkcje przydatne przy "odpluskwianiu" programów i sprzêtu.
 \code
 #include "debug.h"
 \endcode 
*/

#ifndef DEBUG_H
#define DEBUG_H

#include <avr/io.h>
#include "global.h"
#include "uart.h"

/*@{*/

/**
 Wypisuje obszar pamiêci jako sformatowan¹ tablicê szesnastkow¹ wraz ze znakami ASCII
 \param buffer wskaŸnik do obszaru pamiêci
 \param length d³ugoœæ obszaru do wyœwietlenia
*/
void DEBUG_hextable(u08 *buffer, u16 length);

/*@}*/

#endif
