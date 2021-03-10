/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


// AKTUALNA WERSJA PROJEKTU --------------------------------------------------------
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "littlevgl_support.h"
#include "board.h"

#include "data.h"
#include "pin_mux.h"
#include "qcom_api.h"
#include "wlan_qcom.h"
#include <stdbool.h>
#include "fsl_spi.h"
#include "smart_fridge.h"
#include "RC522.h"
#include "peripherals.h"
#include "GUI.h"
#include "GUI_ExtData.h"
#include <fsl_rtc.h>
#include "fsl_spifi.h"
#include "fsl_sctimer.h"


#define TIME_SERVER "showcase.api.linx.twenty57.net"


#define SPIFI_DATA_START_ADDRESS 200704
#define SPIFI_NUMBER_OF_PRODUCTS_ADDRESS SPIFI_DATA_START_ADDRESS+56001
#define SPIFI_SHOP_LIST_ADDRESS 266240
#define SPIFI_NETWORK_DATA_ADDRESS 274432


//#define SPIFI_DATA_START_ADDRESS 143360
//#define SPIFI_NUMBER_OF_PRODUCTS_ADDRESS SPIFI_DATA_START_ADDRESS+56001
//#define SPIFI_SHOP_LIST_ADDRESS 225280
//#define SPIFI_NETWORK_DATA_ADDRESS 241664

#define NETWORK_DATA_LENGTH 98
#define PRODUCTS_DATA_LENGTH_BYTES 3500


#define EXAMPLE_SPI_MASTER          SPI0

static volatile bool s_lvgl_initialized = false;


const int TASK_MAIN_PRIO       = configMAX_PRIORITIES - 3;
const int TASK_MAIN_STACK_SIZE = 800;

portSTACK_TYPE *task_main_stack = NULL;
TaskHandle_t task_main_task_handler;
TaskHandle_t task_disp_task_handler;


#define SSID_LENGTH 33
#define PASSWORD_LENGTH 65
typedef struct{
	char WIFI_SSID[33];
	char WIFI_PASSWORD[65];
	int SSID_LEN;
	int PASSWORD_LEN;
}WIFI_AUTH;

WIFI_AUTH wifi_data;


QCOM_SSID g_ssid;
QCOM_PASSPHRASE g_passphrase;

WLAN_AUTH_MODE g_auth    = WLAN_AUTH_WPA2_PSK;
WLAN_CRYPT_TYPE g_cipher = WLAN_CRYPT_AES_CRYPT;


extern int numIrqs;
extern int initTime;

// PWM Event
uint32_t event;
uint32_t event2;

int licznik = 0;

//
/////////////////////////////////////////////////////////////////////////////////
//#define NOTE_C4  26  //Defining note frequency
//#define NOTE_D4  29
//#define NOTE_E4  33
//#define NOTE_F4  34
//#define NOTE_G4  39
//#define NOTE_A4  44
//#define NOTE_B4  49
//#define NOTE_C5  52
//#define NOTE_D5  58
//#define NOTE_E5  65
//#define NOTE_F5  69
//#define NOTE_G5  78
//#define NOTE_A5  88
//#define NOTE_B5  98
///////////////////////////////////////////////////////////////////////////////////
enum STATE
{
    STATE_IDLE,
    STATE_CONNECTING,
    STATE_CONNECTED,
};

int state = STATE_IDLE;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

spifi_command_t command[COMMAND_NUM] = {
		{PAGE_SIZE, false, kSPIFI_DataInput, 1, kSPIFI_CommandDataQuad, kSPIFI_CommandOpcodeAddrThreeBytes, 0x6B},
		{PAGE_SIZE, false, kSPIFI_DataOutput, 0, kSPIFI_CommandDataQuad, kSPIFI_CommandOpcodeAddrThreeBytes, 0x32},
		{1, false, kSPIFI_DataInput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x05},
		{0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeAddrThreeBytes, 0x20},
		{0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x06},
		{1, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x31}};

void BOARD_InitPWM(void)
{
	sctimer_config_t config;
	sctimer_pwm_signal_param_t pwmParam;
	CLOCK_AttachClk(kMAIN_CLK_to_SCT_CLK);
	CLOCK_SetClkDiv(kCLOCK_DivSctClk, 2, true);
	SCTIMER_GetDefaultConfig(&config);
	SCTIMER_Init(SCT0, &config);
	pwmParam.output = kSCTIMER_Out_5;
	pwmParam.level = kSCTIMER_HighTrue;
	pwmParam.dutyCyclePercent = 20;
	SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 1000U, CLOCK_GetFreq(kCLOCK_BusClk), &event);
