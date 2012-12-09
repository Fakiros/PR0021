/** \file twi.h \brief Sterownik interfejsu TWI. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl

  Based on:
  Title: Interrupt driven Two Wire Interface
  Author: kkempeneers (Koen Kempeneers)
  Registered at AVRfreaks.net November 11, 2003
*/
/** \defgroup twi Interfejs TWI
	
	Obs³uga interfejsu TWI
 \code
 #include "twi.h"
 \endcode 

 \par Wykorzystywane przerwania:
 
 \c SIG_2WIRE_SERIAL
*/
#ifndef __TWI_H__
#define __TWI_H__

#include <avr/io.h>
#include <compat/twi.h>
#include <avr/signal.h>
#include <avr/interrupt.h>

#include "config.h"
#include "global.h"

/*@{*/

/** Bit definitions for the tw_status register */
#define TWI_BUSY 7

extern volatile unsigned char TWI_address;	///< Adres sprzêtowy uk³adu
extern volatile unsigned char *TWI_data;	///< WskaŸnik do danych TWI 
extern volatile unsigned char TWI_ddr;	///< Kierukek danych TWI
extern volatile unsigned char TWI_bytes;	///< Iloœæ bajtów TWI
extern volatile unsigned char TWI_stop;	///< Znacznik zakoñczenia transmisji

extern volatile unsigned char TWI_stat;	///< Status TWI
extern volatile unsigned char TWI_retry_cnt; ///< Licznik powtórzeñ TWI

/** 
 Inicjalizacja TWI
*/
void TWI_init(void);

/** 
 Zapisuje dane do uk³adu z interfejsem TWI
 \param address 7-bitowy adres sprzêtowy uk³adu
 \param *data wskaŸnik do bloku danych
 \param bytes wielkoœæ bloku do wys³ania
 \return 0 (zero)
*/
int TWI_write(unsigned char address, unsigned char *data, unsigned char bytes);

/** 
 Czyta dane z uk³adu z interfejsem TWI
 \param address 7-bitowy adres sprzêtowy uk³adu
 \param *data wskaŸnik do bloku danych
 \param bytes wielkoœæ bloku do odczytu
 \return 0 (zero)
*/
int TWI_read(unsigned char address, unsigned char *data, unsigned char bytes);

/*@}*/

#endif //__TWI_H__

