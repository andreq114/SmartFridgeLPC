/*
 * GUI_ExtData.h
 *
 *  Created on: Jan 7, 2021
 *      Author: Owner
 */

#ifndef GUI_EXTDATA_H_
#define GUI_EXTDATA_H_

#include "stdint.h"
#include "stdbool.h"

#define SHOPLIST_NAMES_LENGTH	10
#define SHOPLIST_NAME_SIZE 		50

typedef void( * scanNetFun )();
typedef bool( * connectNetFun )(const char*, const char*);
typedef void( * disconnectNetFun )();

enum {
    DATA_DAIRY,
	DATA_DRINKS,
	DATA_ALCOHOLS,
	DATA_SAUCES,
	DATA_SWEETS,
	DATA_FISHES,
	DATA_MEAT,
	DATA_FROZEN,
	DATA_PLANTS
};
typedef uint8_t GUI_DATA_CATEGORIES;

typedef struct {
	uint32_t uid;
	char name[49];
	char date[11];
	GUI_DATA_CATEGORIES category;
} GUI_Data_ProductList;


extern uint32_t * GUI_Date;
extern bool * GUI_DataChanged;
extern volatile bool * GUI_ShopListChanged;
extern char** GUI_NetworkList;
extern char GUI_ShopList[SHOPLIST_NAMES_LENGTH][SHOPLIST_NAME_SIZE];
extern uint32_t GUI_ShopListProducts;
extern scanNetFun GUI_ScanNetworks;
extern connectNetFun GUI_ConnectNetwork;
extern disconnectNetFun GUI_DisconnectNetwork;

#endif /* GUI_EXTDATA_H_ */
