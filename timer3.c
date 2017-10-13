#include "c8051f020.h"
#include "defs.h"

#include "button.h"
#include "timer0.h"
#include "timer3.h"


uint8_t Timer3Counter = 0;


/*
 * - Reset overflow flag
 * - If ButtonMode == 0
 *    - Increment the Timer3 counter
 *		- If counter == 5 (100ms)
 *			- Switch to mode 1
 *			- Setup Timer3 accordingly
 *			- Start Timer0 for dimming
 * - Elif ButtonMode == 1
 *   - Dim the Led according to direction
 *   - Switch direction and correct overflow when duty cycle == 0
 */
void timer3() interrupt 14
{
	TMR3CN &= 0x7F;

	if (ButtonMode == 0) {
		Timer3Counter++;
		if (Timer3Counter == 5) {
			ButtonMode = 1;
			setup_timer3_mode1();
			start_timer3();
			TR0 = 1;
		}
	} else { // ButtonMode == 1
		PWM += Direction;
		if (PWM == 0) {
			// correct overflow
			PWM = 255*Direction;
			Direction ^= 0xFE;
		}
	}
}

/*
 * Timer3 operates with the following configuration
 *	- Mode 1 (16bit autoreload)
 *	- Timer Clock = SysClock / 12
 *	- 200ms Clock Timer
 */
void setup_timer3_mode0(void)
{
	TMR3CN = 0;
	TMR3L = 0x2C;
	TMR3H = 0x7F;
	TMR3RLL = TMR3L;
	TMR3RLH = TMR3H;

	Timer3Counter = 0;
}

/*
 * Timer3 operates with the following configuration
 *	- Mode 1 (16bit autoreload)
 *	- Timer Clock = SysClock / 12
 *	- 10ms Clock Timer
 */
void setup_timer3_mode1(void)
{
	TMR3CN = 0;
	TMR3L = 0x7C;
	TMR3H = 0xF9;
	TMR3RLL = TMR3L;
	TMR3RLH = TMR3H;
}

/*
 * - Enable Timer3 interrupts
 * - Start Timer3
 */
void start_timer3(void)
{
	EIE2 |= 0x01;
	TMR3CN |= 0x04;
}
