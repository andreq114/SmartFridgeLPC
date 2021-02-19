/*
 * RC522.c
 *
 *  Created on: 15 lis 2020
 *      Author: andre
 */


#include "RC522.h"

SPI_Type *spiBase;
uint8_t destBuff[60];
Uid uid;

lv_obj_t *labelekm;
void dispPRINTFth(const char *text){

		lv_textarea_add_text(labelekm,text);

}

void setConsoleLabel2(lv_obj_t *labelek){
	labelekm = labelek;
}




void RC522_Init(SPI_Type *base,int rstPort,int rstPin){

	spiBase = base;

	GPIO_PinWrite(GPIO,rstPort,rstPin,0);
	for(volatile int i=0;i<1000000;i++);
	GPIO_PinWrite(GPIO,rstPort,rstPin,1);

	PRINTF("\r\nPrzed Soft Reset");
	RC522_SoftReset();
	PRINTF("\r\nPo Soft Reset");


	// When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
	RC522_WriteByteToRegister(TModeReg,0x80);			// TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
	RC522_WriteByteToRegister(TPrescalerReg,0xA9);		// TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25�s.
	RC522_WriteByteToRegister(TReloadRegH,0x03);		// Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
	RC522_WriteByteToRegister(TReloadRegL,0xE8);

	RC522_WriteByteToRegister(TxASKReg, 0x40);			// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	RC522_WriteByteToRegister(ModeReg,0x3D);			// Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)

	RC522_TurnOnAntena();


	/*//ACTIVATE INTERRUPT
	 uint8_t regVal = 0xA0; //rx irq
	 RC522_WriteByteToRegister(ComIEnReg, regVal);*/


}

void RC522_WriteDataToRegister(uint8_t reg,uint8_t *data,uint8_t dataLen){
	spi_transfer_t xfer            = {0};
	uint8_t buff[dataLen+1];
	buff[0] = reg;
	for(volatile int i=0;i<dataLen;i++){
		buff[i+1] = data[i];
	}
	//PRINTF("\r\nWpisywanie adresu");
	xfer.txData      = buff;
	//xfer.rxData      = destByte;
	xfer.dataSize    = sizeof(buff);
	xfer.configFlags = kSPI_FrameAssert;
	status_t temp = SPI_MasterTransferBlocking(spiBase, &xfer);

	/*if(temp == kStatus_Success){
		PRINTF("\r\nSUKCES");
	}else if(temp == kStatus_InvalidArgument){
		PRINTF("\r\nINVALID ARGUMENT");
	}else if(temp == kStatus_SPI_Timeout){
		PRINTF("\r\nTIMEOUT");
	}else{
		PRINTF("\r\nNIEZNANA FLAGA");
	}*/
	/*
	xfer.txData      = data;
	//xfer.rxData      = destByte;
	xfer.dataSize    = dataLen;
	xfer.configFlags = kSPI_FrameAssert;
	//temp = SPI_MasterTransferBlocking(spiBase, &xfer);
	for(uint8_t index = 0;index < dataLen; index++){
		buff[0] = data[index];
		spi_transfer_t xfer1            = {0};
		xfer1.txData      = buff;
		//xfer.rxData      = destByte;
		xfer1.dataSize    = sizeof(data);
		xfer1.configFlags = kSPI_FrameAssert;
		status_t temp = SPI_MasterTransferBlocking(spiBase, &xfer1);
*/
		/*if(temp == kStatus_Success){
			PRINTF("\r\nSUKCES");
		}else if(temp == kStatus_InvalidArgument){
			PRINTF("\r\nINVALID ARGUMENT");
		}else if(temp == kStatus_SPI_Timeout){
			PRINTF("\r\nTIMEOUT");
		}else{
			PRINTF("\r\nNIEZNANA FLAGA");
		}*/
	//}

}

void RC522_WriteByteToRegister(uint8_t reg,uint8_t bajt){
	spi_transfer_t xfer            = {0};
	uint8_t data[2];
	data[0] = reg;
	data[1] = bajt;
	//PRINTF("\r\nWpisywanie adresu");
	xfer.txData      = data;
	//xfer.rxData      = destByte;
	xfer.dataSize    = sizeof(data);
	xfer.configFlags = kSPI_FrameAssert;
	status_t temp = SPI_MasterTransferBlocking(spiBase, &xfer);

	/*if(temp == kStatus_Success){
		PRINTF("\r\nSUKCES");
	}else if(temp == kStatus_InvalidArgument){
		PRINTF("\r\nINVALID ARGUMENT");
	}else if(temp == kStatus_SPI_Timeout){
		PRINTF("\r\nTIMEOUT");
	}else{
		PRINTF("\r\nNIEZNANA FLAGA");
	}*/

	//-------------------------------------------------------------------------------------------------------
	/*data[0] = bajt;
	//PRINTF("\r\nWpisywanie wartosci");
	xfer.txData      = data;
	//xfer.rxData      = destByte;
	xfer.dataSize    = sizeof(data);
	xfer.configFlags = kSPI_FrameAssert;
	temp = SPI_MasterTransferBlocking(spiBase, &xfer);
	*/
	/*if(temp == kStatus_Success){
			PRINTF("\r\nSUKCES");
		}else if(temp == kStatus_InvalidArgument){
			PRINTF("\r\nINVALID ARGUMENT");
		}else if(temp == kStatus_SPI_Timeout){
			PRINTF("\r\nTIMEOUT");
		}else{
			PRINTF("\r\nNIEZNANA FLAGA");
		}*/

	//SPI_WriteData(spiBase,bajt,kSPI_FrameAssert);

}

