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

static char PROGMEM wsk2_1[] PROGMEM = "MENU_2_1\0"; 
static char PROGMEM wsk2_2[] PROGMEM = "MENU_2_2\0"; 
static char PROGMEM wsk2_3[] PROGMEM = "MENU_2_3\0"; 
static char PROGMEM wsk2_4[] PROGMEM = "MENU_2_4\0"; 


PROGMEM  menu_t menu2[] =
{ 
	//	exe funct				   , handler	,menu ptr	, display
	{	NULL					      ,NULL  		,NULL			,wsk2_1	},
	{	BarGraph				      ,NULL  		,NULL			,wsk2_2	},
	{	menu_g2_exe				   ,NULL  		,NULL			,wsk2_3 },
	{	NULL					      ,NULL  		,NULL			,wsk2_4	},
	{	MENU_termination_item	,NULL  		,NULL			,NULL	}// This line terminate menu content 
};


static char PROGMEM disp_ustawienia[] PROGMEM = "Ustawienia \0"; 
static char PROGMEM disp_OdczytParam[] PROGMEM = "Odczyt Parametrow \0"; 
static char PROGMEM wsk3[] PROGMEM = "Licznik Wegla \0"; 


PROGMEM  menu_t Menu1[] =
{ 
	//	exe funct				   , handler		,menu ptr			            , display
	{	NULL					      ,NULL  			,Menu_Ustawienia 				   ,disp_ustawienia	},
	{	NULL					      ,NULL  			,Menu_OdczytParam             ,disp_OdczytParam	},
	{	menu_g_exe			   	,menu_g3_exe	,menu2         	            ,wsk3 },
	{	MENU_termination_item	,NULL  			,NULL				               ,NULL	} // This line terminate menu content

};








void menu_g_exe(void)
{
	char string[17];
	char * ptr;
	//ptr = ultoa	(	ACLine_SummaryTime_sec , &string[0],  10u);
	
    //lcd_gotoxy(3,3);
	//lcd_puts(ptr);
	
    lcd_gotoxy(4,4);
	lcd_puts("to tak");
}

void menu_g2_exe(void)
{
    lcd_gotoxy(4,4);
	lcd_puts("          ");
}

void menu_g3_exe(void)
{
	char string[17];
	char * ptr;
	ptr = ultoa	(	ACLine_SummaryTime_sec , &string[0],  10u);
	
    lcd_gotoxy(3,3);
	lcd_puts(ptr);


}


void BarGraph(void)
{
	encoder_init_clbk(0, DownBar, UpBar);
	encoder_init_clbk(1, Escape, NULL);
	PushButton_init_clbk(0, NULL, NULL);
	PushButton_init_clbk(1, Escape, NULL);

	lcd_gotoxy(2,2);
	lcd_ProgressBar(BarGraph_level, 50, 10);
}

void UpBar(void)
{
	if(BarGraph_level < 50)
	  BarGraph_level++;
	lcd_gotoxy(2,2);
	lcd_ProgressBar(BarGraph_level, 50, 10);
	PWM_SetValue(BarGraph_level);
}

void DownBar(void)
{
   if(BarGraph_level > 0)
	BarGraph_level--;
   lcd_gotoxy(2,2);
	lcd_ProgressBar(BarGraph_level, 50, 10);
	PWM_SetValue(BarGraph_level);
}

void Escape(void)
{

	MENU_KeyboardInit();
	lcd_gotoxy(2,2);
	lcd_puts("                ");
}

