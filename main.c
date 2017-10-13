#include "c8051f020.h"
#include "defs.h"

#include "accelerometer.h"
#include "button.h"
#include "flags.h"
#include "lcd.h"
#include "thermometer.h"
#include "timer0.h"
#include "timer2.h"
#include "smbus.h"


/*
 * Initial setup of microcontroller
 */
void init(void)
{
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	// 00 - 2Mhz; 01 - 4Mhz; 02 - 8Mhz; 03 - 16Mhz;
	OSCICN |= 0x00;

	XBR0 = 0x05;
	XBR1 = 0x00;
	XBR2 = 0x40;

	// Enable SMBus with ACKs on acknowledge
	SMB0CN = 0x44;
	// SMBus clock rate.
	SMB0CR = -10;
	// SMBus interrupt enable
	EIE1 |= 0x02;
	
	P0MDOUT |= 0x40;
	Led = 0;

	init_button();
	setup_timer0();
	setup_timer2();
	start_timer2();

	// Global interrupt enable
	EA = 1;
}

void main (void)
{
	init();
	init_lcd();
	init_acc();
	
	while (1) {
		HALT_WHILE(!Ready);
		if (FlagAcc)
			read_acc();
		if (FlagTer)
			read_term();
		if (FlagLcd)
			write_lcd();
		Ready = 0;
	}
}

