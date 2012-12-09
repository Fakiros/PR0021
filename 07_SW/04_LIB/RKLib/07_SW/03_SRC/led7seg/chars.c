#include "../led7seg.h"

// makro u³atwiaj¹ce definiowanie kodów znaków na wyœwietlaczu
#ifdef LED7SEG_SEGMENTS_H		///< Wybieranie segmentów stanem wysokim
#define DEF_LED_CHAR(a,b,c,d,e,f,g,h)  (uint8_t)((a<<LED7SEG_A)|(b<<LED7SEG_B)| \
					(c<<LED7SEG_C)|(d<<LED7SEG_D)| \
					(e<<LED7SEG_E)|(f<<LED7SEG_F)| \
					(g<<LED7SEG_G)|(h<<LED7SEG_H))
#else					///< Wybieranie segmentów stanem niskim
#define DEF_LED_CHAR(a,b,c,d,e,f,g,h) (uint8_t)~((a<<LED7SEG_A)|(b<<LED7SEG_B)| \
					(c<<LED7SEG_C)|(d<<LED7SEG_D)| \
					(e<<LED7SEG_E)|(f<<LED7SEG_F)| \
					(g<<LED7SEG_G)|(h<<LED7SEG_H))
#endif

/*
   -a-
 f|   |b
   -g-
 e|   |c
   -d-  .h
*/

// ksztalty wyswietlanych znakow w FLASHU
prog_char LED7SEG_CHARS[]={
//                   a b c d e f g h     adr ascii znak
	DEF_LED_CHAR(1,1,1,1,1,1,0,0),  //00  48    0
	DEF_LED_CHAR(0,1,1,0,0,0,0,0),  //01  49    1
	DEF_LED_CHAR(1,1,0,1,1,0,1,0),  //02  50    2
	DEF_LED_CHAR(1,1,1,1,0,0,1,0),  //03  51    3
	DEF_LED_CHAR(0,1,1,0,0,1,1,0),  //04  52    4
	DEF_LED_CHAR(1,0,1,1,0,1,1,0),  //05  53    5
	DEF_LED_CHAR(1,0,1,1,1,1,1,0),  //06  54    6
	DEF_LED_CHAR(1,1,1,0,0,1,0,0),  //07  55    7
	DEF_LED_CHAR(1,1,1,1,1,1,1,0),  //08  56    8
	DEF_LED_CHAR(1,1,1,1,0,1,1,0),  //09  57    9
//                   a b c d e f g h     adr ascii znak
	DEF_LED_CHAR(1,0,0,0,0,1,0,0),  //10  58    : (|~ -fa)
	DEF_LED_CHAR(1,1,0,0,0,0,0,0),  //11  59    ; (~| -ab)
	DEF_LED_CHAR(0,0,0,1,1,0,1,0),  //12  60    <
	DEF_LED_CHAR(0,0,0,0,0,0,1,0),  //13  61    = (minus)
	DEF_LED_CHAR(0,0,1,1,0,0,1,0),  //14  62    >
	DEF_LED_CHAR(1,1,0,0,1,0,1,0),  //15  63    ?
	DEF_LED_CHAR(1,1,0,1,1,1,1,0),  //16  64    @
//                   a b c d e f g h     adr ascii znak
	DEF_LED_CHAR(1,1,1,0,1,1,1,0),  //17  65    A
	DEF_LED_CHAR(0,0,1,1,1,1,1,0),  //18  66    b
	DEF_LED_CHAR(1,0,0,1,1,1,0,0),  //19  67    C
	DEF_LED_CHAR(0,1,1,1,1,0,1,0),  //20  68    d
	DEF_LED_CHAR(1,0,0,1,1,1,1,0),  //21  69    E
	DEF_LED_CHAR(1,0,0,0,1,1,1,0),  //22  70    F
	DEF_LED_CHAR(1,0,1,1,1,1,1,0),  //23  71    G
	DEF_LED_CHAR(0,1,1,0,1,1,1,0),  //24  72    H
	DEF_LED_CHAR(0,0,1,0,0,0,0,0),  //25  73    i
	DEF_LED_CHAR(0,1,1,1,1,0,0,0),  //26  74    J
//                   a b c d e f g h     adr ascii znak
	DEF_LED_CHAR(0,1,0,0,0,0,1,0),  //27  75    K (_| -bg)
	DEF_LED_CHAR(0,0,0,1,1,1,0,0),  //28  76    L
	DEF_LED_CHAR(0,0,0,0,0,1,1,0),  //29  77    M (|_ -gf)
	DEF_LED_CHAR(0,0,1,0,1,0,1,0),  //30  78    n
	DEF_LED_CHAR(0,0,1,1,1,0,1,0),  //31  79    o
	DEF_LED_CHAR(1,1,0,0,1,1,1,0),  //32  80    P
	DEF_LED_CHAR(1,1,1,0,0,1,1,0),  //33  81    q
	DEF_LED_CHAR(0,0,0,0,1,0,1,0),  //34  82    r
	DEF_LED_CHAR(1,0,1,1,0,1,1,0),  //45  83    S
	DEF_LED_CHAR(0,0,0,1,1,1,1,0),  //36  84    t
//                   a b c d e f g h     adr ascii znak
	DEF_LED_CHAR(0,1,1,1,1,1,0,0),  //37  85    U
	DEF_LED_CHAR(0,0,1,1,1,0,0,0),  //38  86    v
	DEF_LED_CHAR(1,1,0,0,0,1,1,0),  //39  87    W (o -abgf)
	DEF_LED_CHAR(0,1,1,0,1,1,1,0),  //40  88    X (jak H)
	DEF_LED_CHAR(0,1,1,1,0,1,1,0),  //41  89    y 
	DEF_LED_CHAR(1,1,0,1,1,0,1,0),  //42  90    Z (jak 2)
	DEF_LED_CHAR(1,0,0,1,1,1,0,0),  //43  91    [
	DEF_LED_CHAR(0,1,0,0,0,1,1,0),  //44  92    u (u -bfg)
	DEF_LED_CHAR(1,1,1,1,0,0,0,0),  //45  93    ]
//                   a b c d e f g h     adr ascii znak
	DEF_LED_CHAR(0,0,0,0,0,0,0,0),  //46  94    BLANK
	DEF_LED_CHAR(0,0,0,1,0,0,0,0)   //47  95    _
	};  