//	//SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_L);
//
//	pwmParam.output = kSCTIMER_Out_3;
//	pwmParam.level = kSCTIMER_HighTrue;
//	pwmParam.dutyCyclePercent = 50;
//	SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 6000U, CLOCK_GetFreq(kCLOCK_BusClk), &event2);
	SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_L);
}

void dimmScreen(uint8_t value){
	SCTIMER_UpdatePwmDutycycle(SCT0, 5, value*0.59, event);
}
//int duration[] = {         //duration of each note (in ms) Quarter Note is set to 250 ms
//		250, 125, 250, 125, 250, 125, 750, 400, 500,
//		1500,
//		250, 125, 250, 125, 250, 125, 750, 400, 500,
//		1500,
//		250, 125, 250, 125, 250, 125, 750, 400, 500
//};
//
//void song(){
//	for (int i=0;i<29;i++){              //203 is the total number of music notes in the song
//	  if(i%2 == 0)
//		  SCTIMER_UpdatePwmDutycycle(SCT0, 3, 80, event2);
//	  else
//		  SCTIMER_UpdatePwmDutycycle(SCT0, 3, 0, event2);
//	  vTaskDelay(duration[i]);
//	}                        //delay is used so it doesn't go to the next loop before tone is finished playing
//	SCTIMER_UpdatePwmDutycycle(SCT0, 3, 0, event2);
//}

void check_if_finish();
void saveNetworkInFlash(void);

// Function to connect with Wifi network
bool connectWiF(const char *ssid, const char *password){

	apDisconnect();
	strcpy(wifi_data.WIFI_SSID,ssid);
	strcpy(wifi_data.WIFI_PASSWORD,password);
	wifi_data.SSID_LEN = SSID_LENGTH;
	wifi_data.PASSWORD_LEN = PASSWORD_LENGTH;

	strcpy(g_ssid.ssid,ssid);
	strcpy(g_passphrase.passphrase,password);

	bool dhcpGetted = false;
	int wifiTimer = 0;
	apConnect(&g_ssid, &g_passphrase, g_auth, g_cipher);

	while(wifiTimer < 5){
		if(!dhcpGetted){
			if(isConnected()){
				if(getDhcp()){
					///dispPRINTF("Polaczono\n",label3);
					PRINTF("POL");
					GPIO_PinWrite(GPIO,1,22,1);
					vTaskDelay(MSEC_TO_TICK(200));
					GPIO_PinWrite(GPIO,1,22,0);
					vTaskDelay(MSEC_TO_TICK(200));
					GPIO_PinWrite(GPIO,1,22,1);
					vTaskDelay(MSEC_TO_TICK(200));
					GPIO_PinWrite(GPIO,1,22,0);
					vTaskDelay(MSEC_TO_TICK(200));
					GPIO_PinWrite(GPIO,1,22,1);
					vTaskDelay(MSEC_TO_TICK(400));
					GPIO_PinWrite(GPIO,1,22,0);
					state = STATE_CONNECTED;
					dhcpGetted = true;
					saveNetworkInFlash();
					return true;
				}
				else{
					PRINTF("POL");
					GPIO_PinWrite(GPIO,1,22,1);
					vTaskDelay(MSEC_TO_TICK(900));
					GPIO_PinWrite(GPIO,1,22,0);

				}
			}
		}
		wifiTimer++;
		vTaskDelay(MSEC_TO_TICK(1000));
	}
	return false;
}

// Function to disconnect with Wifi with which we are currently connected
void disconnectWifi(void){
	if(isConnected()){
		GPIO_PinWrite(GPIO,1,22,1);
		vTaskDelay(MSEC_TO_TICK(300));
		GPIO_PinWrite(GPIO,1,22,0);
		vTaskDelay(MSEC_TO_TICK(300));
		GPIO_PinWrite(GPIO,1,22,1);
		vTaskDelay(MSEC_TO_TICK(900));
		GPIO_PinWrite(GPIO,1,22,0);
		GUI_SetActualSSID("");
		apDisconnect();
	}else{
		GPIO_PinWrite(GPIO,1,22,1);
		vTaskDelay(MSEC_TO_TICK(300));
		GPIO_PinWrite(GPIO,1,22,0);
		vTaskDelay(MSEC_TO_TICK(300));
		GPIO_PinWrite(GPIO,1,22,1);
		vTaskDelay(MSEC_TO_TICK(900));
		GPIO_PinWrite(GPIO,1,22,0);
		vTaskDelay(MSEC_TO_TICK(300));
		GPIO_PinWrite(GPIO,1,22,1);
		vTaskDelay(MSEC_TO_TICK(900));
		GPIO_PinWrite(GPIO,1,22,0);
	}
}