void RC522_ReadDataFromRegister(uint8_t reg,
								uint8_t *values,
								uint8_t count,
								uint8_t rxAlign){
	if(count == 0)
		return;

	uint8_t address = 0x80 | (reg); 		// MSB == 1 is for reading.
	uint8_t index = 0;
	uint8_t value_t;
	uint8_t dataSend[1];
	uint8_t dataRec[1];



	dataSend[0] = address;

	count--;

	spi_transfer_t xfer1            = {0};
	xfer1.txData      = dataSend;
	xfer1.dataSize    = 1;
	xfer1.configFlags = kSPI_FrameAssert;

	status_t temp = SPI_MasterTransferBlocking(spiBase, &xfer1);


	while(index < count){
		if(index == 0 && rxAlign){
			uint8_t mask = 0;
			for (uint8_t i = rxAlign; i <= 7; i++) {
				mask |= (1 << i);
			}

			dataSend[0] = address;
			spi_transfer_t xfer2            = {0};
			xfer2.txData      = dataSend;
			xfer2.rxData      = dataRec;
			xfer2.dataSize    = 1;
			xfer2.configFlags = kSPI_FrameAssert;

			temp = SPI_MasterTransferBlocking(spiBase, &xfer2);

			value_t = dataRec[0];
			values[0] = (values[index] & ~mask) | (value_t & mask);
		}
		else{
			dataSend[0] = address;
			spi_transfer_t xfer3            = {0};
			xfer3.txData      = dataSend;
			xfer3.rxData      = dataRec;
			xfer3.dataSize    = 1;
			xfer3.configFlags = kSPI_FrameAssert;

			temp = SPI_MasterTransferBlocking(spiBase, &xfer3);
			values[index] = dataRec[0];
		}
		index++;
	}
	/*dataSend[0] = 0x00;
	xfer.txData      = dataSend;
	xfer.rxData      = dataRec;
	xfer.dataSize    = 1;
	xfer.configFlags = kSPI_FrameAssert;
	temp = SPI_MasterTransferBlocking(spiBase, &xfer);

	values[index] = dataRec[0];*/
	/*if(temp == kStatus_Success){
		PRINTF("\r\nSUKCES");
	}else if(temp == kStatus_InvalidArgument){
		PRINTF("\r\nINVALID ARGUMENT");
	}else if(temp == kStatus_SPI_Timeout){
		PRINTF("\r\nTIMEOUT");
	}else{
		PRINTF("\r\nNIEZNANA FLAGA");
	}*/

}

uint8_t RC522_ReadByteFromRegister(uint8_t reg){
	//PRINTF("\r\nCzytanie bajtu");

	uint8_t value_t;
	uint8_t data[1];
	uint8_t bajt = 0x80 | (reg);


	data[0] = bajt;

	spi_transfer_t xfer            = {0};
	xfer.txData      = data;
	//xfer.rxData      = &value_t;
	xfer.dataSize    = 1;
	xfer.configFlags = kSPI_FrameAssert;

	status_t temp = SPI_MasterTransferBlocking(spiBase, &xfer);
	/*if(temp == kStatus_Success){
		PRINTF("\r\nSUKCES");
	}else if(temp == kStatus_InvalidArgument){
		PRINTF("\r\nINVALID ARGUMENT");
	}else if(temp == kStatus_SPI_Timeout){
		PRINTF("\r\nTIMEOUT");
	}else{
		PRINTF("\r\nNIEZNANA FLAGA");
	}
	*/

	//-----------------------------------------------------------------------------------------------
	data[0] = 0x00;


	xfer.txData      = data;
	xfer.rxData      = &value_t;
	xfer.dataSize    = 1;
	xfer.configFlags = kSPI_FrameAssert;

	temp = SPI_MasterTransferBlocking(spiBase, &xfer);
	/*if(temp == kStatus_Success){
		PRINTF("\r\nSUKCES");
	}else if(temp == kStatus_InvalidArgument){
		PRINTF("\r\nINVALID ARGUMENT");
	}else if(temp == kStatus_SPI_Timeout){
		PRINTF("\r\nTIMEOUT");
	}else{
		PRINTF("\r\nNIEZNANA FLAGA");
	}*/
	//value_t = SPI_ReadData(spiBase);

	return value_t;
}



void RC522_TurnOnAntena(){

	uint8_t value = RC522_ReadByteFromRegister(TxControlReg);
	if ((value & 0x03) != 0x03) {
			RC522_WriteByteToRegister(TxControlReg, value | 0x03);
	}


}


void RC522_SoftReset(){
	PRINTF("\r\nWpisanie bajtu soft reset");
	RC522_WriteByteToRegister(CommandReg, SoftReset);
	PRINTF("\r\nPo wpisaniu bajtu soft reset");
	for(volatile int i=0;i<1000000;i++);

	while (RC522_ReadByteFromRegister(CommandReg) & (1<<4)) {
	// PCD still restarting - unlikely after waiting 50ms, but better safe than sorry.
	}

}

void RC522_GetFirmwareVersion(){

	uint8_t version;
	version = RC522_ReadByteFromRegister(VersionReg);
	PRINTF("\r\nFIRMWARE VERSION: 0x");
	PRINTF("%02x",version);

	switch(version){
	case 0x88:
		PRINTF("\r\n = (clone)");
		break;
	case 0x90:
		PRINTF("\r\n = v0.0");
		break;
	case 0x91:
		PRINTF("\r\n = v1.0");
		break;
	case 0x92:
		PRINTF("\r\n = v2.0");
		break;
	default:
		PRINTF("\r\n = (unknown)");
		break;
	}

	if((version == 0x00) || (version == 0xFF)){
		PRINTF("\r\nWARNING: Communication failure, is the MFRC522 properly connected?");
	}
}





bool RC522_IsNewCardPresent(){
	uint8_t bufferATQA[2];
	uint8_t buffLen = sizeof(bufferATQA);

	//RC522_WriteByteToRegister(TxModeReg, 0x00);
	//RC522_WriteByteToRegister(RxModeReg, 0x00);

	//RC522_WriteByteToRegister(ModWidthReg, 0x26);

	enum StatusCode result = RC522_RequestA(bufferATQA,&buffLen);
	/*if(result == STATUS_OK)
		PRINTF("OK\r\n");
	else if(result == STATUS_ERROR)
		PRINTF("STATUS_ERROR\r\n");
	else if(result == STATUS_COLLISION)
			PRINTF("STATUS_COLLISION\r\n");
	else if(result == STATUS_TIMEOUT)
			PRINTF("STATUS_TIMEOUT\r\n");
	else if(result == STATUS_NO_ROOM)
			PRINTF("STATUS_NO_ROOM\r\n");
	else if(result == STATUS_INTERNAL_ERROR)
			PRINTF("STATUS_INTERNAL_ERROR\r\n");
	else if(result == STATUS_INVALID)
				PRINTF("STATUS_INVALID\r\n");
	else if(result == STATUS_CRC_WRONG)
				PRINTF("STATUS_CRC_WRONG\r\n");
	else if(result == STATUS_MIFARE_NACK)
				PRINTF("STATUS_MIFARE_NACK\r\n");
	*/

	//checkStatus(result);
	return (result == STATUS_OK || result == STATUS_COLLISION);

}


