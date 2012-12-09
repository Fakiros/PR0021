#ifndef MENU_GEN_H
#define MENU_GEN_H

#include <avr/pgmspace.h>


#define MENU_LEVELS		2
#define MENU_ITEMS		3

#define NULL	0

typedef void (*FuncExe_t)(void);


typedef struct menu_s
{
	void	* FuncExe;
	void	* HandlerFuncExe;
	void 	* MenuIn;
	 char			*  display;
}menu_t;


typedef struct
{
	uint8_t		Level;
	uint8_t		Item[MENU_LEVELS];
	void		*Pointer[MENU_LEVELS];
}menu_statuss_t;


extern void MENU_init(menu_t * menu_p);
extern void MENU_KeyboardInit(void);

extern void MENU_handler(void);

extern void  MENU_termination_item(void);



#endif 
