#ifndef _pwm_h_
#define _pwm_h_

#define PWM_FULL_PERIOD 1

extern  void PWM_init(void);
extern  void PWM_SetValue(unsigned int value);
extern  void PWM_SetSin(unsigned int freq);
extern	void PWM_Stop(void);
	

#endif /* _pwm_h_ */
