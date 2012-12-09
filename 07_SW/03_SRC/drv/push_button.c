#include <avr/pgmspace.h>

/* No of Buttons */
#define PBUTTONS_NO 3




typedef struct
{
	unsigned char *SetReg;
	unsigned char *ReadReg;
	unsigned char *DirReg;
	unsigned char PinNo;
	unsigned char DebounceNo;
}PushButton_cfg_t;

typedef struct
{
	void * FuncOnEvent;
	void * FuncOffEvent;;
}PushButton_clbk_t;


 PROGMEM  PushButton_cfg_t PushButton_cfg[PBUTTONS_NO] =
{ 
	{	&PORTA,	&PINA,	&DDRA,	0 , 20},
	{	&PORTA,	&PINA,	&DDRA,	1 , 20},
	{	&PORTD,	&PIND,	&DDRD,	2 , 2 },   /* AC Line */
};

PushButton_clbk_t PushButton_clbk[PBUTTONS_NO];



void PushButton_init()
{
   unsigned char i;
   for(i=0; i < PBUTTONS_NO ; i++)
   {
	   *(unsigned char *)pgm_read_byte(&PushButton_cfg[i].DirReg) &= ~( 1 << pgm_read_byte(&PushButton_cfg[i].PinNo));
	   *(unsigned char *)pgm_read_byte(&PushButton_cfg[i].SetReg) |= (1 << pgm_read_byte(&PushButton_cfg[i].PinNo));
   }
}


void PushButton_init_clbk( unsigned char PButtonNo,  void * const FuncOnEvent(void), void * const FuncOffEvent(void))
{

	PushButton_clbk[PButtonNo].FuncOnEvent = FuncOnEvent;
	PushButton_clbk[PButtonNo].FuncOffEvent = FuncOffEvent;
}


void PushButton_handler(void)
{
	static unsigned char Prt_prev[PBUTTONS_NO];
	static unsigned char Prt_cntr[PBUTTONS_NO]; 
	static unsigned char funcExec[PBUTTONS_NO];
	unsigned char Prt_current;
	unsigned char i;
	void (*func)(void);
	
	for(i = 0; i < PBUTTONS_NO; i++)
	{	
		unsigned char PinNo = pgm_read_byte(&PushButton_cfg[i].PinNo);
		unsigned char DebounceNo = pgm_read_byte(&PushButton_cfg[i].DebounceNo);


		Prt_current = ((  (*(unsigned char *)pgm_read_byte(&PushButton_cfg[i].ReadReg)) & (1 << PinNo)) >> PinNo );

		
		if(Prt_current != Prt_prev[i]) // Port state changed
		{	
		    Prt_cntr[i] = 0u; //reset debounce counter
		    funcExec[i] = 1u; // Execute function
		}
		else
		{
			if( Prt_cntr[i] < DebounceNo) // debounce time not expired
			{
				Prt_cntr[i]++;
			}
			else if (1u == funcExec[i]) 
			{
				funcExec[i] = 0; // stop function execution
				if(Prt_current > 0) //Switched on
				{
					func =  PushButton_clbk[i].FuncOnEvent;
					if (func != 0) {func();}
				}
				else // Switched off
				{
					func =  PushButton_clbk[i].FuncOffEvent;
					if (func != 0) {func();}
				}
				Prt_cntr[i] = 0u; 
			}
		}
		Prt_prev[i] = Prt_current;

	}
}
