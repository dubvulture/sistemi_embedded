#include "c8051f020.h"
#include "defs.h"
#include "timer0.h"


uint8_t PWM = 255;
int8_t Direction = 1;


/*
 * - Reset overflow flag and stop timer
 * - Set the high byte to 0xFF since we only need the low one
 * - Toggle the Led and set the new timer value based on the duty cycle
 * - Start the timer
 */
void timer0() interrupt 1
{
	TF0 = 0;
	TR0 = 0;
	TH0 = 0xFF;

	if (Led) {
		Led = 0;
		TL0 = PWM;	
	}
	else {
		Led = 1;
		TL0 = 0xFF - PWM;
	}

	TR0 = 1;
}


/*
 * Timer0 operates with the following configuration
 *   - Mode 1 (16bit)
 *   - Timer Clock = SysClock / 12
 *   - 1 Clock Timer to raise the interrupt asap
 */
void setup_timer0(void)
{
	TMOD |= 0x01;
	CKCON = 0;
	TL0 = 0xFF;
	TH0 = 0xFF;

	ET0 = 1;
}
