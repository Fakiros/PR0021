/* Main loop period [ticks] */
#define MAIN_LOOP_PERIOD	5

extern volatile unsigned int loop_timer;

extern void SchedulerInit();
extern void WaitForLoopFinished();
extern void PeriodicTask();
