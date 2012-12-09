#include <avr/pgmspace.h>
#include "drv/Push_button.h"
#include "NVM/nvm.h"
#include "Appl/Scheduler.h"



#define NULL	0
#define ACLINE_CROSS_PER_SEC 50u


unsigned long ACLine_SummaryTime_sec;


static void ACLine_cross(void);


void ACLine_init()
{
   	PushButton_init_clbk(2, ACLine_cross, NULL);
	NVM_ReadDirect( &ACLine_SummaryTime_sec, &NVM.ACLineTime, sizeof(ACLine_SummaryTime_sec));
}


static void ACLine_cross(void)
{
	static char i = 0;
	i++;
	if(i > ACLINE_CROSS_PER_SEC)
	{
		i = 0;
		ACLine_SummaryTime_sec++;
	}
}

void ACLine_handler(void)
{
	 static uint32_t cntr;
	 cntr++;
	 if(cntr > (1000 * 60 * 60 / MAIN_LOOP_PERIOD))
	 {
		cntr = 0;
		NVM_WriteDirect(&NVM.ACLineTime, &ACLine_SummaryTime_sec, sizeof(&ACLine_SummaryTime_sec) );
	 }
	 
}