// Function to save wifi SSID and Password in SPIFI
void saveNetworkInFlash(void){

	uint32_t data=0, page=0;
	int i=0;
	int j=0;
	int k=0;
	char *ptr = &wifi_data.WIFI_SSID[0];


	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
	SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_NETWORK_DATA_ADDRESS);
	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[ERASE_SECTOR]);
	check_if_finish();



	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
	SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_NETWORK_DATA_ADDRESS + page * PAGE_SIZE);
	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[PROGRAM_PAGE]);
	for (i = 0; i < PAGE_SIZE; i += 4)
	{
		for (j = 0; j < 4; j++)
		{
			if(k < 98){
				data |= ((uint32_t)(*ptr)) << (j * 8);
				ptr++;
				k++;
			}else{
				data = 0;
			}
		}

		SPIFI_WriteData(BOARD_FLASH_SPIFI, data);
		data = 0;
	}

	check_if_finish();


	/* Reset to memory command mode */
	SPIFI_ResetCommand(BOARD_FLASH_SPIFI);
	SPIFI_SetMemoryCommand(BOARD_FLASH_SPIFI, &command[READ]);
}


// Function to read Wifi SSID and Password and connect with it
void autoConnectWifi(void){
	uint8_t *val;
	/* Reset to memory command mode */
	SPIFI_ResetCommand(BOARD_FLASH_SPIFI);

	SPIFI_SetMemoryCommand(BOARD_FLASH_SPIFI, &command[READ]);

	char *readproducts;
	readproducts = &wifi_data.WIFI_SSID[0];
	for (int i = 0; i < SECTOR_SIZE; i++)	// Odczyta to max. 3 produkty, do odczytania 50 produktow zwiekszyc do 56000
	{
		if(i < SSID_LENGTH+PASSWORD_LENGTH){
			val = (uint8_t *)(FSL_FEATURE_SPIFI_START_ADDR+SPIFI_NETWORK_DATA_ADDRESS + i);
			*readproducts = *val;
			readproducts++;

		}
	}

	if(connectWiF(wifi_data.WIFI_SSID,wifi_data.WIFI_PASSWORD))
		GUI_SetActualSSID(wifi_data.WIFI_SSID);
	else
		GUI_SetActualSSID("");

}

// Function to translate products list from smart_fridge file to another for littlevgl use
void translateList(){
	if(products_numb >= 25)
		products_numb = 0;

	for(int i=0;i<products_numb;i++){
		DATA_ProdList[i].category = ((int)products[i].productCategory[0])- '0';

		for(int j=0;j<11;j++){
			DATA_ProdList[i].date[j] = products[i].expirationDate[j];
		}

		int j = 0;
		int k = 0;

		while(products[i].productName[k] != '\0'){
			DATA_ProdList[i].name[j] = products[i].productName[k];
			j++;
			k++;
		}
		DATA_ProdList[i].name[j] = ' ';
		j++;

		k = 0;
		while(products[i].productBrand[k] != '\0'){
			DATA_ProdList[i].name[j] = products[i].productBrand[k];
			j++;
			k++;
		}
		DATA_ProdList[i].name[j] = ' ';
		j++;

		k = 0;
		while(products[i].productDetails[k] != '\0'){
			DATA_ProdList[i].name[j] = products[i].productDetails[k];
			j++;
			k++;
		}
		DATA_ProdList[i].name[j] = ' ';
		j++;

		k = 0;
		while(products[i].productCapacity[k] != '\0'){
			DATA_ProdList[i].name[j] = products[i].productCapacity[k];
			j++;
			k++;
		}
		DATA_ProdList[i].name[j] = '\0';

	}
}

// Function to check if writing to SPIFI is finish
void check_if_finish()
{
    uint8_t val = 0;
    /* Check WIP bit */
    do
    {
        SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[GET_STATUS]);
        while ((BOARD_FLASH_SPIFI->STAT & SPIFI_STAT_INTRQ_MASK) == 0U)
        {
        }
        val = SPIFI_ReadDataByte(BOARD_FLASH_SPIFI);
    } while (val & 0x1);
}



