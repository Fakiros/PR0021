#include <avr/pgmspace.h>

#define ENCODERS_NO 2
#define ENC_PULSES_PER_EVENT	4
//typedef (void func_tt (void)) func_t;

#define ENC_DEBOUNCE_CYCLES		2u


typedef struct
{
	unsigned char current_input;
	unsigned char previous_input;
	unsigned char current_state;
	unsigned char last_state;
}encoder_state_t;

typedef struct
{
	unsigned char *SetRegA;
	unsigned char *ReadRegA;
	unsigned char *DirRegA;
	unsigned char PinNoA;
	unsigned char *SetRegB;
	unsigned char *ReadRegB;
	unsigned char *DirRegB;
	unsigned char PinNoB;
}encoder_cfg_t;

typedef struct
{
	void * FuncIncr;
	void * FuncDecr;
}encoder_clbk_t;


 PROGMEM  encoder_cfg_t encoder_cfg[ENCODERS_NO] =
{ //	SetRegA	|	ReadRegA	|	DirEgA	|	PinNoA
	{	&PORTD,	&PIND,	&DDRD,	6,	&PORTD,	&PIND,	&DDRD,	7 },
	{	&PORTC,	&PINC,	&DDRC,	1,	&PORTC,	&PINC,	&DDRC,	0 },

};

encoder_clbk_t encoder_clbk[ENCODERS_NO];
encoder_state_t encoder_state[ENCODERS_NO];


void encoder_init_pins(unsigned char EncoderNo,	void * const FuncIncr(void),  void * const FuncDecr(void));
void encoder_handler(void);


 
PROGMEM  unsigned char encoder_steps[4] = 
{
	2, 3, 1, 0
};

//unsigned char Prt, Prt_prev;
//unsigned char Bfr[1000];
//unsigned char *Bfr_p = Bfr;


void encoder_init()
{
   unsigned char i;
   for(i=0; i < ENCODERS_NO ; i++)
   {
	   *(unsigned char *)pgm_read_byte(&encoder_cfg[i].DirRegA) &= ~( 1 << pgm_read_byte(&encoder_cfg[i].PinNoA));
	   *(unsigned char *)pgm_read_byte(&encoder_cfg[i].DirRegB) &= ~( 1 << pgm_read_byte(&encoder_cfg[i].PinNoB));
	   *(unsigned char *)pgm_read_byte(&encoder_cfg[i].SetRegA)	|= (1 << pgm_read_byte(&encoder_cfg[i].PinNoA));
	   *(unsigned char *)pgm_read_byte(&encoder_cfg[i].SetRegB)	|= (1 << pgm_read_byte(&encoder_cfg[i].PinNoB));
	}
}


void encoder_init_clbk( unsigned char EncoderNo,  void * const FuncIncr(void), void * const FuncDecr(void))
{
	unsigned char i = EncoderNo;
	encoder_clbk[EncoderNo].FuncIncr = FuncIncr;
	encoder_clbk[EncoderNo].FuncDecr = FuncDecr;
}



void encoder_debounce(void)
{
	static unsigned char Prt_prev;
	unsigned char Prt_current;
	unsigned char i;
	static unsigned char counter;
	
	for(i = 0; i < ENCODERS_NO; i++)
	{	
		unsigned char PinNoA = pgm_read_byte(&encoder_cfg[i].PinNoA);
		unsigned char PinNoB = pgm_read_byte(&encoder_cfg[i].PinNoB);

		Prt_current = ((  (*(unsigned char *)pgm_read_byte(&encoder_cfg[i].ReadRegA)) & (1 << PinNoA)) >> PinNoA );

		if(0 != PinNoB)
		{
			//Prt_current &= ~();
			Prt_current |= ((  (*(unsigned char *)pgm_read_byte(&encoder_cfg[i].ReadRegB)) & (1 << PinNoB)) >> (PinNoB - 1) );
		}
		else
		{
			Prt_current |= (( (*(unsigned char *)pgm_read_byte(&encoder_cfg[i].ReadRegB)) & 1 ) << 1 );
		}
		

		//Prt_current = (PIND & ((1 << 6) | (1 << 7))) >> 6;
		if(Prt_current == encoder_state[i].previous_input)
		{	
			
			if(counter < ENC_DEBOUNCE_CYCLES)
			{
				counter++;
			}
			else
			{
				encoder_state[i].current_input = Prt_current;
			}
		}
		else
		{
			encoder_state[i].previous_input = Prt_current;
			counter = 0;
		}
	}
}



void encoder_handler(void)
{

	static unsigned char Previous_input[ENCODERS_NO];
	static unsigned char Previous_state[ENCODERS_NO];
	static signed char pulses_counter;
	unsigned char i;
	void (*func)(void);
	
	
	
	for(i = 0; i < ENCODERS_NO; i++)
	{
		if (encoder_state[i].current_input != Previous_input[i])
		{
			unsigned char	state;
			unsigned char	state_up;
			unsigned char	state_down;
		
			state = pgm_read_byte(&encoder_steps[encoder_state[i].current_input]);
			state_up = (state + 1) & 0x03;
			state_down = (state + 3 ) & 0x03;
			if (Previous_state[i]== state_down)
			{
				pulses_counter++;
				if(pulses_counter >= ENC_PULSES_PER_EVENT)
				{
					func =  encoder_clbk[i].FuncIncr;
					if (func != 0) {func();}
					pulses_counter = 0;
				}
			}
		
			if(Previous_state[i] == state_up)
			{			
				pulses_counter--;
				if(pulses_counter <= 0)
				{
					func =  encoder_clbk[i].FuncDecr;
					if (func != 0) {func();}
					pulses_counter = ENC_PULSES_PER_EVENT;
				}
			}
			
			
			
			Previous_state[i] = state;
			Previous_input[i] = encoder_state[i].current_input;
		}
	}
}
