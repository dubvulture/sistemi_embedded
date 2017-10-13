#include "defs.h"

#include "accelerometer.h"
#include "flags.h"
#include "smbus.h"
#include "timer2.h"


int8_t Buffer[3][8];
uint8_t BuffIdx = 0;
/*
 * - Write 0s to the Interrupt Setup Register
 * - set Active Mode to Mode (autoincremented)
 * - set the Sample Rate to 16 det/s (autoincremented)
 */
uint8_t cmd_acc[4] = {0x06, 0x00, 0x01, 0x03};
code int8_t LUT[65] = {
// Positive angles
	  0,   3,   5,   8,  11,  14,  16,  19,  22,  25,  28,
	 31,  34,  38,  41,  45,  49,  53,  58,  63,  70,  80,
// Higher than 80 / Shaken
	 90,  90,  90,  90,  90,  90,  90,  90,  90,  90,
// Lower than -80 / Shaken
	-90, -90, -90, -90, -90, -90, -90, -90, -90, -90, -90,
// Negative angles
	-80, -70, -63, -58, -53, -49, -45, -41, -38, -34, -31,
	-28, -25, -22, -19, -16, -14, -11,  -8,  -5,  -3
};


void init_acc(void)
{
	SM_Send(ACC, cmd_acc, 4, ACC_SEND);
}

/*
 * - Unset this worker flag
 * - src indicates from which address/register to read
 *   0x00 corresponds to the X angle (followed by Y and Z)
 * - Setup DataRead manually since we need to use SM_Send first
 *   and rely SMBus interrupt tricks.
 * - Get the angles' values from the Lookup Table
 * - Save them to the appropriate buffer and advance its position
 */
void read_acc(void)
{
	uint8_t src = 0x00;
	uint8_t dest[3];
	
	FlagAcc = 0;
	DataRead = dest;
	SM_Send(ACC, &src, 1, ACC_READ);
	HALT_WHILE(SM_Busy);
	
	Buffer[0][BuffIdx] = LUT[dest[0] & 0x3F];
	Buffer[1][BuffIdx] = LUT[dest[1] & 0x3F];
	dest[2] &= 0x3F;
	if (dest[2] & 0x20)
		Buffer[2][BuffIdx] = ~(90 + LUT[dest[2]]) + 1;
	else
		Buffer[2][BuffIdx] = 90 - LUT[dest[2]];
	BuffIdx++;
	BuffIdx = BuffIdx % 8;
}