// Function to read products nad shopping list from SPIFI
void loadDataFromFlashMemory(){
	uint8_t *val;
	/* Reset to memory command mode */
	SPIFI_ResetCommand(BOARD_FLASH_SPIFI);

	SPIFI_SetMemoryCommand(BOARD_FLASH_SPIFI, &command[READ]);

	uint8_t *readproducts;
	char *shoplistptr;

	// Read products from flash
	readproducts = &products[0].size;
	for (int i = 0; i < PRODUCTS_DATA_LENGTH_BYTES; i++)
	{
		val = (uint8_t *)(FSL_FEATURE_SPIFI_START_ADDR+SPIFI_DATA_START_ADDRESS + i);


		*readproducts = *val;
		readproducts++;
	}

	// Read number of products from flash
	val = (uint8_t *)(FSL_FEATURE_SPIFI_START_ADDR+SPIFI_NUMBER_OF_PRODUCTS_ADDRESS);
	products_numb = *val;

	// Read shop list from flash
	shoplistptr = &shoplist[0][0];


	bool ptr2 = false;
	bool ptr3 = false;
	bool ptr4 = false;
	bool ptr5 = false;


	for(int i=0;i<SHOPLIST_NAME_SIZE*5;i++){
		val = (uint8_t *)(FSL_FEATURE_SPIFI_START_ADDR+SPIFI_SHOP_LIST_ADDRESS + i);

		if(i >=50 && i<100){
			if(!ptr2){
				shoplistptr = &shoplist[1][0];
				ptr2 = true;
			}
		}else if(i >=100 && i<150){
			if(!ptr3){
				shoplistptr = &shoplist[2][0];
				ptr3 = true;
			}
		}else if(i >=150 && i<200){
			if(!ptr4){
				shoplistptr = &shoplist[3][0];
				ptr4 = true;
			}
		}else if(i >=200 && i<250){
			if(!ptr5){
				shoplistptr = &shoplist[4][0];
				ptr5 = true;
			}
		}
		*shoplistptr = *val;
		shoplistptr++;
	}
}

