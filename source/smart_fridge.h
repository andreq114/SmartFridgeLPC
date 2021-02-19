/*
 * smart_fridge.h
 *
 *  Created on: 22 lis 2020
 *      Author: andre
 */

#ifndef SMART_FRIDGE_H_
#define SMART_FRIDGE_H_
#include <stdlib.h>
#include <cr_section_macros.h>

#include "RC522.h"

#include <stdbool.h>
#include "fsl_gpio.h"
#include "fsl_pint.h"
#include "lvgl.h"
#include "stdio.h"
#include "qcom_api.h"
#include "wlan_qcom.h"
#include "data.h"

#define INITIAL_PRODUCTS 0  // If we want to have some products on start


// -------------------------------------- MIFARE 1k BLOCKS ------------------------------
// SECTOR ------- BLOCK ------- DATA
// 	 3	  -------  12   ------- PRODUCT_NAME_FIRST_16_BYTES
//        -------  13   ------- PRODUCT_NAME_SECOND_16_BYTES
//        -------  14   ------- PRODUCT_CAPACITY_16_BYTES
//        -------  15   ------- TRAILER_BLOCK

// 	 4	  -------  16   ------- PRODUCT_BRAND_FIRST_16_BYTES
//        -------  17   ------- PRODUCT_BRAND_SECOND_16_BYTES
//        -------  18   ------- PRODUCT_EXPIRATION_DATE_16_BYTES
//        -------  19   ------- TRAILER_BLOCK

// 	 5	  -------  20   ------- PRODUCT_DETAILS_FIRST_16_BYTES
//        -------  21   ------- PRODUCT_DETAILS_SECOND_16_BYTES
//        -------  22   ------- PRODUCT_CATEGORY_16_BYTES
//        -------  23   ------- TRAILER_BLOCK


// ----------------------------------- NTAG215 PAGES ------------------------------
// PAGE --------- DATA
//  8   --------- PRODUCT_NAME_4_BYTES
//  9   --------- PRODUCT_NAME_4_BYTES
//  10  --------- PRODUCT_NAME_4_BYTES
//  11  --------- PRODUCT_NAME_4_BYTES
//  12  --------- PRODUCT_NAME_4_BYTES
//  13  --------- PRODUCT_NAME_4_BYTES
//  14  --------- PRODUCT_NAME_4_BYTES
//  15  --------- PRODUCT_NAME_4_BYTES

//  16  --------- PRODUCT_BRAND_4_BYTES
//  17  --------- PRODUCT_BRAND_4_BYTES
//  18  --------- PRODUCT_BRAND_4_BYTES
//  19  --------- PRODUCT_BRAND_4_BYTES
//  20  --------- PRODUCT_BRAND_4_BYTES
//  21  --------- PRODUCT_BRAND_4_BYTES
//  22  --------- PRODUCT_BRAND_4_BYTES
//  23  --------- PRODUCT_BRAND_4_BYTES

//  24  --------- PRODUCT_CAPACITY_4_BYTES
//  25  --------- PRODUCT_CAPACITY_4_BYTES
//  26  --------- PRODUCT_CAPACITY_4_BYTES
//  27  --------- PRODUCT_CAPACITY_4_BYTES

//  28  --------- PRODUCT_DETAILS_4_BYTES
//  29  --------- PRODUCT_DETAILS_4_BYTES
//  30  --------- PRODUCT_DETAILS_4_BYTES
//  31  --------- PRODUCT_DETAILS_4_BYTES
//  32  --------- PRODUCT_DETAILS_4_BYTES
//  33  --------- PRODUCT_DETAILS_4_BYTES
//  34  --------- PRODUCT_DETAILS_4_BYTES
//  35  --------- PRODUCT_DETAILS_4_BYTES

//  36  --------- PRODUCT_EXPIRATION_DATE_4_BYTES
//  37  --------- PRODUCT_EXPIRATION_DATE_4_BYTES
//  38  --------- PRODUCT_EXPIRATION_DATE_4_BYTES
//  39  --------- PRODUCT_EXPIRATION_DATE_4_BYTES

//  40  --------- PRODUCT_CATEGORY_4_BYTES
//  41  --------- PRODUCT_CATEGORY_4_BYTES
//  42  --------- PRODUCT_CATEGORY_4_BYTES
//  43  --------- PRODUCT_CATEGORY_4_BYTES

#define THIRD_SECTOR_TRAILER 15
#define FOURTH_SECTOR_TRAILER 19
#define FIFTH_SECTOR_TRAILER 23


#define WWW_SERVER "api.thingspeak.com"





typedef struct {
	uint8_t size; 					// Number of bytes in the UID. 4, 7 or 10.
	uint8_t uidByte[10];			// ID of card
	uint8_t productName[33];		// Name of product ex. Chocolate
	uint8_t productBrand[33];		// Brand of product ex. Wedel
	uint8_t productDetails[33];		// Details of product ex. Strawberry
	uint8_t productCapacity[17];	// Capacity of product ex. 100 g
	int8_t expirationDate[11];		// Data from block 16 - expiration date of product ex. 31.08.2020
	uint8_t productCategory[2];		// Data from block 17 - category of product ex. 1 (Means for example Alcohol)
} Product;

enum TARGET{
	MIFARE,
	NTAG215,
};


extern Product products[];
extern uint32_t products_numb;
extern bool listChanged;
extern volatile bool thingSpeakUpdateAvailable;
extern volatile bool flashUpdateAvailable;
extern volatile bool shopListChanged;
extern volatile bool flashShopListChanged;

void groupProductData(char (*shoplist)[SHOPLIST_NAME_SIZE]);
extern void SF_sendProductsToThingSpeak(char (*shoplist)[SHOPLIST_NAME_SIZE]);

void SF_startRFID_Module(SPI_Type *base,int rstPort,int rstPin);

bool SF_detectProduct();
void sortList();
void NTAG215_read_config();
void MIFARE_read_config();
bool readProductsData(Uid product_uid);

#endif /* SMART_FRIDGE_H_ */
