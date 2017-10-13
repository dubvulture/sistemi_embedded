#include "defs.h"

#include "accelerometer.h"
#include "flags.h"
#include "lcd.h"
#include "smbus.h"
#include "thermometer.h"


uint8_t FstLine[16] = {
//	   X    ws  ws/-     0     0     °    ws    ws
	0x58, 0x20, 0x30, 0x30, 0x30, 0xDF, 0x20, 0x20,
// 	  ws     Y    ws  ws/-     0     0     °    ws
	0x20, 0x59, 0x20, 0x30, 0x30, 0x30, 0xDF, 0x20
};
uint8_t SndLine[16] = {
//	   Z    ws  ws/-     0     0     °    ws    ws
	0x5A, 0x20, 0x30, 0x30, 0x30, 0xDF, 0x20, 0x20,
//	  ws    ws    ws  ws/-     0     0     °     C
	0x20, 0x20, 0x20, 0x20, 0x30, 0x30, 0xDF, 0x43
};
uint8_t cmd_lcd[8] = {0x38, 0x39, 0x14, 0x74, 0x54, 0x6F, 0x0C, 0x01};


void init_lcd(void) 
{
	SM_Send(LCD, cmd_lcd, 8, COM);
}

/*
 * - Unset this worker flag
 * - Write a blank character or a minus depending on the temperature sign
 * - Write angles' mean values
 * - Clear the display and returns the cursor at the beginning
 * - Send the text to the LCD
 *
 * Compiler optimization aside, here is asserted that before SM_Send no other
 * SMBUS interrupts will be processed, so that any of the values in Temp or
 * referenced by Buffer won't be modified.
 */
void write_lcd(void)
{
	int16_t tmp;
	uint8_t carriage_return = 0x80;
	uint8_t newline = 0xC0;

	FlagLcd = 0;

	SndLine[11] = (Temp & 0x80) ? 0x2D : 0x20;
	Temp &= 0x7F;
	SndLine[12] = 0x30 + (Temp / 10);
	SndLine[13] = 0x30 + (Temp % 10);

	tmp = REDUCE_SUM(Buffer[0]) >> 3;
	if (tmp & 0x80) {
		tmp = ~tmp + 1;
		FstLine[2] = 0x2D;
	} else {
		FstLine[2] = 0x20;
	}
	FstLine[3] = 0x30 + ((uint8_t)tmp / 10);
	FstLine[4] = 0x30 + ((uint8_t)tmp % 10);

	tmp = REDUCE_SUM(Buffer[1]) >> 3;
	if (tmp & 0x80) {
		tmp = ~tmp + 1;
		FstLine[11] = 0x2D;
	} else {
		FstLine[11] = 0x20;
	}
	FstLine[12] = 0x30 + ((uint8_t)tmp / 10);
	FstLine[13] = 0x30 + ((uint8_t)tmp % 10);

	tmp = REDUCE_SUM(Buffer[2]) >> 3;
	if (tmp & 0x80) {
		tmp = ~tmp + 1;
		SndLine[2] = 0x2D;
	} else {
		SndLine[2] = 0x20;
	}
	SndLine[3] = 0x30 + ((uint8_t)tmp / 10);
	SndLine[4] = 0x30 + ((uint8_t)tmp % 10);

	SM_Send(LCD, &carriage_return, 1, COM);
	SM_Send(LCD, FstLine, 16, DAT);
	SM_Send(LCD, &newline, 1, COM);
	SM_Send(LCD, SndLine, 16, DAT);
}
