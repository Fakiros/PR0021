#include <inttypes.h> 
#include <string.h> 
#include <avr/io.h> 
#include <avr/pgmspace.h>
#include <avr/wdt.h>


#include <stdlib.h>

     
#include "..\..\04_LIB\RKLib\07_SW\03_SRC\global.h"
#include "..\..\04_LIB\RKLib\07_SW\03_SRC\uart.h"

//#include "..\..\Libs\RKLib\src\uart0.h"
//#include "..\Libs\RKLib\conv.h"
//#include "uart.h"
//#include "..\..\03_SRC\fbus\n61sms.h"
#include "..\..\03_SRC\drv\Timers.h"
#include "..\..\03_SRC\drv\encoder.h"
#include "drv\AC_Line.h"

#include "drv\lcd.h"
#include "appl\scheduler.h"
#include "menu\menu_gen.h"
#include "menu\Menu_1.h"


#include"Nvm\nvm.h"

#include "delay.h"
#include "drv/DS18B20/ds18x20.h"
#include  "Temperature/Temperature.h"





#include "inverter\pwm.h"

unsigned int step2 = 50;
unsigned char dead_time = 0;




void FastTask(void);

void encoder_incr (void)
{
	step2++;
	PWM_SetValue(step2);
	lcd_gotoxy(0,1);
	lcd_ProgressBar(step2/4, 100, 20);
}

void encoder_decr (void)
{
	step2--;
	PWM_SetValue(step2);
	lcd_gotoxy(0,1);
	lcd_ProgressBar(step2/4, 100, 20);
}

void encoder2_incr (void)
{
	dead_time++;
	PWM_SetDeadTime(dead_time);
	lcd_gotoxy(0,2);
	lcd_ProgressBar(dead_time/4, 100, 20);
}

void encoder2_decr (void)
{
	dead_time--;
	PWM_SetDeadTime(dead_time);
	lcd_gotoxy(0,2);
	lcd_ProgressBar(dead_time/4, 100, 20);
}

//*/

void FastTask(void)
{
	encoder_debounce();
	encoder_handler();
	PushButton_handler();
	

	
}


int main(void)
{
	SchedulerInit();

	PWM_init();
    PWM_SetSin(50);
	encoder_init();
	PushButton_init();
	ACLine_init();
    //encoder_init_clbk(0, encoder_incr, encoder_decr);
	//encoder_init_clbk(1, encoder2_incr, encoder2_decr);
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_home();
	MENU_init(Menu1);
   Temp_init();
	
	 sei();

	lcd_puts("taki sobie");

	uint16_t KeyOnCounter ;

    NVM_ReadDirect( &KeyOnCounter, &NVM.KeyOnCounter, sizeof(KeyOnCounter));
	KeyOnCounter++;
	NVM_WriteDirect(&NVM.KeyOnCounter, &KeyOnCounter, sizeof(KeyOnCounter) );

	uint8_t i;

	for(i = 0; i < 7; i++)
	{
		lcd_LoadCustomChar(&LcdCustomChar[i][0], i);
	}

//	DDRD |= (1<<7);

	uint16_t loop_counter = 0;
	uint8_t char_counter, sec = 0;



	while(1)
	{
			loop_counter++;
			
	        char stre[17];


					
			//lcd_clrscr();

	//			PORTD |= (1 << 7);

			//PORTB &= ~(1 << 7);

//			if (loop_counter >= 200)
//			{
//				lcd_gotoxy(0,0);
//				(void)itoa(char_counter, stre, 16);
//				lcd_puts(stre);
//				lcd_puts(" = ");
//				lcd_putc(char_counter);
//				lcd_puts("     ");
//				char_counter++;
//				loop_counter = 0;
//				
//				
//			}
//			lcd_gotoxy(0,3);
//			lcd_puts("01234567890123456789");
	//		encoder_handler();
	//		PushButton_handler();
//			
	//		PORTD &= ~(1 << 7);
//			
			//PORTB &= ~(1 << 7);
		WaitForLoopFinished();
			//PORTB |= (1 << 7);
		Temp_handler();
		MENU_handler();
		ACLine_handler();
	}

}