enum StatusCode RC522_RequestA(uint8_t *bufferATQA,uint8_t *buffLen){
	return RC522_REQAorWUPA(PICC_CMD_REQA,bufferATQA,buffLen);
}


enum StatusCode RC522_REQAorWUPA(uint8_t command,uint8_t *bufferATQA,uint8_t *buffLen){
	uint8_t validBits;
	enum StatusCode result;

	if(bufferATQA == NULL || *buffLen < 2){	// The ATQA response is 2 bytes long.
		return STATUS_NO_ROOM;
	}

	RC522_ClearRegisterMask(CollReg,0x80);
	validBits = 7;

	result = RC522_TransceiveData1(&command,(uint8_t)1,bufferATQA,buffLen,&validBits,0,false);
	/*if(result == STATUS_OK)
			PRINTF("OK\r\n");
		else if(result == STATUS_ERROR)
			PRINTF("ERROR\r\n");
		else if(result == STATUS_COLLISION)
				PRINTF("STATUS_COLLISION\r\n");
		else if(result == STATUS_TIMEOUT)
				PRINTF("STATUS_TIMEOUT\r\n");
		else if(result == STATUS_NO_ROOM)
				PRINTF("STATUS_NO_ROOM\r\n");
		else if(result == STATUS_INTERNAL_ERROR)
				PRINTF("STATUS_INTERNAL_ERROR\r\n");
		else if(result == STATUS_INVALID)
					PRINTF("STATUS_INVALID\r\n");
		else if(result == STATUS_CRC_WRONG)
					PRINTF("STATUS_CRC_WRONG\r\n");
		else if(result == STATUS_MIFARE_NACK)
					PRINTF("STATUS_MIFARE_NACK\r\n");
	*/
	if(result != STATUS_OK){
		return result;
	}
	//PRINTF("BUFOR: %u, validBits: %u",*buffLen,validBits);
	if(*buffLen != 2 || validBits != 0){
		return STATUS_ERROR;
	}

	return STATUS_OK;


}


void RC522_ClearRegisterMask(uint8_t reg,uint8_t mask){
	uint8_t temp;
	temp = RC522_ReadByteFromRegister(reg);
	RC522_WriteByteToRegister(reg,temp&(~mask));

}

enum StatusCode RC522_TransceiveData1(uint8_t *sendData,
									uint8_t sendLen,
									uint8_t *backData,
									uint8_t *backLen,
									uint8_t *validBits,
									uint8_t rxAlign,
									bool checkCRC)
														{

	uint8_t waitIRQ = 0x30;
	return RC522_ComunicateWithPICC(PCD_Transceive,waitIRQ,sendData,sendLen,backData,backLen,validBits,rxAlign,checkCRC);
}


enum StatusCode RC522_ComunicateWithPICC(uint8_t cmd,
										 uint8_t waitIRQ,
										 uint8_t *sendData,
										 uint8_t sendLen,
										 uint8_t *backData,
										 uint8_t *backLen,
										 uint8_t *validBits,
										 uint8_t rxAlign,
										 bool checkCRC){


	uint8_t _validBits;;
	uint8_t n;
	unsigned int i;

	uint8_t txLastBits = validBits ? *validBits:0;
	uint8_t bitFraming = (rxAlign<<4)+txLastBits;



	RC522_WriteByteToRegister(CommandReg,PCD_Idle);
	RC522_WriteByteToRegister(ComIrqReg,0x7F);
	RC522_WriteByteToRegister(FIFOLevelReg,0x80);
	RC522_WriteDataToRegister(FIFODataReg,sendData,sendLen);
	RC522_WriteByteToRegister(BitFramingReg,bitFraming);
	RC522_WriteByteToRegister(CommandReg,cmd);

	if(cmd == PCD_Transceive){
		RC522_SetRegisterBitMask(BitFramingReg,0x80);
	}

	// Wait for the command to complete.
	// In PCD_Init() we set the TAuto flag in TModeReg. This means the timer automatically starts when the PCD stops transmitting.
	// Each iteration of the do-while-loop takes 17.86μs.
	// TODO check/modify for other architectures than Arduino Uno 16bit

	i=2000;
	while(1){
		n = RC522_ReadByteFromRegister(ComIrqReg);
		//PRINTF("Bajt z rejestru: %02x\r\n",n);


		if(n & waitIRQ){
			break;
		}
		if(n & 0x01){
			//dispPRINTFth("TIMEOUT 2\n");
			return STATUS_ERROR;
		}
		if(--i==0){
			//dispPRINTFth("CWP_TIMEOUT 3\n");
			return STATUS_TIMEOUT;
		}
	}
	//dispPRINTFth("Po probie odczytu\n");
	//dispPRINTFth("COMM_WITH_PICC_1\n");
	// 35.7ms and nothing happend. Communication with the MFRC522 might be down.

	// Stop now if any errors except collisions were detected.
	uint8_t errorRegValue = RC522_ReadByteFromRegister(ErrorReg);
	if(errorRegValue & 0x13){
		return STATUS_ERROR;
	}





	// Stop now if any errors except collisions were detected.

	if(backData && backLen){
		n = RC522_ReadByteFromRegister(FIFOLevelReg);

		if(n > *backLen)
			return STATUS_NO_ROOM;
		//PRINTF("BUFFER PRZED: %u\r\n",*backLen);
		*backLen = n;
		//PRINTF("BUFFER PO: %u\r\n",*backLen);

		RC522_ReadDataFromRegister(FIFODataReg,backData,n,rxAlign);
		_validBits = RC522_ReadByteFromRegister(ControlReg) & 0x07;

		if(validBits)
			*validBits = _validBits;
	}

	// Tell about collisions
	if (errorRegValue & 0x08) {		// CollErr
		return STATUS_COLLISION;
	}

	// Perform CRC_A validation if requested.
	if(backData && backLen && checkCRC){
		// In this case a MIFARE Classic NAK is not OK.
				if (*backLen == 1 && _validBits == 4)
				{
					PRINTF("TU WYWALAM");
					return STATUS_MIFARE_NACK;
				}

				// We need at least the CRC_A value and all 8 bits of the last byte must be received.
				if (*backLen < 2 || _validBits != 0) {
					//dispPRINTFth("TU WYRZUCAM\n");
					return STATUS_CRC_WRONG;
				}

				// Verify CRC_A - do our own calculation and store the control in controlBuffer.
				uint8_t controlBuffer[2];
				enum StatusCode status = RC522_CalculateCRC(&backData[0], *backLen - 2, &controlBuffer[0]);
				if (status != STATUS_OK) {
					return status;
				}
				/*if ((backData[*backLen - 2] != controlBuffer[0]) || (backData[*backLen - 1] != controlBuffer[1])) {
					return STATUS_CRC_WRONG;
				}*/
	}
	return STATUS_OK;

}

