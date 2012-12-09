#include "../uart0.h"

void UART0_getstr (char *s)
{
  uint8_t i=0;
  uint8_t ch=0;
  do
  {
    WDR();			// restart licznika Watchdoga
    ch=UART0_getchar();		// pobieraj znak
    s[i]=ch;			// wstaw go do ³añcucha i inkrementuj
    i++;			// inkrementuj iloœæ znaków odebranych
  }
  while ((ch != 10) && (ch != 13) && (i <= UART0_MAX_GETSTR));
// dopóki nie napotka znaków koñca linii lub nie przekroczy d³ugoœci MAX_GETSTR
  s[i-1]=0;			// wstaw 0 na koniec ³añcucha
}
