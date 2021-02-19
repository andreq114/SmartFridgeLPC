/*
 * GUI_ExtData.c
 *
 *  Created on: Jan 7, 2021
 *      Author: Owner
 */


#include "GUI_ExtData.h"
#include "stddef.h"

uint32_t * GUI_Date = NULL;
bool * GUI_DataChanged= NULL;
volatile bool * GUI_ShopListChanged = NULL;
char** GUI_NetworkList = NULL;
char GUI_ShopList[SHOPLIST_NAMES_LENGTH][SHOPLIST_NAME_SIZE];
uint32_t GUI_ShopListProducts = 0;
scanNetFun GUI_ScanNetworks = NULL;
connectNetFun GUI_ConnectNetwork = NULL;
disconnectNetFun GUI_DisconnectNetwork = NULL;
