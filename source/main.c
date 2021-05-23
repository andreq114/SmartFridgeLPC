/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


// Thesis name: Sterownik inteligentnej lodówki z panelem graficznym
// Authors:
// Andrzej Przybylo
// Kamil Wielgosz

// Tested on Release - Optimization O2
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


// SPIFI ADRESSES
#define SPIFI_DATA_START_ADDRESS 200704
#define SPIFI_NUMBER_OF_PRODUCTS_ADDRESS SPIFI_DATA_START_ADDRESS+56001
#define SPIFI_SHOP_LIST_ADDRESS 266240
#define SPIFI_NETWORK_DATA_ADDRESS 274432


// Network information sizes
#define SSID_LENGTH 33
#define PASSWORD_LENGTH 65

#define NETWORK_DATA_LENGTH SSID_LENGTH+PASSWORD_LENGTH


// RFID SPI Interface
#define RFID_MODULE_SPI          SPI0

static volatile bool s_lvgl_initialized = false;



// --------------------------------------------------- WIFI ---------------------------------------------

portSTACK_TYPE *task_main_stack = NULL;
TaskHandle_t task_main_task_handler;

// Structure to safe informations about network with which we have already connected
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

bool manualDisconnect = false;

enum STATE
{
    STATE_IDLE,
    STATE_CONNECTING,
    STATE_CONNECTED,
};

int state = STATE_IDLE;

extern int numIrqs;
extern int initTime;

//-------------------------------------- END WiFi ----------------------------------------------------------------//


// PWM Event
uint32_t event;


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
	pwmParam.dutyCyclePercent = 60;
	SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 1000U, CLOCK_GetFreq(kCLOCK_BusClk), &event);
	SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_L);
}

void dimmScreen(uint8_t value){
	SCTIMER_UpdatePwmDutycycle(SCT0, 5, value*0.59, event);
}


void check_if_finish();
void saveNetworkInFlash(void);
void disconnectWifi(void);

// Function to connect with Wifi network
bool connectWiFi(const char *ssid, const char *password){
	disconnectWifi();
	bool dhcpGetted = false;
	int wifiTimer = 0;
	strcpy(wifi_data.WIFI_SSID,ssid);
	strcpy(wifi_data.WIFI_PASSWORD,password);
	wifi_data.SSID_LEN = SSID_LENGTH;
	wifi_data.PASSWORD_LEN = PASSWORD_LENGTH;

	strcpy(g_ssid.ssid,ssid);
	strcpy(g_passphrase.passphrase,password);

	apConnect(&g_ssid, &g_passphrase, g_auth, g_cipher);

	while(wifiTimer < 10){
			if(!dhcpGetted){
				if(isConnected()){
					if(getDhcp()){
						PRINTF("Connected, DHCP address getted");
						networkConnectedSignal();

						state = STATE_CONNECTED;
						dhcpGetted = true;
						saveNetworkInFlash();
						GUI_SetActualSSID(wifi_data.WIFI_SSID);
						manualDisconnect = false;
						return true;
					}
					else{
						GUI_SetActualSSID("");
						PRINTF("Getting DHCP address failed");
					}
				}
			}
			wifiTimer++;
			vTaskDelay(MSEC_TO_TICK(1000));
		}

#if TEST_MODE == 1
	PRINTF("POLACZENIE Z SIECIA NIEUDANE.\r\n");
#endif
	return false;
}

// Function to disconnect with Wifi with which we are currently connected
void disconnectWifi(void){
		GUI_SetActualSSID("");
		apDisconnect();
		state = STATE_IDLE;
		manualDisconnect = true;
}
// Function for display to disconnect Wifi connection
void manualDisconnectWifi(void){
	networkDisconnectSignal();
	disconnectWifi();
}


