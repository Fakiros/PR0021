/*! \file lcd.h \brief Obs³uga znakowego wyœwietlacza LCD w trybie 4 bitowym. */
/*
  Copyright (c) 2004,5 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** \defgroup lcd Wyœwietlacz LCD
 
 Obs³uga znakowego wyœwietlacza LCD w trybie 4 bitowym.
 
 \code
 #include "lcd.h"
 \endcode 
 
 Jako jedno ze standardowych urz¹dzeñ wyjœciowych 
 w systemach mikroprocesorowych jest stosowany 
 znakowy wyœwietlacz LCD. Z uwagi na szerokie 
 zastosowanie w rozmaitych projektach, procedury 
 jego obs³ugi powinny siê cechowaæ ³atwoœci¹ stosowania
 (zarówno w programach jak i w sprzêcie) oraz uniwersalnoœci¹.
 Wyœwietlacz jest obs³ugiwany w trybie 4 bitowym. 
 Nale¿y go pod³¹czyæ do dowolnego portu mikrokontrolera
 z tym, ¿e linie kontrolne RS i EN musz¹ byæ pod³¹czone
 do bitów znajduj¹cych siê w jednej "po³ówce" bajtu 
 np. je¿eli RS pod³¹czymy do linii nr 2 to EN mo¿emy 
 pod³¹czyæ do jednej linii: 0, 1 lub 3. 
 Nie jest mo¿liwe pod³¹czenie jej do linii 4, 5, 6 lub 7 
 gdy¿ jest ona automatycznie przydzielana do pod³¹czenia 
 czterech bitów danych wyœwietlacza. 
 Analogiczna zasada dotyczy wykorzystania górnej "po³ówki" 
 portu do pod³¹czenia linii kontrolnych EN i RS -
 wtedy bity 0-3 portu s¹ wykorzystane do pod³¹czenia 
 linii danych wyœwietlacza. 
 
 \ref config:
 
 W pliku \c "config.h" znajduj¹cym siê w katalogu projektu nale¿y umieœciæ nastêpuj¹c¹ zawartoœæ:
 
 \code
#define LCD_PORT	PORTA	// port wyœwietlacza
#define LCD_RS 		5	// bit linii RS
#define LCD_EN 		4	// bit linii EN
#define LCD_X		16	// liczba znaków w linii wyœwietlacza
#define LCD_Y		2	// liczba wierszy wyœwietlacza
// opcjonalne ustawienia - powoduj¹ zwiêkszenie d³ugoœci kodu
// jeœli nie s¹ wymagane nale¿y je "zakomentowaæ":
#define LCD_BIT_SWAP		// zamiana bitów danych LCD D3<>D0, D2<>D1
#define LCD_WRAP		// "zawijanie" tekstu na wyœwietlaczu
#define LCD_WIN1250		// polskie znaki wg WIN1250
 \endcode
 
 \note Wartoœci z \c #define mo¿na zmieniaæ w celu dostosowania 
 biblioteki do tworzonego urz¹dzenia i oprogramowania.
 
 \par Wykorzytywane biblioteki:
 
 \ref delay
 
 \par Przyk³ad:
 
 Wykorzystanie definicji polskich znaków diakrytycznych:
 
 \code
 LCD_PL_chars_init();	// inicjalizacja polskich znaków
 LCD_putstr_P(PSTR("Tak mo"_z"na pisa"_c" bez WIN1250"));
 \endcode
 
 Rysowanie paska postêpu:
 
 \code
 LCD_progressbar_init(); // inicjalizacja generatora znaków
 LCD_progressbar(1,67);	 // pasek postêpu w wierszu 1
 
 \endcode
*/

#ifndef __LCD_H__
#define __LCD_H__

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "global.h"
#include "delay.h"
#include "conv.h"

/*@{*/

#if ((LCD_EN>3)&(LCD_RS<=3))|((LCD_EN<=3)&(LCD_RS>3))
  #error "Bity LCD_EN i LCD_RS musza znajdowac sie w jednej polowce bajtu!!!"
#endif

#if (LCD_EN>3)&(LCD_RS>3)
  #define LCD_HALF_LO
#else
  #undef LCD_HALF_LO
#endif

#define LCD_PORT_O		LCD_PORT      
#define LCD_PORT_D		DDR(LCD_PORT)
#define LCD_PORT_I		PIN(LCD_PORT)

/**
 Wy³¹cza kursor
*/
#define LCD_cursor_off()	LCD_send(0x0C, 0)

/**
 W³¹cza kursor
*/
#define LCD_cursor_on()		LCD_send(0x0F, 0)