void RC522_SetRegisterBitMask(uint8_t reg,uint8_t mask){
	uint8_t temp;
	temp = RC522_ReadByteFromRegister(reg);
	RC522_WriteByteToRegister(reg,temp|mask);
}

enum StatusCode RC522_CalculateCRC(uint8_t *data,uint8_t length,uint8_t *result){
	RC522_WriteByteToRegister(CommandReg,PCD_Idle);		// Stop any active command.
	RC522_WriteByteToRegister(DivIrqReg,0x04);			// Clear the CRCIRq interrupt request bit
	RC522_WriteByteToRegister(FIFOLevelReg,0x80);		// FlushBuffer = 1, FIFO initialization
	RC522_WriteDataToRegister(FIFODataReg,data,length); // Write data to the FIFO
	RC522_WriteByteToRegister(CommandReg,PCD_CalcCRC);	// Start the calculation

	// Wait for the CRC calculation to complete. Each iteration of the while-loop takes 17.73us.
	uint16_t i = 0;
	uint8_t n;
		while (1) {
			n = RC522_ReadByteFromRegister(DivIrqReg);	// DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
			if (n & 0x04) {						// CRCIRq bit set - calculation done
				break;
			}
			if (--i == 0) {						// The emergency break. We will eventually terminate on this one after 89ms. Communication with the MFRC522 might be down.
				return STATUS_TIMEOUT;
			}
		}
		RC522_WriteByteToRegister(CommandReg, PCD_Idle);		// Stop calculating CRC for new content in the FIFO.

		// Transfer the result from the registers to the result buffer
		result[0] = RC522_ReadByteFromRegister(CRCResultRegL);
		result[1] = RC522_ReadByteFromRegister(CRCResultRegH);
		return STATUS_OK;
	/*
	for(uint16_t i=5000;i>0;i--){
		// DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
		uint8_t n = RC522_ReadByteFromRegister(DivIrqReg);
		if (n & 0x04) {									// CRCIRq bit set - calculation done
			RC522_WriteByteToRegister(CommandReg, PCD_Idle);	// Stop calculating CRC for new content in the FIFO.
			// Transfer the result from the registers to the result buffer
			result[0] = RC522_ReadByteFromRegister(CRCResultRegL);
			result[1] = RC522_ReadByteFromRegister(CRCResultRegH);
			return STATUS_OK;
		}
	}
	// 89ms passed and nothing happend. Communication with the MFRC522 might be down.
	PRINTF("CALCULATECRC_TIMEOUT");
	return STATUS_TIMEOUT;*/
	// Wait for the CRC calculation to complete. Each iteration of the
	// while-loop takes 17.73�s.
	//uint16_t i = 5000;
	//uint8_t n;
	/*while (1) {
		n = RC522_ReadByteFromRegister(DivIrqReg); // DivIrqReg[7..0] bits are: Set2
		// reserved reserved MfinActIRq
		// reserved CRCIRq reserved
		// reserved
		//PRINTF("\r\n%02x",n);
		if (n & 0x04) { // CRCIRq bit set - calculation done
			break;
		}
		if (--i == 0) { // The emergency break. We will eventually terminate on
			// this one after 89ms. Communication with the MFRC522
			// might be down.
			return STATUS_TIMEOUT;
		}
	}
	RC522_WriteByteToRegister(CommandReg,PCD_Idle); // Stop calculating CRC for new content in the FIFO.

	// Transfer the result from the registers to the result buffer
	result[0] = RC522_ReadByteFromRegister(CRCResultRegL);
	result[1] = RC522_ReadByteFromRegister(CRCResultRegH);
	return STATUS_OK;*/
}


bool RC522_ReadCardSerial(){
	char buff[20];
	enum StatusCode result = RC522_PICC_Select(&uid,0);

	/*checkStatus(result);
	dispPRINTFth("\nDlugosc UID:\n");
	if(uid.size == 4){
		sprintf(buff,"%d",uid.uidByte[0]);
		dispPRINTFth(buff);
		dispPRINTFth(" ");
		sprintf(buff,"%d",uid.uidByte[1]);
		dispPRINTFth(buff);
		dispPRINTFth(" ");
		sprintf(buff,"%d",uid.uidByte[2]);
		dispPRINTFth(buff);
		dispPRINTFth(" ");
		sprintf(buff,"%d",uid.uidByte[3]);
		dispPRINTFth(buff);
		dispPRINTFth("\n");
	}else{
		sprintf(buff,"%d",uid.uidByte[0]);
		dispPRINTFth(buff);
		dispPRINTFth(" ");
		sprintf(buff,"%d",uid.uidByte[1]);
		dispPRINTFth(buff);
		dispPRINTFth(" ");
		sprintf(buff,"%d",uid.uidByte[2]);
		dispPRINTFth(buff);
		dispPRINTFth(" ");
		sprintf(buff,"%d",uid.uidByte[3]);
		dispPRINTFth(buff);
		dispPRINTFth(" ");
		sprintf(buff,"%d",uid.uidByte[4]);
		dispPRINTFth(buff);
		dispPRINTFth(" ");
		sprintf(buff,"%d",uid.uidByte[5]);
		dispPRINTFth(buff);
		dispPRINTFth(" ");
		sprintf(buff,"%d",uid.uidByte[6]);
		dispPRINTFth(buff);
		dispPRINTFth("\n");
	}
	dispPRINTFth("UID: ");
	//for(int i=0;i<uid.size;i++)
		//dispPRINTF("%02x ",uid.uidByte[i]);
	//dispPRINTF("\r\n");*/
	return (result == STATUS_OK);
}

Uid RC522_getUid(){
	return uid;
}