// Function to save wifi SSID and Password in SPIFI
void saveNetworkInFlash(void){
	uint32_t data=0, page=0;
	int i=0;
	int j=0;
	int k=0;
	char *ptr = &wifi_data.WIFI_SSID[0];

#if TEST_MODE == 1
	PRINTF("ZAPISYWANIE NASTEPUJACEJ SIECI W PAMIECI FLASH:\r\n");
	PRINTF("NAZWA SIECI: %s\r\n",wifi_data.WIFI_SSID);
	PRINTF("HASLO SIECI: %s\r\n",wifi_data.WIFI_PASSWORD);
#endif


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

	SPIFI_ResetCommand(BOARD_FLASH_SPIFI);
	SPIFI_SetMemoryCommand(BOARD_FLASH_SPIFI, &command[READ]);
}


// Function to read Wifi SSID and Password and connect with it
void autoConnectWifi(void){
	uint8_t *val;
	char *readproducts;

	SPIFI_ResetCommand(BOARD_FLASH_SPIFI);
	SPIFI_SetMemoryCommand(BOARD_FLASH_SPIFI, &command[READ]);

	readproducts = &wifi_data.WIFI_SSID[0];
	for (int i = 0; i < SECTOR_SIZE; i++)
	{
		if(i < SSID_LENGTH+PASSWORD_LENGTH){
			val = (uint8_t *)(FSL_FEATURE_SPIFI_START_ADDR+SPIFI_NETWORK_DATA_ADDRESS + i);
			*readproducts = *val;
			readproducts++;
		}
	}
#if TEST_MODE == 1
	PRINTF("\r\n");
	PRINTF("-- AUTOMATYCZNE LACZENIE Z ZAPAMIETANA SIECIA WIFI --\r\n");
	PRINTF("SIEC ODCZYTANA Z PAMIECI FLASH\r\n");
	PRINTF("NAZWA SIECI: %s\r\n",wifi_data.WIFI_SSID);
	PRINTF("HASLO SIECI: %s\r\n",wifi_data.WIFI_PASSWORD);
	PRINTF("\r\nPROBA POLACZENIA...\r\n");
#endif

	connectWiFi(wifi_data.WIFI_SSID,wifi_data.WIFI_PASSWORD);

}

