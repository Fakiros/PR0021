
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "menu/menu_gen.h"
#include "drv/encoder.h"
#include "drv/lcd.h"





typedef void func_t(void);

menu_statuss_t 	    menu_statuss;

//uint8_t 	menu_item[MENU_LEVELS];


uint8_t	menu_level;



static void menu_up(void);
static void menu_down(void);
static void menu_in(void);
static void menu_out(void);

static void menu_update_display(void);


/****************************************************************************/
/*                                MenuItem Events                           */
/****************************************************************************/

void MENU_init(menu_t * menu_p)
{
	   MENU_KeyboardInit();
	   menu_level = 0;
	   menu_statuss.Item[menu_level] = 0;	   
	   menu_statuss.Pointer[menu_level] = menu_p;
	   menu_update_display();
}

void MENU_KeyboardInit(void)
{
	encoder_init_clbk(0, menu_down, menu_up);
	encoder_init_clbk(1, menu_out, menu_in);
	PushButton_init_clbk(0, menu_in, NULL);
	PushButton_init_clbk(1, menu_out, NULL);
}

static void menu_up(void)
{
	typedef void (*FuncExe_t)(void);
	FuncExe_t FuncExe;
	uint8_t menu_current_item;
	menu_t	*menu_current_p;

	menu_statuss.Item[menu_level]++;
	menu_current_item = menu_statuss.Item[menu_level];
	menu_current_p = menu_statuss.Pointer[menu_level];
	FuncExe = (FuncExe_t)pgm_read_word(&menu_current_p[menu_current_item].FuncExe);
	
	if(FuncExe == MENU_termination_item)
	{
		menu_statuss.Item[menu_level]--;
		MENU_termination_item();
	}
	menu_update_display();
} 

static void menu_down(void)
{
	if(menu_statuss.Item[menu_level] > 0)
	{
		menu_statuss.Item[menu_level]--;
	}
	menu_update_display();
}

/*push button - enter */
static void menu_in(void)
{
	menu_t	*menu_in_p;
	typedef void (*FuncExe_t)(void);
	FuncExe_t FuncExe;

	uint8_t menu_current_item;
	menu_t	*menu_current_p;

	menu_current_item = menu_statuss.Item[menu_level];
	menu_current_p = menu_statuss.Pointer[menu_level];
	
	menu_in_p =(menu_t *) pgm_read_word(&menu_current_p[menu_current_item].MenuIn);
	FuncExe = (FuncExe_t)pgm_read_word(&menu_current_p[menu_current_item].FuncExe);
	
	if( menu_in_p != NULL)
	{
		menu_level++;
		menu_statuss.Pointer[menu_level] = menu_in_p;
		menu_statuss.Item[menu_level] = 0;
	}

	if(FuncExe != NULL)
	{
		FuncExe();
	}
	menu_update_display();
}

/*push button - escape */

static void menu_out(void)
{
	if (menu_level > 0)
	{
		menu_level--;
	}
	menu_update_display();
}

static void menu_update_display(void)
{

	uint8_t	level;
	menu_t	*menu_p;
	uint8_t	menu_item;

	uint8_t menu_current_item;
	menu_t	*menu_current_p;

	menu_current_item = menu_statuss.Item[menu_level];
	menu_current_p = menu_statuss.Pointer[menu_level];
	
	//volatile  uint8_t *wsk;
	lcd_gotoxy(0,0);
	lcd_puts("                    ");
	lcd_gotoxy(0,0);
	for(level = 0; level <= menu_level; level++)
	{
		menu_p = menu_statuss.Pointer[level];
		menu_item = menu_statuss.Item[level];

		lcd_putc(0x7E); // Right arrow
		lcd_puts_p((char*)pgm_read_word(&(menu_p[menu_item].display)));
	}


	//lcd_puts_p((char*)pgm_read_word(&(menu_current_p[menu_current_item].display)));

}

void MENU_handler(void)
{	

	typedef void (*FuncExe_t)(void);
	FuncExe_t FuncExe;

	uint8_t menu_current_item;
	menu_t	*menu_current_p;

	menu_current_item = menu_statuss.Item[menu_level];
	menu_current_p = menu_statuss.Pointer[menu_level];
	
	FuncExe = (FuncExe_t)pgm_read_word(&menu_current_p[menu_current_item].HandlerFuncExe);
	if(FuncExe != NULL)
	{
		FuncExe();
	}
}


/****************************************************************************/
/*                                MenuItem Events                           */
/****************************************************************************/
void MENU_termination_item(void)
{
	
}




//void menu_handler(void)
//{

//}