/*enum StatusCode RC522_PICC_Select(Uid *uid,uint8_t validBits){
	bool uidComplete;
	bool selectDone;
	bool useCascadeTag;
	uint8_t cascadeLevel = 1;
	enum StatusCode result;
	uint8_t count;
	uint8_t checkBit;
	uint8_t index;
	uint8_t uidIndex;					// The first index in uid->uidByte[] that is used in the current Cascade Level.
	uint8_t currentLevelKnownBits;		// The number of known UID bits in the current Cascade Level.
	uint8_t buffer[9];					// The SELECT/ANTICOLLISION commands uses a 7 byte standard frame + 2 bytes CRC_A
	uint8_t bufferUsed;				// The number of bytes used in the buffer, ie the number of bytes to transfer to the FIFO.
	uint8_t rxAlign;					// Used in BitFramingReg. Defines the bit position for the first bit received.
	uint8_t txLastBits;				// Used in BitFramingReg. The number of valid bits in the last transmitted byte.
	uint8_t *responseBuffer;
	uint8_t responseLength;

	// Description of buffer structure:
	//		uint8_t 0: SEL				Indicates the Cascade Level: PICC_CMD_SEL_CL1,
	//PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
	//		uint8_t 1: NVB					Number of Valid Bits (in complete command, not
	//just the UID): High nibble: complete uint8_ts, Low nibble: Extra bits.
	//		uint8_t 2: UID-data or CT		See explanation below. CT means Cascade
	//Tag.
	//		uint8_t 3: UID-data
	//		uint8_t 4: UID-data
	//		uint8_t 5: UID-data
	//		uint8_t 6: BCC					Block Check Character - XOR of uint8_ts
	//2-5
	//		uint8_t 7: CRC_A
	//		uint8_t 8: CRC_A
	// The BCC and CRC_A are only transmitted if we know all the UID bits of the
	// current Cascade Level.
	//
	// Description of uint8_ts 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft:
	// UID contents and cascade levels)
	//		UID size	Cascade level	uint8_t2	uint8_t3	uint8_t4
	//uint8_t5
	//		========	=============	=====	=====	=====	=====
	//		 4 uint8_ts		1			uid0	uid1	uid2	uid3
	//		 7 uint8_ts		1			CT		uid0	uid1	uid2
	//						2			uid3	uid4	uid5	uid6
	//		10 uint8_ts		1			CT		uid0	uid1	uid2
	//						2			CT		uid3	uid4	uid5
	//						3			uid6	uid7	uid8	uid9

	// Sanity checks
		if (validBits > 80) {
			return STATUS_INVALID;
		}

		// Prepare MFRC522
		RC522_ClearRegisterMask(CollReg, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.

		// Repeat Cascade Level loop until we have a complete UID.
		uidComplete = false;
		while (!uidComplete) {
			// Set the Cascade Level in the SEL byte, find out if we need to use the Cascade Tag in byte 2.
			switch (cascadeLevel) {
				case 1:
					buffer[0] = PICC_CMD_SEL_CL1;
					uidIndex = 0;
					useCascadeTag = validBits && uid->size > 4;	// When we know that the UID has more than 4 bytes
					dispPRINTFth("CASE 1\n");
					break;

				case 2:
					buffer[0] = PICC_CMD_SEL_CL2;
					uidIndex = 3;
					useCascadeTag = validBits && uid->size > 7;	// When we know that the UID has more than 7 bytes
					dispPRINTFth("CASE 2\n");
					break;

				case 3:
					buffer[0] = PICC_CMD_SEL_CL3;
					uidIndex = 6;
					useCascadeTag = false;						// Never used in CL3.
					dispPRINTFth("CASE 3");
					break;

				default:
					return STATUS_INTERNAL_ERROR;
					dispPRINTFth("DEFAUT");
					break;
			}

			// How many UID bits are known in this Cascade Level?
			currentLevelKnownBits = validBits - (8 * uidIndex);
			if (currentLevelKnownBits < 0) {
				currentLevelKnownBits = 0;
			}
			// Copy the known bits from uid->uidByte[] to buffer[]
			index = 2; // destination index in buffer[]
			if (useCascadeTag) {
				buffer[index++] = PICC_CMD_CT;
			}
			uint8_t bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0); // The number of bytes needed to represent the known bits for this level.
			if (bytesToCopy) {
				uint8_t maxBytes = useCascadeTag ? 3 : 4; // Max 4 bytes in each Cascade Level. Only 3 left if we use the Cascade Tag
				if (bytesToCopy > maxBytes) {
					bytesToCopy = maxBytes;
				}
				for (count = 0; count < bytesToCopy; count++) {
					buffer[index++] = uid->uidByte[uidIndex + count];
				}
			}
			// Now that the data has been copied we need to include the 8 bits in CT in currentLevelKnownBits
			if (useCascadeTag) {
				currentLevelKnownBits += 8;
			}

			// Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
			selectDone = false;
			int i=0;
			while (!selectDone) {
				//dispPRINTFth("TEXT_");
				// Find out how many bits and bytes to send and receive.
				if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
					//Serial.print(F("SELECT: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
					buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes
					// Calculate BCC - Block Check Character
					buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
					// Calculate CRC_A
					result = RC522_CalculateCRC(buffer, 7, &buffer[7]);

					if (result != STATUS_OK) {
						//dispPRINTFth("Tu wypierdzielam sie\n");
						return result;
					}
					txLastBits		= 0; // 0 => All 8 bits are valid.
					bufferUsed		= 9;
					// Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
					responseBuffer	= &buffer[6];
					responseLength	= 3;
				}
				else { // This is an ANTICOLLISION.
					//Serial.print(F("ANTICOLLISION: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
					txLastBits		= currentLevelKnownBits % 8;
					count			= currentLevelKnownBits / 8;	// Number of whole bytes in the UID part.
					index			= 2 + count;					// Number of whole bytes: SEL + NVB + UIDs
					buffer[1]		= (index << 4) + txLastBits;	// NVB - Number of Valid Bits
					bufferUsed		= index + (txLastBits ? 1 : 0);
					// Store response in the unused part of buffer
					responseBuffer	= &buffer[index];
					responseLength	= sizeof(buffer) - index;
				}

				// Set bit adjustments
				rxAlign = txLastBits;											// Having a separate variable is overkill. But it makes the next line easier to read.
				RC522_WriteByteToRegister(BitFramingReg, (rxAlign << 4) + txLastBits);	// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

				// Transmit the buffer and receive the response.
				result = RC522_TransceiveData1(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign,false);
				checkStatus(result);
				if (result == STATUS_COLLISION) { // More than one PICC in the field => collision.
					uint8_t valueOfCollReg = RC522_ReadByteFromRegister(CollReg); // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
					if (valueOfCollReg & 0x20) { // CollPosNotValid
						//dispPRINTFth("Tu wypierdzielam sie2\n");
						return STATUS_COLLISION; // Without a valid collision position we cannot continue
					}
					uint8_t collisionPos = valueOfCollReg & 0x1F; // Values 0-31, 0 means bit 32.
					if (collisionPos == 0) {
						collisionPos = 32;
					}
					if (collisionPos <= currentLevelKnownBits) { // No progress - should not happen
						return STATUS_INTERNAL_ERROR;
					}
					// Choose the PICC with the bit set.
					currentLevelKnownBits	= collisionPos;
					count			= currentLevelKnownBits % 8; // The bit to modify
					checkBit		= (currentLevelKnownBits - 1) % 8;
					index			= 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First byte is index 0.
					buffer[index]	|= (1 << checkBit);
				}
				else if (result != STATUS_OK) {

					return result;
				}
				else { // STATUS_OK
					if (currentLevelKnownBits >= 32) { // This was a SELECT.
						selectDone = true; // No more anticollision
						// We continue below outside the while.
					}
					else { // This was an ANTICOLLISION.
						// We now have all 32 bits of the UID in this Cascade Level
						currentLevelKnownBits = 32;
						// Run loop again to do the SELECT.
					}
				}
			} // End of while (!selectDone)

			// We do not check the CBB - it was constructed by us above.

			// Copy the found UID bytes from buffer[] to uid->uidByte[]
			index			= (buffer[2] == PICC_CMD_CT) ? 3 : 2; // source index in buffer[]
			bytesToCopy		= (buffer[2] == PICC_CMD_CT) ? 3 : 4;
			for (count = 0; count < bytesToCopy; count++) {
				uid->uidByte[uidIndex + count] = buffer[index++];
			}

			// Check response SAK (Select Acknowledge)
			if (responseLength != 3 || txLastBits != 0) { // SAK must be exactly 24 bits (1 byte + CRC_A).
				return STATUS_ERROR;
			}
			// Verify CRC_A - do our own calculation and store the control in buffer[2..3] - those bytes are not needed anymore.
			result = RC522_CalculateCRC(responseBuffer, 1, &buffer[2]);

			if (result != STATUS_OK) {
				return result;
			}
			PRINTF("\r\n%d %d\n\r%d %d\r\n",buffer[2],responseBuffer[1],buffer[3],responseBuffer[2]);
			if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
				dispPRINTFth("WRONG CRC\n");
				PRINTF("WRONG CRC:(\r\n");
				return STATUS_CRC_WRONG;
			}
			if (responseBuffer[0] & 0x04) { // Cascade bit set - UID not complete yes
				cascadeLevel++;
			}
			else {
				uidComplete = true;
				uid->sak = responseBuffer[0];
			}
		} // End of while (!uidComplete)

		// Set correct uid->size
		uid->size = 3 * cascadeLevel + 1;

		return STATUS_OK;
}
*/

