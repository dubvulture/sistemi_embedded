#ifndef LCD_H
#define LCD_H

#include "defs.h"

#define LCD 0x3E
#define COM 0x00
#define DAT 0x40

#define REDUCE_SUM(x) \
	((int16_t)x[0] + (int16_t)x[1] + (int16_t)x[2] + (int16_t)x[3] + \
	 (int16_t)x[4] + (int16_t)x[5] + (int16_t)x[6] + (int16_t)x[7])


/* First Line pattern to send to the LCD */
extern uint8_t FstLine[16];
/* Second Line pattern to send to the LCD */
extern uint8_t SndLine[16];
/* Init command for the LCD */
extern uint8_t cmd_lcd[8];


/*
 * Send instructions to the LCD in order to initialize it
 */
void init_lcd(void);
/*
 * Routine that sends information regarding temperature and the mean
 * of the 8 last angles of inclination
 */
void write_lcd(void);


#endif
