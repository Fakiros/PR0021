#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>


#include "drv/AC_line.h"
#include "menu/menu_gen.h"


static void  menu_g_exe(void);
static void  menu_g2_exe(void);
static void menu_g3_exe(void);


static void  BarGraph(void);
static void  UpBar(void);
static void  DownBar(void);
static void  Escape(void);

uint8_t BarGraph_level;

/****************************************************************************/
/*                                MenuContent                               */
/****************************************************************************/
/*
PROGMEM  menu_t menu1[MENU_ITEMS] =
{ 
	//	exe funct	,menu ptr	, display
	{	NULL		,NULL		,PSTR("Menu11")	},
	{	NULL		,NULL		,PSTR("Menu12")	},
	{	menu_g_exe	,NULL		,PSTR("Menu13") },


	

};
*/

static char PROGMEM disp_[] PROGMEM = "Text\0"; 
 


PROGMEM  menu_t Menu_Ustawienia[] =
{ 
	//	exe funct				   , handler	,menu ptr	, display
	{	NULL					      ,NULL  		,NULL			,disp_	},
	{	MENU_termination_item	,NULL  		,NULL			,NULL	}// This line terminate menu content 
};