// Function to save products and shopping list to SPIFI
void saveDataInFlash(){

	/* Reset the SPIFI to switch to command mode */
	SPIFI_ResetCommand(BOARD_FLASH_SPIFI);

	EnableIRQ(SPIFI0_IRQn);


	// WRITE TO SPIFI
	uint8_t *productsptr;
	char *shoplistptr;
	uint32_t data=0, page=0;
	int i=0;
	int j=0;
	int k=0;
	int l=0;
	productsptr = &products[0].size;

	/* Write enable */
	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
	/* Set address */
	SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, 0U+SPIFI_DATA_START_ADDRESS+l*4096);
	/* Erase sector */
	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[ERASE_SECTOR]);
	/* Check if finished */
	check_if_finish();


	while (page < 14)
	{
		SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
		SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_DATA_START_ADDRESS+l*4096+ page * PAGE_SIZE);
		SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[PROGRAM_PAGE]);
		for (i = 0; i < PAGE_SIZE; i += 4)
		{
			if(k < PRODUCTS_DATA_LENGTH_BYTES){
				for (j = 0; j < 4; j++)
				{
					if(k < PRODUCTS_DATA_LENGTH_BYTES){
						data |= ((uint32_t)(*productsptr)) << (j * 8);
						productsptr++;
						k++;
					}else{
						data|=((uint32_t)(0)) << (j * 8);
					}
				}
				SPIFI_WriteData(BOARD_FLASH_SPIFI, data);
				data = 0;
			}else{
				SPIFI_WriteData(BOARD_FLASH_SPIFI, data);
				data = 0;
			}
		}
		page++;
		check_if_finish();
	}

	//vTaskDelay(200);



	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
	/* Set address */
	SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_NUMBER_OF_PRODUCTS_ADDRESS);
	/* Erase sector */
	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[ERASE_SECTOR]);
	/* Check if finished */
	check_if_finish();
	// WRITE NUMBER OF PRODUCTS
	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
	SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_NUMBER_OF_PRODUCTS_ADDRESS);
	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[PROGRAM_PAGE]);

	for (i = 0; i < PAGE_SIZE; i += 4)
	{
		if(i == 0){
			SPIFI_WriteData(BOARD_FLASH_SPIFI, products_numb);

		}else{
			for (j = 0; j < 4; j++)
			{
				data |= ((uint32_t)(0)) << (j * 8);

			}
			SPIFI_WriteData(BOARD_FLASH_SPIFI, data);
		}

	}
	check_if_finish();



	// WRITE SHOP LIST
	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
	SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_SHOP_LIST_ADDRESS);
	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[ERASE_SECTOR]);
	check_if_finish();



	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
	SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_SHOP_LIST_ADDRESS);
	SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[PROGRAM_PAGE]);

	k=0;
	data = 0;
	bool ptr2 = false;
	bool ptr3 = false;
	bool ptr4 = false;
	bool ptr5 = false;
	shoplistptr = &shoplist[0][0];
	for (i = 0; i < PAGE_SIZE; i += 4)
	{

		for (j = 0; j < 4; j++)
		{
			if(i+j >=50 && i<100){
				if(!ptr2){
					shoplistptr = &shoplist[1][0];
					ptr2 = true;
				}
			}else if(i+j >=100 && i<150){
				if(!ptr3){
					shoplistptr = &shoplist[2][0];
					ptr3 = true;
				}
			}else if(i+j >=150 && i<200){
				if(!ptr4){
					shoplistptr = &shoplist[3][0];
					ptr4 = true;
				}
			}else if(i+j >=200 && i<250){
				if(!ptr5){
					shoplistptr = &shoplist[4][0];
					ptr5 = true;
				}
			}else{

			}
			if(i < 250){
				data |= ((uint32_t)(*shoplistptr)) << (j * 8);
				shoplistptr++;
			}else{
				data = 0;
			}
		}

		SPIFI_WriteData(BOARD_FLASH_SPIFI, data);
		data = 0;

	}
	check_if_finish();

	/* Reset to memory command mode */
	SPIFI_ResetCommand(BOARD_FLASH_SPIFI);
	SPIFI_SetMemoryCommand(BOARD_FLASH_SPIFI, &command[READ]);
	flashUpdateAvailable = false;
	shopListChanged = false;
}


// Function to scan networks and write it to list from which littlevgl will update
// it on screen
static void scanNetworks(){
	apScan(DATA_Networks);
}



// WiFi task
// This task is checking every 20 s that is there any change in products/shopping list - if
// there is then save products list and shopping list in SPIFI and if we are connected
// to wifi send it to ThingSpeak server
void task_main(void *param)
{
    int32_t result = 0;
    (void)result;

    /* Initialize WIFI shield */
    result = WIFISHIELD_Init();
    PRINTF("CREATING WIFI TASK1\r\n");
    assert(A_OK == result);

    /* Initialize the WIFI driver (thus starting the driver task) */
    result = wlan_driver_start();
    PRINTF("CREATING WIFI TASK2\r\n");
    assert(A_OK == result);

    // Buzzer signal, it tell us that wifi module starts
    GPIO_PinWrite(GPIO,1,22,1);
    vTaskDelay(MSEC_TO_TICK(500));
    GPIO_PinWrite(GPIO,1,22,0);

    autoConnectWifi();



    while (1)
    {
    	if(flashUpdateAvailable || shopListChanged){
    		thingSpeak_shopListChanged = shopListChanged;
    		saveDataInFlash();
    	}

    	if(thingSpeak_UpdateAvailable || thingSpeak_shopListChanged){

    		if(isConnected() && state == STATE_CONNECTED){
    			SF_sendProductsToThingSpeak(shoplist);
    		}
    	}

    	vTaskDelay(MSEC_TO_TICK(20000));
    }
}


// Littlevgl task
static void AppTask(void *param)
{

    lv_port_pre_init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    s_lvgl_initialized = true;


    GUI_Init();
    GUI_SetProductList(DATA_ProdList, &products_numb);
    GUI_SetDate(&DATA_Date);
    GUI_SetDataChanged(&DATA_DataChanged);
    GUI_SetShopListChanged(&shopListChanged);
    GUI_SetNetworkList(DATA_Networks);

    // WIFI Events
    GUI_SetScanNetworkFun(scanNetworks);
    GUI_SetConnectNetworkFun(connectWiF);
    GUI_SetDisconnectNetworkFun(disconnectWifi);
    GUI_SetBrightnessFun(dimmScreen);
    shoplist = GUI_GetShopList();

#if INITIAL_PRODUCTS == 1
    translateList();
    DATA_DataChanged = true;
#else
    loadDataFromFlashMemory();
    translateList();
    DATA_DataChanged = true;
#endif



    for (;;)
    {
        lv_task_handler();
    	vTaskDelay(5);
    }
}

