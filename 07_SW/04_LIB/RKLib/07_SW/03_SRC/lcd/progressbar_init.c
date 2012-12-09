#include "../lcd.h"

/*
	definicje znaków i funkcja wskaŸnika postêpu na LCD
*/
prog_char __PBARchars[] = {		
	32 , 32 , 32 , 32 , 32 , 32 , 32 , 32,
	16 , 16 , 16 , 16 , 16 , 16 , 16 , 16,
	24 , 24 , 24 , 24 , 24 , 24 , 24 , 24,
	28 , 28 , 28 , 28 , 28 , 28 , 28 , 28,
	30 , 30 , 30 , 30 , 30 , 30 , 30 , 30,
	31 , 31 , 31 , 31 , 31 , 31 , 31 , 31
	};

void LCD_progressbar_init(void)	// ³aduje odpowiedni zestaw znaków
{				// UWAGA! inne wczeœniej zdefniowane
  LCD_defchars_P(__PBARchars);	// znaki np. polskie zostan¹ nadpisane
}

