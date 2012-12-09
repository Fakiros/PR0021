/*! \file pcf8583.h \brief Dostêp do zegara czasu rzeczywistego z magistral¹ I2C typu PCF8583. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** \defgroup pcf8583 Zegar I2C PCF8583
 
 Dostêp do zegara czasu rzeczywistego z magistral¹ I2C typu PCF8583.
 
 \code
 #include "pcf8583.h"
 \endcode 
 
 \ref config:
 
 W pliku \c "config.h" znajduj¹cym siê w katalogu projektu nale¿y umieœciæ nastêpuj¹c¹ zawartoœæ:
    	     
 \code
#define I2C_PORT	PORTD	//port do którego s¹ pod³¹czone linie magistrali
#define I2C_SDA		5	//bit powy¿szego portu u¿ywany jako linia SDA
#define I2C_SCL 	6	//bit powy¿szego portu u¿ywany jako linia SCL
 \endcode
 
 \note Wartoœci z \c #define mo¿na zmieniaæ w celu dostosowania 
 biblioteki do tworzonego urz¹dzenia i oprogramowania.
 
 \par Wykorzytywane biblioteki:
 
 \ref i2c
*/

#ifndef __PCF8583_H__
#define __PCF8583_H__

#include "config.h"
#include "conv.h"
#include "i2c.h"

/*@{*/

#ifndef PCF8583_A0
/**
 Stan linii A0 uk³adu okreœlaj¹cy adres. Mo¿na zmieniæ w pliku \c "config.h"
*/
#define PCF8583_A0	0
#endif // PCF8583_A0

/**
 Bajt statusu uk³adu
*/
extern volatile u08 PCF8583_status;

/**
 Bajt alarmu uk³adu
*/
extern volatile u08 PCF8583_alarm;

/**
 Czyta bajt z uk³adu
 \param address adres komórki w uk³adzie
 \return odczytany bajt
*/
u08 PCF8583_read(u08 address);

/**
 Zapisuje bajt do uk³adu
 \param address adres komórki w uk³adzie 
 \param data bajt do wpisania
*/
void PCF8583_write(u08 address, u08 data);

/**
 Czyta bajt z uk³adu w formacie BCD
 \param address adres komórki w uk³adzie 
 \return odczytany bajt
*/
u08 PCF8583_read_bcd(u08 address);

/**
 Zapisuje bajt do uk³adu w formacie BCD
 \param address adres komórki w uk³adzie 
 \param data bajt do wpisania
*/
void PCF8583_write_bcd(u08 address,u08 data);

/**
 Czyta status uk³adu
 \return status uk³adu
*/
u08 PCF8583_get_status(void);

/**
 Inicjalizuje uk³ad
*/
void PCF8583_init(void);

/**
 Zatrzymuje uk³ad
*/
void PCF8583_stop(void);

/**
 Startuje uk³ad
*/
void PCF8583_start(void);

/**
 Odwiesza uk³ad
*/
void PCF8583_hold_off(void);

/**
 Zawiesza uk³ad
*/
void PCF8583_hold_on(void);

/**
 Wy³¹cza alarm
*/
void PCF8583_alarm_off(void);

/**
 Za³¹cza alarm
*/
void PCF8583_alarm_on(void);

/**
 Zapisuje s³owo do uk³adu
 \param address adres komórki w uk³adzie 
 \param data s³owo do wpisania
*/
void PCF8583_write_word(u08 address,u16 data);

/**
 Ustawia datê w uk³adzie
 \param address adres komórki w uk³adzie
 \param day dzieñ
 \param year rok
*/
void PCF8583_write_date(u08 address,u08 day,u16 year);

/**
 Czyta czas z uk³adu
 \param hour godzina
 \param min minuta
 \param sec sekunda
 \param hsec setne czêœci sekundy
*/
void PCF8583_get_time(u08 *hour,u08 *min,u08 *sec,u08 *hsec);

/**
 Ustawia czas w uk³adzie
 \param hour godzina
 \param min minuta
 \param sec sekunda
 \param hsec setne czêœci sekundy
*/
void PCF8583_set_time(u08 hour,u08 min,u08 sec,u08 hsec);

/**
 Czyta datê z uk³adu
 \param day dzieñ
 \param month miesi¹c
 \param year rok
*/
void PCF8583_get_date(u08 *day,u08 *month,u16 *year);

/**
 Ustawia datê w uk³adzie
 \param day dzieñ
 \param month miesi¹c
 \param year rok
*/
void PCF8583_set_date(u08 day,u08 month,u16 year);

/**
 Czyta czas alarmu z uk³adu
 \param hour godzina
 \param min minuta
 \param sec sekunda
 \param hsec setne czêœci sekundy
*/
void PCF8583_get_alarm_time(u08 *hour,u08 *min,u08 *sec,u08 *hsec);

/**
 Ustawia czas alarmu w uk³adzie
 \param hour godzina
 \param min minuta
 \param sec sekunda
 \param hsec setne czêœci sekundy
*/
void PCF8583_set_alarm_time(u08 hour,u08 min,u08 sec,u08 hsec);

/**
 Czyta datê alarmu z uk³adu
 \param day dzieñ
 \param month miesi¹c
*/
void PCF8583_get_alarm_date(u08 *day,u08 *month);

/**
 Ustawia datê alarmu w uk³adzie
 \param day dzieñ
 \param month miesi¹c
*/
void PCF8583_set_alarm_date(u08 day,u08 month);

/*@}*/

#endif // __PCF8583_H__
