/*
 * RC522_def.h
 *
 *  Created on: 15 lis 2020
 *      Author: andre
 */

#ifndef RC522_DEF_H_
#define RC522_DEF_H_

#define MF_KEY_SIZE 6

/*--------------------------------------REGISTERS------------------------------------------*/
#define CommandReg 		(0x01<<1)
#define TModeReg 		(0x2A<<1)
#define TPrescalerReg 	(0x2B<<1)
#define TReloadRegH 	(0x2C<<1)
#define TReloadRegL 	(0x2D<<1)
#define TxASKReg 		(0x15<<1)
#define ModeReg 		(0x11<<1)
#define TxControlReg 	(0x14<<1)
#define VersionReg 		(0x37<<1)
#define TxModeReg 		(0x12<<1)
#define RxModeReg 		(0x13<<1)
#define ModWidthReg 	(0x24<<1)
#define ComIrqReg		(0x04<<1)
#define FIFOLevelReg 	(0x0A<<1)
#define FIFODataReg		(0x09<<1)
#define BitFramingReg	(0x0D<<1)
#define ErrorReg		(0x06<<1)
#define CollReg 		(0x0E<<1)
#define DivIrqReg		(0x05<<1)
#define CRCResultRegL 	(0x22<<1)
#define CRCResultRegH 	(0x21<<1)
#define ControlReg 		(0x0C<<1)
#define ComIEnReg 		(0x02<<1)
#define Status2Reg 		(0x08<<1)

/*--------------------------------------COMMANDS-------------------------------------------	*/
#define SoftReset 				(0x0F)
#define PCD_Idle				(0x00)
#define PCD_CalcCRC 			(0x03)
#define PCD_Transceive 			(0x0C)
#define PICC_CMD_REQA 			(0x26)
#define PICC_CMD_SEL_CL1 		(0x93)
#define PICC_CMD_SEL_CL2 		(0x95)
#define PICC_CMD_SEL_CL3 		(0x97)
#define PICC_CMD_CT 			(0x88)
#define PICC_CMD_HLTA 			(0x50)
#define PCD_MFAuthent 			(0x0E)
#define PICC_CMD_MF_READ 		(0x30)
#define PICC_CMD_MF_AUTH_KEY_A 	(0x60)
#endif /* RC522_DEF_H_ */