enum StatusCode RC522_PICC_Select(Uid *uid,uint8_t validBits){
		bool uidComplete;
		bool selectDone;
		bool useCascadeTag;
		uint8_t cascadeLevel = 1;
		enum StatusCode result;
		uint8_t count;
		uint8_t checkBit;
		uint8_t index;
		uint8_t uidIndex;					// The first index in uid->uidByte[] that is used in the current Cascade Level.
		int8_t currentLevelKnownBits;		// The number of known UID bits in the current Cascade Level.
		uint8_t buffer[9];					// The SELECT/ANTICOLLISION commands uses a 7 byte standard frame + 2 bytes CRC_A
		uint8_t bufferUsed;				// The number of bytes used in the buffer, ie the number of bytes to transfer to the FIFO.
		uint8_t rxAlign;					// Used in BitFramingReg. Defines the bit position for the first bit received.
		uint8_t txLastBits;				// Used in BitFramingReg. The number of valid bits in the last transmitted byte.
		uint8_t *responseBuffer;
		uint8_t responseLength;

		// Description of buffer structure:
		//		Byte 0: SEL 				Indicates the Cascade Level: PICC_CMD_SEL_CL1, PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
		//		Byte 1: NVB					Number of Valid Bits (in complete command, not just the UID): High nibble: complete bytes, Low nibble: Extra bits.
		//		Byte 2: UID-data or CT		See explanation below. CT means Cascade Tag.
		//		Byte 3: UID-data
		//		Byte 4: UID-data
		//		Byte 5: UID-data
		//		Byte 6: BCC					Block Check Character - XOR of bytes 2-5
		//		Byte 7: CRC_A
		//		Byte 8: CRC_A
		// The BCC and CRC_A are only transmitted if we know all the UID bits of the current Cascade Level.
		//
		// Description of bytes 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft: UID contents and cascade levels)
		//		UID size	Cascade level	Byte2	Byte3	Byte4	Byte5
		//		========	=============	=====	=====	=====	=====
		//		 4 bytes		1			uid0	uid1	uid2	uid3
		//		 7 bytes		1			CT		uid0	uid1	uid2
		//						2			uid3	uid4	uid5	uid6
		//		10 bytes		1			CT		uid0	uid1	uid2
		//						2			CT		uid3	uid4	uid5
		//						3			uid6	uid7	uid8	uid9

		// Sanity checks
		if (validBits > 80) {
			return STATUS_INVALID;
		}

