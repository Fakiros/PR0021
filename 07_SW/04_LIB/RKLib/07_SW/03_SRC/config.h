#ifndef F_CPU
 #define F_CPU		16000000 //czêstotliwoœæ zegara w Hz
#endif
/////////////////////////////////////////////////////////////////////////

//-------------------------------------------------
// UART
//-------------------------------------------------

//#define UART0_BAUD	9600 		// prêdkoœæ transmisji
//#define UART0_BUF_SIZE	16		// wielkoœæ buforów UART 

#define UART_BAUD	9600 		// prêdkoœæ transmisji
#define UART_BUF_SIZE	128		// wielkoœæ buforów UART
					// automatycznie w³¹cza 
					// obs³ugê UART na przerwaniach
//#define UART0_MAX_GETSTR	8	
//#define UART_DE_PORT	PORTD		// port linii DE konwertera RS485
//#define UART_DE_BIT	7		// bit portu linii DE konwertera RS485
//*/
//-------------------------------------------------
// PS2MOUSE
//-------------------------------------------------

//#define PS2MOUSE_PORT	PORTD
//#define PS2MOUSE_CLOCK 0
//#define PS2MOUSE_DATA	6


/*  DS 18x20  */
 
 #define OW_PORT	PORTA
 #define OW_BIT		2



