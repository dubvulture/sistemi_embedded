/*
 * Timer0 is used to control the Led PWM and its duration is 
 * modified based on the to the duty cycle
 */

#ifndef TIMER0_H
#define TIMER0_H


/* PWM duty cycle */
extern uint8_t PWM;
/* Dimming "direction" */
extern int8_t Direction;


void setup_timer0(void);


#endif
