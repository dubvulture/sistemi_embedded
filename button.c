#include "c8051f020.h"
#include "defs.h"

#include "button.h"
#include "timer3.h"


uint8_t ButtonMode = 0;


/*
 * Interrupt routine for the button on P3.7
 *  - Reset int flag
 *  - If mode == 0
 *    (when toggling Led on/off and using timer3 to count 1s to switch mode)
 *    - If on falling edge
 *      - Start timer3 and set P3IF to raise an interrupt on raising edge
 *    - Elif on raising edge
 *      - Stop timer3
 *      - Toggle timer0 that controls the PWM for the Led 
 *      - Set P3IF to raise an interrupt on falling edge
 *  - Elif ButtonMode == 1 and RISING (there is no other combination)
 *    (during which the Led is dimming)
 *      - Only during rising edge stop timer3 and switch back to mode 0
 *        (this means the button has been released and the user no longer
 *         wants to dim the Led)
 */
void interrupt_button(void) interrupt 19
{
	P3IF &= 0x7F;

	if (ButtonMode == 0) {
		if (FALLING) {
			setup_timer3_mode0();
			start_timer3();
			P3IF |= 0x08;
		} else { // (RISING)
			TMR3CN &= 0xFB;
			
			if (TR0) {
				TF0 = 0;
				Led = 0;
			}
			TR0 = ~TR0;

			P3IF &= 0xF7;
		}
	} else { // ButtonMode == 1 and RISING
		TMR3CN &= 0xFB;
		ButtonMode = 0;
		P3IF &= 0xF7;
	}
}


/*
 * Set the interrupt to be raised as soon as the button is pressed
 * and enable the corrisponding interrupt flag
 */
void init_button(void)
{
	P3IF = 0x00;
	EIE2 |= 0x20;
}
