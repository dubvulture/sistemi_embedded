#ifndef SMBUS_H
#define SMBUS_H

#include "c8051f020.h"
#include "defs.h"

// SMBus states:
#define SMB_BUS_ERROR 0x00
 // (MT & MR) START transmitted
#define SMB_START 0x08
 // (MT & MR) repeated START
#define SMB_RP_START 0x10
// (MT) Slave address + W transmitted; ACK received
#define SMB_MTADDACK 0x18
// (MT) Slave address + W transmitted; NACK received
#define SMB_MTADDNACK 0x20
// (MT) data byte transmitted; ACK received
#define SMB_MTDBACK 0x28
// (MT) data byte transmitted; NACK received
#define SMB_MTDBNACK 0x30
// (MT) arbitration lost;
#define SMB_MTARBLOST 0x38
// (MR) Slave address + R transmitted; ACK received
#define SMB_MRADDACK 0x40
// (MR) Slave address + R transmitted;
#define SMB_MRADDNACK 0x48
// (MR) data byte received; ACK received
#define SMB_MRDBACK 0x50
// (MR) data byte received; NACK received
#define SMB_MRDBNACK 0x58

// Values for masking the address of the device
#define WRITE 0xFE
#define READ 0x01


/* Flag set when the SMBUS is being used */
extern bit SM_Busy;
/* Holds the slave address + R/W bit */
extern uint8_t Slave;
/* Holds the value of the selected modes defined for LCD and Accelerometer */
extern uint8_t SM_Mode;
/* Pointer to the data to be written */
extern uint8_t *DataWrite;
/* Pointer where to store the data that will be received */
extern uint8_t *DataRead;
/* Both DataWrite and DataRead length counter */
extern uint16_t DataLen;


/* Send @len bytes from @src to @chip_select with selected @mode */
void SM_Send(uint8_t chip_select, uint8_t *src, uint16_t len, uint8_t mode);
/* Get @len bytes from @chip_select */
void SM_Get(uint8_t chip_select, uint8_t *dest, uint16_t len);


#endif
