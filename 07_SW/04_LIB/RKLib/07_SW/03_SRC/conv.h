/** \file conv.h \brief Funkcje konwertuj¹ce. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** 
 \defgroup conv Funkcje konwertuj¹ce
 
 Funkcje dokonuj¹ce konwersji pomiêdzy ró¿nymi sposobami zapisywania liczb.
 
 \code
 #include "conv.h"
 \endcode 
*/

#ifndef __CONV_H__
#define __CONV_H__

#include "config.h"
#include "global.h"

/*@{*/

/**
 Zamiana kodu BCD na binarny
 bity BCD: 7654 - dziesiatki, 3210 -jednosci
 \param bcd wartoœæ w kodzie BCD
 \return kod binarny z kodu BCD
 \see u08 bin2bcd(u08 bin);
*/
u08 bcd2bin(u08 bcd);

/**
 Zamiana kodu binarnego na BCD
 \param bin wartoœæ binarna z zakresu 0-99
 \return kod BCD, bity: 7654 - dziesiatki, 3210 -jednosci
 \see u08 bcd2bin(u08 bcd);
*/
u08 bin2bcd(u08 bin);

/**
 Zamiana wartoœci binarnej na znak ASCII w kodzie szesnastkowym
 \param val wartoœæ binarna z zakresu 0-15
 \return znak ASCII w kodzie szesnastkowym 0-F
*/
char nibble2hex(u08 val);

/**
 Zamiana wartoœci binarnej na tekst ASCII w kodzie szesnastkowym
 \param val wartoœæ binarna z zakresu 0-255
 \param *s wskaznik do zwracanego ci¹gu ASCII w kodzie szesnastkowym 00-FF
*/
void byte2hex(u08 val, char *s);

/**
 Zamiana wartoœci binarnej na tekst ASCII w kodzie szesnastkowym
 \param val wartoœæ binarna z zakresu 0-65535
 \param *s wskaznik do zwracanego ci¹gu ASCII w kodzie szesnastkowym 0000-FFFF
*/
void word2hex(u16 val, char *s);

/**
 Zamiana wartoœci binarnej na tekst ASCII w kodzie dziesiêtnym
 \param val wartoœæ binarna z zakresu 0-99
 \param *s wskaznik do zwracanego ci¹gu ASCII w kodzie dziesiêtnym 0-99
*/
void byte2dec(u08 val, char *s);

/*@Konwersja z int do string}*/

void int2str(int val, char *s);

#endif // __CONV_H__
