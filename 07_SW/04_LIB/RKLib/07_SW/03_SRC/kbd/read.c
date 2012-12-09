#include "../kbd.h"

u08 KBD_key;	//zmienna przechowuj¹ca kod ostatnio przyciœniêtego klawisza

/*
	Funkcja pomocnicza (odczyt kolumny)
*/
u08 _KBD_readcol(u08 col)
{
  register u08 c=0;
  if (KBD_row1) c=col; else
  if (KBD_row2) c=col+1; else
  if (KBD_row3) c=col+2; else
  if (KBD_row4) c=col+3;
  return(c);
}

/*
	Odczyt klawiatury
*/
u08 KBD_read(void)
{
  register u08 k;
// ustawienie rzêdów na odczyt
  cbi(KBD_PORT_D_row1,KBD_BIT_row1);  
  cbi(KBD_PORT_D_row2,KBD_BIT_row2);
  cbi(KBD_PORT_D_row3,KBD_BIT_row3);
  cbi(KBD_PORT_D_row4,KBD_BIT_row4);
// za³¹czenie rezystorów podci¹gaj¹cych na liniach rzêdów
  sbi(KBD_PORT_row1,KBD_BIT_row1);
  sbi(KBD_PORT_row2,KBD_BIT_row2);
  sbi(KBD_PORT_row3,KBD_BIT_row3);
  sbi(KBD_PORT_row4,KBD_BIT_row4);

  KBD_key=0;
  KBD_col1_0;
  k=_KBD_readcol(1);
  if (k>0)
    KBD_key=k;
  KBD_col1_1;
  KBD_col2_0;
  k=_KBD_readcol(2);
  if (k>0) 
    KBD_key=3+k;
  KBD_col2_1;
  KBD_col3_0;
  k=_KBD_readcol(3);
  if (k>0) 
    KBD_key=6+k;
  KBD_col3_1;
  KBD_col4_0;
  k=_KBD_readcol(4);
  if (k>0) 
    KBD_key=9+k;
  KBD_col4_1;
  return(KBD_key);
}
