/*
 * Timer3 is used in two different modes
 *   - Mode 0 (ButtonMode == 0)
 *     Switch to MODE1 after 1s (Timer3 might be stopped by the button routine)
 *   - Mode 1 (ButtonMode == 1)
 *     Every 10ms dim the Led
 */

#ifndef TIMER3_H
#define TIMER3_H


/* Counter used for the timer cycles (avoids overflows) */
extern uint8_t Timer3Counter;


void setup_timer3_mode0(void);
void setup_timer3_mode1(void);
void start_timer3(void);


#endif
