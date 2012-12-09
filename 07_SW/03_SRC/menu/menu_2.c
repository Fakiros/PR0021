#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>


#include "menu/menu_gen.h"


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

static char PROGMEM wsk2_1[] PROGMEM = "Obroty na kg\0"; 
static char PROGMEM wsk2_2[] PROGMEM = "Temperatura In\0"; 
static char PROGMEM wsk2_3[] PROGMEM = "MENU_2_3\0"; 
static char PROGMEM wsk2_4[] PROGMEM = "MENU_2_4\0"; 


PROGMEM  menu_t Menu_Ustawienia[] =
{ 
	//	exe funct				   , handler	,menu ptr	, display
	{	NULL					      ,NULL  		,NULL			,wsk2_1	},
	{	NULL				         ,NULL  		,NULL			,wsk2_2	},
	{	NULL     				   ,NULL  		,NULL			,wsk2_3 },
	{	NULL					      ,NULL  		,NULL			,wsk2_4	},
	{	MENU_termination_item	,NULL  		,NULL			,NULL	}// This line terminate menu content 
};




