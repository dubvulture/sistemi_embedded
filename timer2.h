/*
 * Timer3 is used to set each worker task every:
 * - 100ms for the accelerometer
 * - 300ms for the display
 * - 1s for the thermometer
 */

#ifndef TIMER2_H
#define TIMER2_H

/* Counter used for the timer cycles (avoids overflows) */
extern uint8_t Timer2Counter;


void setup_timer2(void);
void start_timer2(void);


#endif
