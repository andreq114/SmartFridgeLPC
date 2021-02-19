/*
 * RC522.h
 *
 *  Created on: 15 lis 2020
 *      Author: andre
 */

#ifndef RC522_H_
#define RC522_H_

#include "RC522_def.h"

#include "fsl_gpio.h"
#include "fsl_spi.h"
#include "fsl_debug_console.h"
#include "lvgl.h"

enum StatusCode{
		STATUS_OK				,	// Success
		STATUS_ERROR			,	// Error in communication
		STATUS_COLLISION		,	// Collission detected
		STATUS_TIMEOUT			,	// Timeout in communication.
		STATUS_NO_ROOM			,	// A buffer is not big enough.
		STATUS_INTERNAL_ERROR	,	// Internal error in the code. Should not happen ;-)
		STATUS_INVALID			,	// Invalid argument.
		STATUS_CRC_WRONG		,	// The CRC_A does not match
		STATUS_MIFARE_NACK		= 0xff	// A MIFARE PICC responded with NAK.
};

typedef struct {
	uint8_t size; // Number of bytes in the UID. 4, 7 or 10.
	uint8_t uidByte[10];
	uint8_t sak; // The SAK (Select acknowledge) byte returned from the PICC
				 // after successful selection.
} Uid;

typedef struct {
	uint8_t keyByte[MF_KEY_SIZE];
} MIFARE_KEY;


void setConsoleLabel2(lv_obj_t *labelek);
void dispPRINTFth(const char *text);


void RC522_Init(SPI_Type *base,int rstPort,int rstPin);

void RC522_ReadDataFromRegister(uint8_t reg,
								uint8_t *data,
								uint8_t count,
								uint8_t rxAlign);
void RC522_WriteByteToRegister(uint8_t reg,uint8_t bajt);

uint8_t RC522_ReadByteFromRegister(uint8_t reg);


void RC522_TurnOnAntena();
void RC522_SoftReset();
void RC522_GetFirmwareVersion();
void checkStatus(enum StatusCode result);


bool RC522_IsNewCardPresent();
enum StatusCode RC522_RequestA(uint8_t *bufferATQA,uint8_t *buffLen);
enum StatusCode RC522_REQAorWUPA(uint8_t command,uint8_t *bufferATQA,uint8_t *buffLen);

void RC522_ClearRegisterMask(uint8_t reg,uint8_t mask);
void RC522_SetRegisterBitMask(uint8_t reg,uint8_t mask);

enum StatusCode RC522_TransceiveData1(uint8_t *sendData,
									uint8_t sendLen,
									uint8_t *backData,
									uint8_t *backLen,
									uint8_t *validBits,
									uint8_t rxAlign,
									bool checkCRC);

enum StatusCode RC522_ComunicateWithPICC(uint8_t cmd,
										 uint8_t waitIRQ,
										 uint8_t *sendData,
										 uint8_t sendLen,
										 uint8_t *backData,
										 uint8_t *backLen,
										 uint8_t *validBits,
										 uint8_t rxALign,
										 bool chckCRC);

enum StatusCode RC522_CalculateCRC(uint8_t *data,uint8_t length,uint8_t *result);

enum StatusCode RC522_PICC_Select(Uid *uid,uint8_t validBits);

enum StatusCode RC522_Authenticate(uint8_t command,uint8_t blockAddr,MIFARE_KEY *key,Uid *uid);

enum StatusCode RC522_MIFARE_Read(uint8_t blockAddress,uint8_t *buffer,uint8_t *bufferLen);

enum StatusCode RC522_NTAG215_AUTH(uint8_t* passWord, uint8_t pACK[]) ;

void RC522_DumpMifareUltralightToSerial();

Uid RC522_getUid();

bool RC522_ReadCardSerial();
enum StatusCode RC522_Halt();
void RC522_StopCrypto1();



#endif /* RC522_H_ */