		// Prepare MFRC522
		RC522_ClearRegisterMask(CollReg, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.

		// Repeat Cascade Level loop until we have a complete UID.
		uidComplete = false;
		while (!uidComplete) {
			// Set the Cascade Level in the SEL byte, find out if we need to use the Cascade Tag in byte 2.
			switch (cascadeLevel) {
				case 1:
					buffer[0] = PICC_CMD_SEL_CL1;
					uidIndex = 0;
					useCascadeTag = validBits && uid->size > 4;	// When we know that the UID has more than 4 bytes
					break;

				case 2:
					buffer[0] = PICC_CMD_SEL_CL2;
					uidIndex = 3;
					useCascadeTag = validBits && uid->size > 7;	// When we know that the UID has more than 7 bytes
					break;

				case 3:
					buffer[0] = PICC_CMD_SEL_CL3;
					uidIndex = 6;
					useCascadeTag = false;						// Never used in CL3.
					break;

				default:
					return STATUS_INTERNAL_ERROR;
					break;
			}

			// How many UID bits are known in this Cascade Level?
			currentLevelKnownBits = validBits - (8 * uidIndex);
			if (currentLevelKnownBits < 0) {
				currentLevelKnownBits = 0;
			}
			// Copy the known bits from uid->uidByte[] to buffer[]
			index = 2; // destination index in buffer[]
			if (useCascadeTag) {
				buffer[index++] = PICC_CMD_CT;
			}
			uint8_t bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0); // The number of bytes needed to represent the known bits for this level.
			if (bytesToCopy) {
				uint8_t maxBytes = useCascadeTag ? 3 : 4; // Max 4 bytes in each Cascade Level. Only 3 left if we use the Cascade Tag
				if (bytesToCopy > maxBytes) {
					bytesToCopy = maxBytes;
				}
				for (count = 0; count < bytesToCopy; count++) {
					buffer[index++] = uid->uidByte[uidIndex + count];
				}
			}
			// Now that the data has been copied we need to include the 8 bits in CT in currentLevelKnownBits
			if (useCascadeTag) {
				currentLevelKnownBits += 8;
			}

			// Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
			selectDone = false;
			while (!selectDone) {
				// Find out how many bits and bytes to send and receive.
				if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
					//Serial.print(F("SELECT: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
					buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes
					// Calculate BCC - Block Check Character
					buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
					// Calculate CRC_A
					result = RC522_CalculateCRC(buffer, 7, &buffer[7]);
					if (result != STATUS_OK) {
						return result;
					}
					txLastBits		= 0; // 0 => All 8 bits are valid.
					bufferUsed		= 9;
					// Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
					responseBuffer	= &buffer[6];
					responseLength	= 3;
				}
				else { // This is an ANTICOLLISION.
					//Serial.print(F("ANTICOLLISION: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
					txLastBits		= currentLevelKnownBits % 8;
					count			= currentLevelKnownBits / 8;	// Number of whole bytes in the UID part.
					index			= 2 + count;					// Number of whole bytes: SEL + NVB + UIDs
					buffer[1]		= (index << 4) + txLastBits;	// NVB - Number of Valid Bits
					bufferUsed		= index + (txLastBits ? 1 : 0);
					// Store response in the unused part of buffer
					responseBuffer	= &buffer[index];
					responseLength	= sizeof(buffer) - index;
				}

				// Set bit adjustments
				rxAlign = txLastBits;											// Having a separate variable is overkill. But it makes the next line easier to read.
				RC522_WriteByteToRegister(BitFramingReg, (rxAlign << 4) + txLastBits);	// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

				// Transmit the buffer and receive the response.
				result = RC522_TransceiveData1(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign,false);
				if (result == STATUS_COLLISION) { // More than one PICC in the field => collision.
					uint8_t valueOfCollReg = RC522_ReadByteFromRegister(CollReg); // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
					if (valueOfCollReg & 0x20) { // CollPosNotValid
						return STATUS_COLLISION; // Without a valid collision position we cannot continue
					}
					uint8_t collisionPos = valueOfCollReg & 0x1F; // Values 0-31, 0 means bit 32.
					if (collisionPos == 0) {
						collisionPos = 32;
					}
					if (collisionPos <= currentLevelKnownBits) { // No progress - should not happen
						return STATUS_INTERNAL_ERROR;
					}
					// Choose the PICC with the bit set.
					currentLevelKnownBits	= collisionPos;
					count			= currentLevelKnownBits % 8; // The bit to modify
					checkBit		= (currentLevelKnownBits - 1) % 8;
					index			= 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First byte is index 0.
					buffer[index]	|= (1 << checkBit);
				}
				else if (result != STATUS_OK) {
					return result;
				}
				else { // STATUS_OK
					if (currentLevelKnownBits >= 32) { // This was a SELECT.
						selectDone = true; // No more anticollision
						// We continue below outside the while.
					}
					else { // This was an ANTICOLLISION.
						// We now have all 32 bits of the UID in this Cascade Level
						currentLevelKnownBits = 32;
						// Run loop again to do the SELECT.
					}
				}
			} // End of while (!selectDone)

			// We do not check the CBB - it was constructed by us above.

			// Copy the found UID bytes from buffer[] to uid->uidByte[]
			index			= (buffer[2] == PICC_CMD_CT) ? 3 : 2; // source index in buffer[]
			bytesToCopy		= (buffer[2] == PICC_CMD_CT) ? 3 : 4;
			for (count = 0; count < bytesToCopy; count++) {
				uid->uidByte[uidIndex + count] = buffer[index++];
			}

			// Check response SAK (Select Acknowledge)
			if (responseLength != 3 || txLastBits != 0) { // SAK must be exactly 24 bits (1 byte + CRC_A).
				return STATUS_ERROR;
			}
			// Verify CRC_A - do our own calculation and store the control in buffer[2..3] - those bytes are not needed anymore.
			result = RC522_CalculateCRC(responseBuffer, 1, &buffer[2]);
			if (result != STATUS_OK) {
				return result;
			}
//			if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
//				return STATUS_CRC_WRONG;
//			}
			if (responseBuffer[0] & 0x04) { // Cascade bit set - UID not complete yes
				cascadeLevel++;
			}
			else {
				uidComplete = true;
				uid->sak = responseBuffer[0];
			}
		} // End of while (!uidComplete)

		// Set correct uid->size
		uid->size = 3 * cascadeLevel + 1;

		return STATUS_OK;
}



void checkStatus(enum StatusCode result){
	if(result == STATUS_OK)
		dispPRINTFth("STATUS_OK\n");
	else if(result == STATUS_ERROR)
		dispPRINTFth("STATUS_ERROR\n");
	else if(result == STATUS_COLLISION)
		dispPRINTFth("STATUS_COLLISION\n");
	else if(result == STATUS_TIMEOUT)
		dispPRINTFth("STATUS_TIMEOUT\n");
	else if(result == STATUS_NO_ROOM)
		dispPRINTFth("STATUS_NO_ROOM\n");
	else if(result == STATUS_INTERNAL_ERROR)
		dispPRINTFth("STATUS_INTERNAL_ERROR\n");
	else if(result == STATUS_INVALID)
		dispPRINTFth("STATUS_INVALID\n");
	else if(result == STATUS_CRC_WRONG)
		dispPRINTFth("STATUS_CRC_WRONG\n");
	else if(result == STATUS_MIFARE_NACK)
		dispPRINTFth("STATUS_MIFARE_NACK\n");
}

