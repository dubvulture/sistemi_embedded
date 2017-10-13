#include "c8051f020.h"
#include "defs.h"

#include "flags.h"
#include "smbus.h"
#include "thermometer.h"
#include "timer2.h"


uint8_t Temp = 0;


/*
 * - Unset this worker flag
 * - Get the currente temperature
 * Here's what sits in dest (s = sign, i = integer, d = decimal)
 * dest[0] = siii iiii
 * dest[1] = iddd d---
 * - Extract integer part
 * - If temperature is negative:
 *   - Complement integer part
 *	 - Two complement decimal part
 *   - Increment temperature if decimal part > 0.0
 *   - Add sign
 * - Round up if decimal part > 0.5
 *
 * Compiler optimization aside, in this function is asserted that after SM_Get
 * no other SMBUS interrupts will be processed, so that the values referenced
 * by DataRead won't be modified.
 */
void read_term(void)
{
	uint8_t dest[2];
	
	FlagTer = 0;
	SM_Get(TER, dest, 2);

	Temp = (dest[0] << 1) | (dest[1] >> 7);

	if (dest[0] & 0x80) {
		Temp = ~Temp;
		dest[1] = ~dest[1] + 1;
		Temp += (dest[1] & 0x78) ? 0 : 1;
		Temp |= 0x80;
	}
	Temp += (dest[1] & 0x40) >> 6;
}
