#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>


#include "drv/AC_line.h"
#include  "Temperature/Temperature.h"


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

static char PROGMEM disp_CoalCounter[] PROGMEM = "Licznik Wegla(imp)\0"; 
static char PROGMEM disp_CoalKg[] PROGMEM      = "Wegiel (kg/dobe)\0"; 
static char PROGMEM disp_TempOut[] PROGMEM     = "Temperatura Out\0"; 
static char PROGMEM disp_TempAvg[] PROGMEM     = "Temp Out Srednia (doba)\0"; 
static char PROGMEM disp_Efficiency[] PROGMEM  = "Sprawnosc (doba)\0";

static void cyclic_CoalCountert(void);
static void cyclic_TempOut(void);


PROGMEM  menu_t Menu_OdczytParam[] =
{ 
	//	exe funct				   , handler	    ,menu ptr	, display
	{	NULL					      ,cyclic_CoalCountert ,NULL			,disp_CoalCounter	},
	{	NULL				         ,NULL  		         ,NULL			,disp_CoalKg	   },
	{	NULL	         		   ,cyclic_TempOut      ,NULL			,disp_TempOut     },
	{	NULL					      ,NULL  		         ,NULL			,disp_TempAvg	   },
   {	NULL					      ,NULL  		         ,NULL			,disp_Efficiency	},
	{	MENU_termination_item	,NULL  		         ,NULL			,NULL	            }// This line terminate menu content 
};


static void cyclic_CoalCountert(void)
{
	char string[17];
	char * ptr;
	ptr = ultoa	(	ACLine_SummaryTime_sec , &string[0],  10u);
	
    lcd_gotoxy(3,3);
	lcd_puts(ptr);


}




static void cyclic_TempOut(void)
{
	char string[5];
	char * ptr;
	ptr = utoa	(	Temp_decicelsius , &string[0],  10u);
	
    lcd_gotoxy(3,3);
	lcd_puts(ptr);


}