enum StatusCode RC522_Halt(){
		enum StatusCode result;
		uint8_t buffer[4];

		// Build command buffer
		buffer[0] = PICC_CMD_HLTA;
		buffer[1] = 0;
		// Calculate CRC_A
		result = RC522_CalculateCRC(buffer, 2, &buffer[2]);
		if (result != STATUS_OK) {
			return result;
		}

		// Send the command.
		// The standard says:
		//		If the PICC responds with any modulation during a period of 1 ms after the end of the frame containing the
		//		HLTA command, this response shall be interpreted as 'not acknowledge'.
		// We interpret that this way: Only STATUS_TIMEOUT is a success.
		result = RC522_TransceiveData1(buffer, sizeof(buffer), NULL, 0,NULL,0,false);
		if (result == STATUS_TIMEOUT) {
			return STATUS_OK;
		}
		if (result == STATUS_OK) { // That is ironically NOT ok in this case ;-)
			return STATUS_ERROR;
		}
		//dispPRINTFth("\n");
		return result;

}


void RC522_StopCrypto1(){
	RC522_ClearRegisterMask(Status2Reg, 0x08);
}

enum StatusCode RC522_Authenticate(uint8_t command,uint8_t blockAddr,MIFARE_KEY *key,Uid *uid){
		uint8_t waitIRq = 0x10;		// IdleIRq

		// Build command buffer
		uint8_t sendData[12];
		sendData[0] = command;
		sendData[1] = blockAddr;
		for (uint8_t i = 0; i < MF_KEY_SIZE; i++) {	// 6 key bytes
			sendData[2+i] = key->keyByte[i];
		}
		// Use the last uid bytes as specified in http://cache.nxp.com/documents/application_note/AN10927.pdf
		// section 3.2.5 "MIFARE Classic Authentication".
		// The only missed case is the MF1Sxxxx shortcut activation,
		// but it requires cascade tag (CT) byte, that is not part of uid.
		for (uint8_t i = 0; i < 4; i++) {				// The last 4 bytes of the UID
			sendData[8+i] = uid->uidByte[i+uid->size-4];
		}

		// Start the authentication.
		return RC522_ComunicateWithPICC(PCD_MFAuthent, waitIRq, &sendData[0], sizeof(sendData),NULL,0,0,0,false);
}


enum StatusCode RC522_MIFARE_Read(uint8_t blockAddress,uint8_t *buffer,uint8_t *bufferLen){
	enum StatusCode result;

	// Sanity check
	if (buffer == NULL || *bufferLen < 18) {
		return STATUS_NO_ROOM;
	}

	// Build command buffer
	buffer[0] = PICC_CMD_MF_READ;
	buffer[1] = blockAddress;
	// Calculate CRC_A
	result = RC522_CalculateCRC(buffer, 2, &buffer[2]);
	if (result != STATUS_OK) {
		return result;
	}
	PRINTF("PO KALKULACJI");

	// Transmit the buffer and receive the response, validate CRC_A.
	return RC522_TransceiveData1(buffer, 4, buffer, bufferLen, NULL, 0,
			true);
}





// NTAG 216

enum StatusCode RC522_NTAG215_AUTH(uint8_t* passWord, uint8_t pACK[]) //Authenticate with 32bit password
{
	// TODO: Fix cmdBuffer length and rxlength. They really should match.
	//       (Better still, rxlength should not even be necessary.)

	enum StatusCode result;
	uint8_t				cmdBuffer[18]; // We need room for 16 bytes data and 2 bytes CRC_A.

	cmdBuffer[0] = 0x1B; //Komenda do autoryzacji

	for (uint8_t i = 0; i<4; i++)
		cmdBuffer[i+1] = passWord[i];

	result = RC522_CalculateCRC(cmdBuffer, 5, &cmdBuffer[5]);

	if (result!=STATUS_OK) {
		return result;
	}

	// Transceive the data, store the reply in cmdBuffer[]
	uint8_t waitIRq		= 0x30;	// RxIRq and IdleIRq
//	byte cmdBufferSize	= sizeof(cmdBuffer);
	uint8_t validBits		= 0;
	uint8_t rxlength		= 5;
	result = RC522_ComunicateWithPICC(PCD_Transceive, waitIRq, cmdBuffer, 7, cmdBuffer, &rxlength, &validBits,0,false);

	pACK[0] = cmdBuffer[0];
	pACK[1] = cmdBuffer[1];

	if (result!=STATUS_OK) {
		return result;
	}

	return STATUS_OK;
} // End PCD_NTAG216_AUTH()






void RC522_DumpMifareUltralightToSerial() {
	enum StatusCode status;
	uint8_t byteCount;
	uint8_t buffer[18];

	char tempBuff[20];


	//dispPRINTFth("Page  0  1  2  3\n");
	// Try the mpages of the original Ultralight. Ultralight C has more pages.
	for (uint8_t page = 8; page < 20; page +=4) { // Read returns data for 4 pages at a time.
			// Read pages
			byteCount = sizeof(buffer);
			status = RC522_MIFARE_Read(page, buffer, &byteCount);
			if (status != STATUS_OK) {
				//("MIFARE READ FAIL");;
				//checkStatus(status);
				break;
			}
			//memset(tempBuff, 0, sizeof(tempBuff));
			for(int k=0;k<16;k++){
				tempBuff[k] = buffer[k];
			}
			tempBuff[16] = '\0';
			//dispPRINTFth(tempBuff);
			// Dump data
//			for (uint8_t offset = 0; offset < 4; offset++) {
//				i = page + offset;
//				if(i < 10)
//					dispPRINTFth("  "); // Pad with spaces
//				else
//					dispPRINTFth(" "); // Pad with spaces
//				//dispPRINTFth(i);
//				dispPRINTFth("  ");
//				for (uint8_t index = 0; index < 4; index++) {
//					i = 4 * offset + index;
//					if(buffer[i] < 0x10)
//						dispPRINTFth(" 0");
//					else
//						dispPRINTFth(" ");
//					sprintf(tempBuff,"%c",buffer[i]);
//					znaczek = buffer[i];
//					dispPRINTFth((char)znaczek);
//				}
//				dispPRINTFth("\n");
//			}
		}
	//dispPRINTFth("\nODCZYTANE!\n");
} // End PICC_DumpMifareUltralightToSerial()