// Task for get current time from http server, synchronize with RTC and every 1 s update int variable for littlevgl
static void task_updateTime(void *param){
	while(state != STATE_CONNECTED){
		vTaskDelay(MSEC_TO_TICK(1000));
	}

	char timeBuff[500];
	char timeTable[30];
	uint32_t number;
	rtc_datetime_t time_date_struct;
	int timeBuffLength;

	httpGet(TIME_SERVER,"UnixTime/tounix?date=now",timeBuff);
	timeBuffLength = strlen(timeBuff);

	for(int i=0;i<11;i++)
		timeTable[i] = timeBuff[timeBuffLength-10+i];

	number = atoi(timeTable);
	// Time from this server is 1 hour behind - add one hour
	number = number + 3604;
	RTC_SecondToDateTime(number,&time_date_struct);
	RTC_EnableTimer(RTC, false);

	/* Sync time in RTC with this from http server */
	RTC_SetDatetime(RTC, &time_date_struct);

	/* Start the RTC time counter */
	RTC_EnableTimer(RTC, true);
	DATA_Date = RTC_GetSecondFromRTC(RTC);
	DATA_DataChanged = true;
	while(1){
		DATA_Date = RTC_GetSecondFromRTC(RTC);
		vTaskDelay(MSEC_TO_TICK(1000));
	}

}

// Task for checking if any RFID card is in reader range, if is read data and add to list
static void RFID_Task(void *param){

	SF_startRFID_Module(EXAMPLE_SPI_MASTER,4,7);	//Init RC522, reset
	//song();
	while (1)
	{
		if(SF_detectProduct()){
			translateList();
			DATA_DataChanged = true;
			for(int i=0;i<1000000;i++);

		}

		vTaskDelay(MSEC_TO_TICK(460));

	}
}


int main(void)
{
	   BaseType_t stat;
       CLOCK_EnableClock(kCLOCK_InputMux);
       BOARD_InitBootPins();
       BOARD_BootClockPLL220M();
       BOARD_InitBootClocks();
       BOARD_InitSDRAM();
       BOARD_InitPeripherals();
       BOARD_InitPWM();
       SYSCON->RTCOSCCTRL |= SYSCON_RTCOSCCTRL_EN_MASK;

       RTC_Init(RTC);

       BOARD_InitQSPI_FLASH();

       stat = xTaskCreate(AppTask, "littlevgl", configMINIMAL_STACK_SIZE + 800, NULL, tskIDLE_PRIORITY + 2, NULL);

       if (pdPASS != stat)
       {
    	   PRINTF("Failed to create littlevgl task");
    	   while (1)
    		   ;
       }

       stat = xTaskCreate(task_main, "main", TASK_MAIN_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &task_main_task_handler);
       assert(pdPASS == stat);

       if (pdPASS != stat)
       {
    	   PRINTF("Failed to create wifi task");
    	   while (1)
    		   ;
       }

       stat = xTaskCreate(RFID_Task, "RFID Detector", configMINIMAL_STACK_SIZE + 1000, NULL, tskIDLE_PRIORITY + 2, NULL);

       if (pdPASS != stat)
       {
    	   PRINTF("Failed to create RC522 task");
    	   while (1)
    		   ;
       }


       stat = xTaskCreate(task_updateTime, "Update time", configMINIMAL_STACK_SIZE + 800, NULL, tskIDLE_PRIORITY + 2, NULL);

       if (pdPASS != stat)
       {
    	   PRINTF("Failed to create updateTime task");
    	   while (1)
    		   ;
       }

       vTaskStartScheduler();

    for (;;)
    {
    } /* should never get here */

}

/*!
 * @brief Malloc failed hook.
 */
void vApplicationMallocFailedHook(void)
{
    for (;;)
        ;
}

/*!
 * @brief FreeRTOS tick hook.
 */
void vApplicationTickHook(void)
{

    if (s_lvgl_initialized)
    {
        lv_tick_inc(1);
    }
}

/*!
 * @brief Stack overflow hook.
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    (void)pcTaskName;
    (void)xTask;

    for (;;)
        ;
}

