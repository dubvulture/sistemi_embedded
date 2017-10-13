#include "c8051f020.h"
#include "defs.h"

#include "accelerometer.h"
#include "lcd.h"
#include "smbus.h"


bit SM_Busy = 0;
uint8_t Slave = 0;
uint8_t SM_Mode = 0;
uint8_t *DataWrite;
uint8_t *DataRead;
uint16_t DataLen = 0;

/*
 * SMBus interrupt service routine:
 * - START
 *   - Send the slave address previously masked
 * - REPEATED START
 *   - Send again the slave address (used for the accelerometer)
 * - MT ADDRESS ACK
 *   - Clear START
 *   - If mode is for the LCD
 *     - Send the mode byte
 *   - Else
 *     - Already start sending data bytes
 * - MT DATA ACK
 *   - If mode is for the accelerometer
 *     - Setup again DataLen and the Slave address for reading
 *   - Elif DataLen > 0
 *     - Keep sending data bytes
 *   - Elif DataLen == 0
 *     - Stop the communication and free the SMBUS
 * - MR ADDRESS ACK
 *   - Just clear START
 * - MR DATA ACK
 *   - If DataLen > 0
 *     - Start/Keep reading data
 *   - Else
 *     - Set AA to 0 in order to receive a NACK
 * - MR DATA NACK and DEFAULT
 *   - Stop the communication and free the SMBUS
 */
void SMBUS_ISR (void) interrupt 7
{
	switch (SMB0STA){
		case SMB_START:
		case SMB_RP_START:
			SMB0DAT = Slave;
			break;
		/*
		 * MASTER TRANSMITTER - DISPLAY / ACCELEROMETER
		*/
		case SMB_MTADDACK:
			STA = 0;
			if (SM_Mode == COM || SM_Mode == DAT) {
				SMB0DAT = SM_Mode;
			} else {
				SMB0DAT = *DataWrite;
				DataWrite++;
				DataLen--;
			}
			break;
		case SMB_MTDBACK:
			if (SM_Mode == ACC_READ) {
				Slave |= READ;
				DataLen = 3;
				STA = 1;
			} else if (DataLen) {
				SMB0DAT = *DataWrite;
				DataWrite++;
				DataLen--;
			} else {
				STO = 1;
				SM_Busy = 0;
			}
			break;
		/*
		 * MASTER RECEIVER - THERMOMETER / ACCELEROMETER
		*/
		case SMB_MRADDACK:
			STA = 0;
			break;
		case SMB_MRDBACK:
			if (DataLen) {
				*DataRead = SMB0DAT;
				DataRead++;
				DataLen--;
			}
			else {
				AA = 0;
			}
			break;
		case SMB_MRDBNACK:
		// Otherwise reset communication
		default:
			STO = 1;
			SM_Busy = 0;
			break;
	}
	SI=0;
}


/*
 * - Wait for the SMBUS to be freed and lock it
 * - Mask @chip_select for the WRITE command
 * - Setup @mode, @src and @len
 * - Send the start signal
 */
void SM_Send(uint8_t chip_select, uint8_t *src, uint16_t len, uint8_t mode)
{
	HALT_WHILE(SM_Busy);
	SM_Busy = 1;
	SMB0CN = 0x44;
	Slave = (chip_select << 1) & WRITE;
	SM_Mode = mode;
	DataWrite = src;
	DataLen = len;
	STO = 0;
	STA = 1;
}

/*
 * - Wait for te SMBUS to be freed and lock it
 * - Mask @chip_select for the READ command
 * - Setup @dest and @len
 * - Send the start signal
 * - Wait for the data to be received
 */
void SM_Get(uint8_t chip_select, uint8_t *dest, uint16_t len)
{
	HALT_WHILE(SM_Busy);
	SM_Busy = 1;
	SMB0CN = 0x44;
	Slave = (chip_select << 1) | READ;
	DataRead = dest;
	DataLen = len;
	STO = 0;
	STA = 1;
	HALT_WHILE(SM_Busy);
}
