#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

// I2C address
#define ACC 0x4C
/* Mode required for SM_Send
 * Either send data to the accelerometer
 * or prepare to read from it */
#define ACC_READ 0x01
#define ACC_SEND 0x41


/* Buffers of the last 8 values of each angle of inclination */
extern int8_t Buffer[3][8];
/* Position indicating where to allocate the next set of angles' values */
extern uint8_t BuffIdx;
/* Init command */
extern uint8_t cmd_acc[4];
/* Lookup table for the angle values */
extern code int8_t LUT[65];


/*
 * Write registers to the accelerometer in order to initialize it
 */
void init_acc(void);
/*
 * Read values of accelerometer into the appropriate buffers
 */
void read_acc(void);


#endif