// Function to translate products list from smart_fridge file to another for littlevgl use
void translateList(){
	int j;
	int k;
	if(products_numb >= 25)
		products_numb = 0;

	for(int i=0;i<products_numb;i++){

		for(int j=0;j<10;j++){
			DATA_ProdList[i].uid[j] = products[i].uidByte[j];
		}

		DATA_ProdList[i].category = ((int)products[i].productCategory[0])- '0';

		for(int j=0;j<11;j++){
			DATA_ProdList[i].date[j] = products[i].expirationDate[j];
		}

		j = 0;
		k = 0;

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

	SPIFI_ResetCommand(BOARD_FLASH_SPIFI);
	SPIFI_SetMemoryCommand(BOARD_FLASH_SPIFI, &command[READ]);

	uint8_t *val;
	volatile uint8_t *  readproducts;
	char *shoplistptr;
	int shopListIndex = 0;

	// Read products from flash
	readproducts = &products[0].size;
	for (int i = 0; i < MAX_NUMBER_OF_PRODUCTS*ONE_PRODUCT_SIZE; i++)
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

	for(int i=0;i<SHOPLIST_NAME_SIZE*SHOPLIST_NAMES_LENGTH;i++){
		val = (uint8_t *)(FSL_FEATURE_SPIFI_START_ADDR+SPIFI_SHOP_LIST_ADDRESS + i);
		if(i % SHOPLIST_NAME_SIZE == 0){
			shoplistptr = &shoplist[shopListIndex][0];
			shopListIndex++;
		}
		*shoplistptr = *val;
		shoplistptr++;
	}
#if TEST_MODE == 1
	PRINTF("\r\nWCZYTYWANIE LISTY PRODUKTOW Z PAMIECI ZEWNETRZNEJ FLASH...\r\n");
	for(int i=0;i<products_numb;i++){
		PRINTF("PRODUKT NUMER %d\r\nNAZWA PRODUKTU: %s %s %s %s\r\n",i+1,products[i].productName,products[i].productBrand,products[i].productDetails,products[i].productCapacity);
		PRINTF("NUMER KATEGORI: %s\r\n",products[i].productCategory);
		PRINTF("DATA WAZNOSCI: %s\r\n\r\n",products[i].expirationDate);
	}
	PRINTF("LISTA PRODUKTOW ZAKTUALIZOWANA.\r\n");
#endif
}

void saveProductsInFlash(void){
	/* Reset the SPIFI to switch to command mode */
#if TEST_MODE == 1
			PRINTF("ZAPISYWANIE LISTY PRODUKTOW DO PAMIECI FLASH\r\n");
#endif
		SPIFI_ResetCommand(BOARD_FLASH_SPIFI);

		EnableIRQ(SPIFI0_IRQn);

		// WRITE TO SPIFI
		volatile uint8_t *productsptr;

		uint32_t data=0, page=0;
		int i=0;
		int j=0;
		int k=0;
		productsptr = &products[0].size;
		int pagesNeeded;

		//Write informations about products------------------------------------------------
		if(MAX_NUMBER_OF_PRODUCTS*ONE_PRODUCT_SIZE%256 == 0)
			pagesNeeded = MAX_NUMBER_OF_PRODUCTS*ONE_PRODUCT_SIZE/256;
		else
			pagesNeeded = (MAX_NUMBER_OF_PRODUCTS*ONE_PRODUCT_SIZE/256)+1;

		//Saving in one sector, maximum products is 4096/140 = 29

		SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
		SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, 0U+SPIFI_DATA_START_ADDRESS);
		SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[ERASE_SECTOR]);
		check_if_finish();

		while (page < pagesNeeded)
		{
			SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
			SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_DATA_START_ADDRESS+ page * PAGE_SIZE);
			SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[PROGRAM_PAGE]);
			for (i = 0; i < PAGE_SIZE; i += 4)
			{
				if(k < MAX_NUMBER_OF_PRODUCTS*ONE_PRODUCT_SIZE){
					for (j = 0; j < 4; j++)
					{
						if(k < MAX_NUMBER_OF_PRODUCTS*ONE_PRODUCT_SIZE){
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



		// WRITE NUMBER OF PRODUCTS----------------------------------------------------
		SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
		/* Set address */
		SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_NUMBER_OF_PRODUCTS_ADDRESS);
		/* Erase sector */
		SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[ERASE_SECTOR]);
		/* Check if finished */
		check_if_finish();

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

		/* Reset to memory command mode */
		SPIFI_ResetCommand(BOARD_FLASH_SPIFI);
		SPIFI_SetMemoryCommand(BOARD_FLASH_SPIFI, &command[READ]);
		//flashUpdateAvailable = false;
}

void saveShopListInFlash(void){
	// WRITE SHOP LIST
#if TEST_MODE == 1
			PRINTF("ZAPISYWANIE LISTY ZAKUPOW DO PAMIECI FLASH\r\n");
#endif

		SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
		SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_SHOP_LIST_ADDRESS);
		SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[ERASE_SECTOR]);
		check_if_finish();


		char *shoplistptr;
		int data = 0;
		int i;
		int j;
		int shopListIndex = 0;
		int page = 0;
		int pagesNeeded;
		if(SHOPLIST_NAMES_LENGTH*SHOPLIST_NAME_SIZE % 256 == 0)
			pagesNeeded = SHOPLIST_NAMES_LENGTH*SHOPLIST_NAME_SIZE/256;
		else
			pagesNeeded = (SHOPLIST_NAMES_LENGTH*SHOPLIST_NAME_SIZE/256)+1;



		//PRINTF("\r\nPotrzebne strony pamieci: %d\r\n",pagesNeeded);
		shoplistptr = &shoplist[0][0];
		while(page < pagesNeeded){
			SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[WRITE_ENABLE]);
			SPIFI_SetCommandAddress(BOARD_FLASH_SPIFI, SPIFI_SHOP_LIST_ADDRESS+page*PAGE_SIZE);
			SPIFI_SetCommand(BOARD_FLASH_SPIFI, &command[PROGRAM_PAGE]);

			for (i = 0; i < PAGE_SIZE; i += 4)
			{

				for (j = 0; j < 4; j++)
				{
					if((((page*PAGE_SIZE)+i+j)-SHOPLIST_NAME_SIZE)%SHOPLIST_NAME_SIZE == 0){
						//PRINTF("\r\nUSTAWIAM WSKAZNIK NA %d ELEMENT\r\n",shopListIndex);
						//PRINTF("\r\nNUMER BAJTU: %d\r\n",page*PAGE_SIZE+i+j);
						shoplistptr = &shoplist[shopListIndex][0];
						shopListIndex++;
					}

					if((page*PAGE_SIZE+i+j) < SHOPLIST_NAME_SIZE*SHOPLIST_NAMES_LENGTH){
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
			page++;
		}


		/* Reset to memory command mode */
		SPIFI_ResetCommand(BOARD_FLASH_SPIFI);
		SPIFI_SetMemoryCommand(BOARD_FLASH_SPIFI, &command[READ]);

		//shopListChanged = false;
}


// Function to scan networks and write it to list from which littlevgl will update
// it on screen
static void scanNetworks(void){
	apScan(DATA_Networks);
}

void deleteProduct(uint8_t *uid){
	if(deleteProductFromList(uid)){
		translateList();
		DATA_DataChanged = true;
		removeProductSignal();
		flashUpdateAvailable = true;
		thingSpeak_UpdateAvailable = true;
	}
}



// WiFi task
// This task is checking every 20 s that is there any change in products/shopping list - if
// there is then save products list and shopping list in SPIFI and if we are connected
// to wifi send it to ThingSpeak server
void task_updateData(void *param)
{
#if TEST_MODE == 1
	rtc_datetime_t time_date_struct;
#endif
	int32_t result = 0;
	(void)result;

	/* Initialize WIFI shield */
	result = WIFISHIELD_Init();
	//PRINTF("CREATING WIFI TASK1\r\n");
	assert(A_OK == result);

	/* Initialize the WIFI driver (thus starting the driver task) */
	result = wlan_driver_start();
	//PRINTF("CREATING WIFI TASK2\r\n");
	assert(A_OK == result);

	// Buzzer signal, it tell us that wifi module starts
	wifiModuleInitSignal();

	// Try to connect with wifi network saved in flash memory
	autoConnectWifi();
	scanNetworks();



	while (1)
	{
#if TEST_MODE == 1
		RTC_GetDatetime(RTC,&time_date_struct);

		PRINTF("GODZINA: %2d:%2d:%2d\r\n",time_date_struct.hour,time_date_struct.minute,time_date_struct.second);
#endif
		if(flashUpdateAvailable){
			flashUpdateAvailable = false;
#if TEST_MODE == 1
			PRINTF("WYKRYTO ZMIANE NA LISCIE PRODUKTOW\r\n");
#endif
			saveProductsInFlash();
		}else{
#if TEST_MODE == 1
			PRINTF("BRAK ZMIAN NA LISCIE PRODUKTOW\r\n");
#endif
		}
		if(shopListChanged){
#if TEST_MODE == 1
			PRINTF("WYKRYTO ZMIANE NA LISCIE ZAKUPOW\r\n");
#endif
			GUI_SetShopListChanged(&shopListChanged);
			thingSpeak_shopListChanged = shopListChanged;
			shopListChanged = false;
			saveShopListInFlash();
		}else{
#if TEST_MODE == 1
			PRINTF("BRAK ZMIAN NA LISCIE ZAKUPOW\r\n");
#endif
		}
		if(isConnected()){
			if(thingSpeak_UpdateAvailable || thingSpeak_shopListChanged){
				thingSpeak_UpdateAvailable = false;
				thingSpeak_shopListChanged = false;
				SF_sendProductsToThingSpeak(shoplist);
			}
		}else{
#if TEST_MODE == 1
			PRINTF("BRAK POLACZENIA Z SIECIA\r\n");
#endif
			if(!manualDisconnect){
#if TEST_MODE == 1
			PRINTF("ROZPOCZYNAM PROBE PRZYWROCENIA SIECI...\r\n");
#endif
				GUI_SetActualSSID("");
				autoConnectWifi();
			}


		}
		vTaskDelay(MSEC_TO_TICK(20000));
	}
}



// Littlevgl task
static void task_Display(void *param)
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
    GUI_SetConnectNetworkFun(connectWiFi);
    GUI_SetDisconnectNetworkFun(manualDisconnectWifi);
    GUI_SetBrightnessFun(dimmScreen);
    shoplist = GUI_GetShopList();

    GUI_SetDelProductFun(deleteProduct);

    //GUI_SetActualSSID(wifi_data.WIFI_SSID);

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
#if TEST_MODE == 1
	PRINTF("\r\nAKTUALIZOWANIE DATY ORAZ GODZINY...\r\n");
#endif
	char timeBuff[500];
	char timeTable[20];
	uint32_t number;
	rtc_datetime_t time_date_struct;
	int timeBuffLength;

	httpGet(TIME_SERVER,"UnixTime/tounix?date=now",timeBuff);
	timeBuffLength = strlen(timeBuff);

	for(int i=0;i<10;i++)
		timeTable[i] = timeBuff[timeBuffLength-11+i];

	number = atoi(timeTable);
	// Time from this server is 1 hour behind - add one hour
	//number = number + 7200;
	number = 1620914160 + 7200;
	RTC_SecondToDateTime(number,&time_date_struct);
	RTC_EnableTimer(RTC, false);

	/* Sync time in RTC with this from http server */
	RTC_SetDatetime(RTC, &time_date_struct);

	/* Start the RTC time counter */
	RTC_EnableTimer(RTC, true);
	DATA_Date = RTC_GetSecondFromRTC(RTC);
	// We need to tell Littlevgl that we set new time to calculate time to expirate products
	DATA_DataChanged = true;
#if TEST_MODE == 1
	PRINTF("DATA ORAZ GODZINA ZOSTALY ZAKTUALIZOWANE\r\n");
#endif
	while(1){
		DATA_Date = RTC_GetSecondFromRTC(RTC);
		vTaskDelay(MSEC_TO_TICK(1000));

	}

}

