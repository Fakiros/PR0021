/*! \file i2c_ee.h \brief Dostêp do pamiêci EEPROM z magistral¹ I2C typu 24Cxx. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** \defgroup i2c_ee EEPROM I2C
 
 Dostêp do pamiêci EEPROM z magistral¹ I2C typu 24Cxx.
 
 \code
 #include "i2c_ee.h"
 \endcode 
 
 \ref config:
 
 W pliku \c "config.h" znajduj¹cym siê w katalogu projektu nale¿y umieœciæ nastêpuj¹c¹ zawartoœæ:
    	     
 \code
#define I2C_PORT	PORTD	// Port do którego s¹ pod³¹czone linie magistrali
#define I2C_SDA		5	// Bit powy¿szego portu u¿ywany jako linia SDA
#define I2C_SCL 	6	// Bit powy¿szego portu u¿ywany jako linia SCL

#define I2C_EE_BASE	0xA0	// Adres bazowy pamiêci EEPROM na magistrali I2C
#define I2C_EE_TYPE	1 	// Typ pamiêci EEPROM na magistrali I2C:
 \endcode
 
 \note Wartoœci z \c #define mo¿na zmieniaæ w celu dostosowania 
 biblioteki do tworzonego urz¹dzenia i oprogramowania.
 
 \par Wykorzytywane biblioteki:
 
 \ref delay, \ref i2c
*/

#ifndef __I2C_EE_H__
#define __I2C_EE_H__

#include "i2c.h"
#include "delay.h"

/*@{*/

#ifndef I2C_EE_BASE
/**
  Adres bazowy pamiêci EEPROM na magistrali I2C
 \note Mo¿na zmieniæ w pliku "config.h" projektu
*/
#define I2C_EE_BASE 0xA0 
#endif

#ifndef I2C_EE_TYPE
/**
 Typ pamiêci EEPROM na magistrali I2C:
 \li 0 : 24C01, 
 \li 1 : 24C01A-24C16,
 \li 2 : 24C164-24C512.
 \note Mo¿na zmieniæ w pliku "config.h" projektu
 jeœli nie zadeklarujemy jawnie typu pamiêci to
 tworzona jest zmienna globalna 
u08  I2C_EE_TYPE
*/
//#define I2C_EE_TYPE 1 
u08  I2C_EE_TYPE;
#endif
#if (I2C_EE_TYPE < 0) || (I2C_EE_TYPE > 2)
  #error Invalid I2C EEPROM type!
#endif

/**
 Wysy³a adres komórki pamiêci na magistralê
 \param address adres komórki
*/
void I2C_EE_send_adr(u16 address);

/**
 Zapisuje stronicowo dane do EEPROM	 	
 \param address adres pierwszej komórki bloku w EEPROM
 \param dat wskaznik do bloku danych w RAM
 \param cnt iloœæ danych - mo¿e przyjmowaæ nast wartoœci: 1,2,4,8,16,32,64
 \return 0 w przypadku powodzenia
*/
u08 I2C_EE_write_page(u16 address, u08* dat, u08 cnt);

/**
 Czyta stronicowo dane z EEPROM
 \param address adres pierwszej komórki bloku w EEPROM
 \param dat wskaznik do bloku danych w RAM
 \param cnt iloœæ danych - mo¿e przyjmowaæ nast wartoœci: 1,2,4,8,16,32,64
*/
void I2C_EE_read_page(u16 address, u08* dat, u08 cnt);

/*@}*/

#endif // __I2C_EE_H__
