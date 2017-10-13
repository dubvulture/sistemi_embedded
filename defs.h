#ifndef DEFS_H
#define DEFS_H

#include "c8051f020.h"

#define HALT_WHILE(x) \
	while(x) { PCON |= 0x01; }

typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;

sbit Button = P3^7;
sbit Led = P0^6;


#endif
