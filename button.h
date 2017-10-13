#ifndef BUTTON_H
#define BUTTON_H

#include "c8051f020.h"

#define RISING (P3IF == 0x08)
#define FALLING (P3IF == 0x00)


/* Button/Led mode (either toggle or dimming) */
extern uint8_t ButtonMode;


void init_button(void);


#endif
