/*
 * GUI_IntData.h
 *
 *  Created on: Jan 7, 2021
 *      Author: Owner
 */

#ifndef GUI_INTDATA_H_
#define GUI_INTDATA_H_

#include "../GUI_ExtData.h"


#define SECONDS_IN_DAY 86400

#include "stdint.h"
#include "stdbool.h"

typedef struct {
	uint8_t day;
	uint8_t month;
	uint16_t year;
	bool redTerm;
	bool yellowTerm;
} GUI_Data_ProductDetails;

typedef struct {
	const char * name;							// ex. "Milk" - name
	GUI_Data_ProductDetails product[10];		// details of each a copy
	uint32_t quantity;							// quantity
	bool toBuying;								// added to a shopping list
} GUI_Data_ProductType;

typedef struct {
	GUI_Data_ProductType products[20];
	//DATA_CATEGORY category;
	uint32_t count;				// number of products in category
} GUI_Data_ProductGroup;

extern GUI_Data_ProductGroup GUI_SortedProd[];

extern const char * GUI_GroupsNames[];

//SETTINGS
extern uint8_t GUI_TermRange;

void 	GUI_IntData_GroupData();
uint8_t GUI_IntData_GetGroups();
void 	GUI_IntData_SetProductList(GUI_Data_ProductList * list, uint32_t * length);
void 	GUI_ChangeTermRange(void);

bool	GUI_IntData_AddProdToShopList(const char *name);
bool	GUI_IntData_DelProdFromShopList(const char *name);


extern uint32_t * GUI_Date;
extern bool * GUI_DataChanged;
extern volatile bool * GUI_ShopListChanged;
extern char** GUI_NetworkList;
extern char GUI_ShopList[SHOPLIST_NAMES_LENGTH][SHOPLIST_NAME_SIZE];
extern const char * GUI_ActualSSID;
extern uint32_t GUI_ShopListProducts;
extern scanNetFun GUI_ScanNetworks;
extern connectNetFun GUI_ConnectNetwork;
extern disconnectNetFun GUI_DisconnectNetwork;
extern setBrightnessFun GUI_SetBrightness;

#endif /* GUI_INTDATA_H_ */