/**
 Przesuwa kursor w prawo
*/
#define LCD_cursor_right()	LCD_send(0x14, 0)

/**
 Przesuwa kursor w lewo
*/
#define LCD_cursor_left()	LCD_send(0x10, 0)

/**
 Definicja polskiego znaków diakrytycznego '¹' na LCD
*/
#define _a		"\008"

/**
 Definicja polskiego znaków diakrytycznego 'æ' na LCD
*/
#define _c		"\001"

/**
 Definicja polskiego znaków diakrytycznego 'ê' na LCD
*/
#define _e		"\002"

/**
 Definicja polskiego znaków diakrytycznego '³' na LCD
*/
#define _l		"\003"

/**
 Definicja polskiego znaków diakrytycznego 'ñ' na LCD
*/
#define _n		"\004" 

/**
 Definicja polskiego znaków diakrytycznego 'ó' na LCD
*/
#define _o		"\005" 

/**
 Definicja polskiego znaków diakrytycznego 'œ' na LCD
*/
#define _s		"\006" 

/**
 Definicja polskiego znaków diakrytycznego '¿' na LCD
*/
#define _z		"\007"

/**
 Definicja polskiego znaków diakrytycznego '¹' na LCD
*/
#define LCD_a		0x08 

/**
 Definicja polskiego znaków diakrytycznego 'æ' na LCD
*/
#define LCD_c		0x09

/**
 Definicja polskiego znaków diakrytycznego 'ê' na LCD
*/
#define LCD_e		0x0a

/**
 Definicja polskiego znaków diakrytycznego '³' na LCD
*/
#define LCD_l		0x0b

/**
 Definicja polskiego znaków diakrytycznego 'ñ' na LCD
*/
#define LCD_n		0x0c

/**
 Definicja polskiego znaków diakrytycznego 'ó' na LCD
*/
#define LCD_o		0x0d

/**
 Definicja polskiego znaków diakrytycznego 'œ' na LCD
*/
#define LCD_s		0x0e

/**
 Definicja polskiego znaków diakrytycznego '¿' na LCD
*/
#define LCD_z		0x0f

/**
 Kod polskiego znaku diakrytycznego '¹' w std. WIN1250 
*/
#define WIN1250_a	0xb9

/**
 Kod polskiego znaku diakrytycznego 'æ' w std. WIN1250 
*/
#define WIN1250_c	0xe6 

/**
 Kod polskiego znaku diakrytycznego 'ê' w std. WIN1250 
*/
#define WIN1250_e	0xea 

/**
 Kod polskiego znaku diakrytycznego '³' w std. WIN1250 
*/
#define WIN1250_l	0xb3 

/**
 Kod polskiego znaku diakrytycznego 'ñ' w std. WIN1250 
*/
#define WIN1250_n	0xf1 

/**
 Kod polskiego znaku diakrytycznego 'ó' w std. WIN1250 
*/
#define WIN1250_o	0xf3 

/**
 Kod polskiego znaku diakrytycznego 'œ' w std. WIN1250 
*/
#define WIN1250_s	0x9c 

/**
 Kod polskiego znaku diakrytycznego '¿' w std. WIN1250 
*/
#define WIN1250_z	0xbf

/**
 Pozycja kursora na LCD 0...79
*/
extern volatile uint8_t LCD_pos;

/**
 Wystawienie danych 4 bitowych na szynê i ich "zatrzaœniêcie"
*/
void LCD_clock(uint8_t data, uint8_t rs);

/**
 Inicjuje wyœwietlacz zgodnie z ustawieniami w pliku "config.h", w³¹cza interfejs 4-bitowy.
*/
void LCD_init(void);

/**
 Wykonuje kasowanie ekranu wyœwietlacza LCD. Umieszcza kursor na pozycji 0,0.
*/
void LCD_clear(void);

/**
 Wystawienie danych 8 bitowych jako 4 bit na szynê i ich "zatrzaœniêcie".
 W programach u¿ytkownika nie nale¿y jej wywo³ywaæ bez specjalnej potrzeby.
 \param data dane do wys³ania
 \param rs stan linii RS wyœwietlacza
 */
void LCD_send(uint8_t data, uint8_t rs);

/**
 Wystawienie danych 8 bitowych jako 4 bit na szynê
 z lini¹ RS=0 i ich "zatrzaœniêcie"
 W programach u¿ytkownika nie nale¿y jej wywo³ywaæ bez specjalnej potrzeby.
 \param data dane do wys³ania
*/
void LCD_send_0(uint8_t data);

