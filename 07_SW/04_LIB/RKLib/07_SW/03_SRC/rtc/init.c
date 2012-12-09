#include "../rtc.h"

void RTC_init(void)
{
#ifdef AS2
  TIMSK &=~_BV(TOIE2);  // Wy³¹cz przerwania TC2
  ASSR |= _BV(AS2);     // prze³¹cz TC2 z taktowania zegarem CPU
                        // na generator asynchoniczny 32768 Hz
  TCCR2 = _BV(CS22)|_BV(CS20);  // ustaw preskaler na podzia³ przez 128
                        // aby generowaæ przerwania dok³adnie co 1 sekundê
  while(ASSR&0x07);     // czekaj na uaktualnienie TC2
  TIMSK |= _BV(TOIE2);  // w³¹cz przerwania z TC2
#else
#ifdef AS0
// !!!!!!!!!!!! tu trzeba wpisaæ inicjalizacjê (nie mam ATmega 103 lub 128) !!!!!!!!!!!!
#endif
#endif
}

struct_time RTC_time;  /// zmienna przechowuj¹ca czas

//sprawdza rok przestêpny
char not_leap(void)
{
  if (!(RTC_time.year%100))            // jeœli rok jest podzielny przez 100
    return (char)(RTC_time.year%400);  // sprawdŸ i zwróæ rok podzielny przez 400
  else                          // w przeciwnym wypadku
    return (char)(RTC_time.year%4);    // zwróæ rok podzielny przez 4
}

// zliczanie czasu
// w oparciu o ATMEL Application Note AVR134
#ifdef AS0
SIGNAL(SIG_OVERFLOW0)           // obs³uga przerwania od licznika 0
#else
#ifdef AS2
SIGNAL(SIG_OVERFLOW2)           // obs³uga przerwania od licznika 2
#endif
#endif
{
  if (++RTC_time.second==60)           // inkrementuj sekundy i sprawdŸ czy jest ich 60
  {                             // jeœli tak to:
    RTC_time.second=0;                 // wyzeruj licznik sekund oraz
    if (++RTC_time.minute==60) // inkrementuj licznik minut i sprawdŸ czy jest ich 60
    {                           // jeœli tak to:
      RTC_time.minute=0;               // wyzeruj licznik minut oraz
      if (++RTC_time.hour==24) // inkrementuj licznik godzin i sprawdŸ czy jest ich 24
      {                         // jeœli tak to:
        RTC_time.hour=0;               // wyzeruj licznik godzin oraz
        if (++RTC_time.day==32)       // inkrementuj licznik dni i sprawdŸ czy jest ich 32
        {                       // jeœli tak to:
          RTC_time.month++;            // inkrementuj licznik miesiêcy
          RTC_time.day=1;             // ustaw dzieñ na 1
        }
        else if (RTC_time.day==31)    // je¿eli dzieñ równa siê 31
        {                       // to sprawdŸ czy s¹ to miesi¹ce: 4, 6, 9, 11
          if ((RTC_time.month==4) || (RTC_time.month==6) || 
              (RTC_time.month==9) || (RTC_time.month==11))
          {                     // jeœli tak to:
            RTC_time.month++;          // inkrementuj licznik miesiêcy
            RTC_time.day=1;           // ustaw dzieñ na 1
          }
        }
        else if (RTC_time.day==30)    // je¿eli dzieñ równa siê 30
        {                       // to sprawdŸ czy
          if (RTC_time.month==2)       // jest to miesi¹c 2 (luty)
          {                     // jeœli tak to:
            RTC_time.month++;          // inkrementuj licznik miesiêcy
            RTC_time.day=1;           // ustaw dzieñ na 1
          }
        }
        else if (RTC_time.day==29)    // je¿eli dzieñ równa siê 29
        {                       // to sprawdŸ czy:
          if ((RTC_time.month==2) && (not_leap())) // miesi¹c 2 i rok nieprzestêpny
          {                     // jeœli tak to:
            RTC_time.month++;          // inkrementuj licznik miesiêcy
            RTC_time.day=1;           // ustaw dzieñ na 1
          }
        }
        if (RTC_time.month==13)        // jeœli miesi¹c wynosi 13
        {                       // to:
          RTC_time.month=1;            // ustaw miesi¹c na 1 (styczeñ)
          RTC_time.year++;             // inkrementuj licznik lat
        }
      }
    }
  }
}