// Task for checking if any RFID card is in reader range, if is read data and add to list
static void task_readRFID(void *param){

	SF_startRFID_Module(RFID_MODULE_SPI,4,7);	//Init RC522, reset
	//song();
	while (1)
	{
		if(SF_detectProduct()){
			translateList();
			DATA_DataChanged = true;
		}
		vTaskDelay(MSEC_TO_TICK(500));
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

       // Init PWM for screen dimming
       // Cause of crashing on Debug mode, works without dimming when we disable PWM pin of display
       BOARD_InitPWM();

       // Configure RTC
       SYSCON->RTCOSCCTRL |= SYSCON_RTCOSCCTRL_EN_MASK;
       RTC_Init(RTC);

       // Configure QSPI Flash for icons and saving products
       BOARD_InitQSPI_FLASH();

       stat = xTaskCreate(task_Display, "Littlevgl task", configMINIMAL_STACK_SIZE + 800, NULL, tskIDLE_PRIORITY + 2, NULL);
       if (pdPASS != stat)
       {
    	   PRINTF("Failed to create littlevgl task");
    	   while (1)
    		   ;
       }

       stat = xTaskCreate(task_updateData, "Wi-Fi task", 890, NULL, tskIDLE_PRIORITY + 2, &task_main_task_handler);
       assert(pdPASS == stat);

       if (pdPASS != stat)
       {
    	   PRINTF("Failed to create wifi task");
    	   while (1)
    		   ;
       }

       stat = xTaskCreate(task_readRFID, "RFID Detector", configMINIMAL_STACK_SIZE + 1000, NULL, tskIDLE_PRIORITY + 2, NULL);

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