/**
 Wystawienie danych 8 bitowych jako 4 bit na szynê
 z lini¹ RS=1 i ich "zatrzaœniêcie"
 W programach u¿ytkownika nie nale¿y jej wywo³ywaæ bez specjalnej potrzeby.
 \param data dane do wys³ania
*/
void LCD_send_1(uint8_t data);

/**
 Ustawienie pozycji kursora wyœwietlacza.
 */
void LCD_xy(uint8_t x, uint8_t y);

/**
 Wyœwietla znak na aktualnej pozycji kursora 
 i przesuwa kursor na nastêpn¹ pozycjê.
 \param ch znak do wyœwietlenia
 */
int LCD_putchar(char ch);

/**
 * Wyœwietlenie znaku ch na wspó³rzêdnych x,y
 \param x wspó³rzêdna pozioma
 \param y wspó³rzêdna pionowa
 \param ch znak do wyœwietlenia
 */
void LCD_putcharxy(uint8_t x, uint8_t y, char ch);

/**
 Wyœwietla na aktualnej pozycji kursora ³añcuch znajduj¹cy
 siê w pamiêci SRAM i przesuwa kursor na nastêpn¹ pozycjê.
 \param s ³añcuch znaków do wyœwietlenia
 */
void LCD_putstr(uint8_t* s);

/**
 Wyœwietla znakow¹ reprezentacjê liczby o wybranej postawie.
 \note korzysta z funkcji \b itoa() co oznacza doœæ du¿e zapotrzebowanie na pamiêæ programu,
 jeœli nie ma specjalnej potrzeby aby j¹ zastosowaæ lepiej skorzystaæ z funkcji:
 LCD_putuint8_t(), LCD_puthexuint8_t(), LCD_puthexU16().
 \param value wartoœæ do wyœwietlenia
 \param radix podstawa np 2, 8, 10, 16
 */
void LCD_putint(int value, uint8_t radix);

/** 
 Wyœwietla dziesiêtn¹ reprezentacjê liczby 8 bitowej
 \param value wartoœæ do wyœwietlenia
*/
void LCD_putuint8_t(uint8_t value);

/** 
 Wyœwietla szesnastkow¹ reprezentacjê liczby 8 bitowej
 \param value wartoœæ do wyœwietlenia
*/
void LCD_puthexuint8_t(uint8_t value);

/** 
 Wyœwietla szesnastkow¹ reprezentacjê liczby 16 bitowej
 \param value wartoœæ do wyœwietlenia
*/
void LCD_puthexU16(u16 value);

/**
 £aduje definicje znaków u¿ytkownika do LCD
 z 64 elementowej tablicy umieszczonej w pamiêci programu.
 \param s tablica umieszczona w pamiêci programu
 */
void LCD_defchars_P(const char *s);

/**
 Wyœwietla na aktualnej pozycji kursora ³añcuch znajduj¹cy
 siê w pamiêci programu (FLASH) i przesuwa kursor na nastêpn¹ pozycjê.
 \param s ³añcuch umieszczony w pamiêci programu
*/
void LCD_putstr_P(const char *s);

/**
 Wyœwietla na aktualnej pozycji kursora ³añcuch znajduj¹cy
 siê w pamiêci EEPROM i przesuwa kursor na nastêpn¹ pozycjê.
 \param s ³añcuch umieszczony w pamiêci EEPROM
*/
void LCD_putstr_E(uint8_t *s);

/**
 Wyœwietla na wspó³rzêdnych x, y ³añcuch znajduj¹cy siê 
 w pamiêci SRAM i przesuwa kursor na nastêpne pozycje. 
 \param x wspó³rzêdna pozioma
 \param y wspó³rzêdna pionowa
 \param s ³añcuch znaków
 */
void LCD_putstrxy(uint8_t x, uint8_t y, char *s);

/**
 £aduje do pamiêci RAM generatora znaków w LCD 
 definicje polskich znaków diakrytycznych (kody 0-7: ¹æê³óœ¿Ÿ).
*/
void LCD_PL_chars_init(void);

/**
 £aduje do pamiêci RAM generatora znaków w LCD definicje 
 znaków s³u¿¹cych do wyœwietlania paska postêpu (kody 0-7).
 \warning inne wczeœniej zdefniowane znaki np. polskie zostan¹ nadpisane
*/
void LCD_progressbar_init(void);

/**
 Wyœwietla poziomy pasek postêpu w wybranym wierszu wyœwietlacza.
 D³ugoœæ paska okreœla zmienna value - 0: brak; 255: ca³y wiersz.
 \param y wiersz wyœwietlacza
 \param value d³ugoœæ paska
*/
void LCD_progressbar(uint8_t y, uint8_t value);

/*@}*/

#endif
