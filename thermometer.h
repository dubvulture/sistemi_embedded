#ifndef THERMOMETER_H
#define THERMOMETER_H

// I2C address
#define TER 0x48


/* Value where the temperature read is stored */
extern uint8_t Temp;


/*
 * Read the temperature into Temp
 */
void read_term(void);


#endif
