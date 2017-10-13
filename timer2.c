#include "c8051f020.h"
#include "defs.h"

#include "flags.h"
#include "timer2.h"


uint8_t Timer2Counter = 0;


/*
 * - Reset overflow flag
 * - Increment the Timer2 counter
 * - Raise each flag according the the elapsed time
 * - Reset the Timer2 counter every second
 */
void timer2() interrupt 5
{
	TF2 = 0;

	Timer2Counter++;
	
	FlagAcc = 1;
	if (Timer2Counter % 3 == 0)
		FlagLcd = 1;
	if (Timer2Counter % 10 == 0)
		FlagTer = 1;
	
	if (Timer2Counter == 30)
		Timer2Counter = 0;
	
	Ready = 1;
}

/*
 * Timer0 operates with the following configuration
 *   - Mode 1 (16bit autoreload)
 *   - Timer Clock = SysClock / 12
 *   - 100ms Clock Timer
 */
void setup_timer2(void)
{
	T2CON = 0;
	TL2 = 0x96;
	TH2 = 0xBF;
	RCAP2L = TL2;
	RCAP2H = TH2;

	Timer2Counter = 0;
}

/*
 * - Enable Timer2 interrupts
 * - Start Timer2
 */
void start_timer2(void)
{
	ET2 = 1;
	TR2 = 1;
}
