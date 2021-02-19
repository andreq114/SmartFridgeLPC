/*
 * GUI_IntData.h
 *
 *  Created on: Jan 7, 2021
 *      Author: Owner
 */

#ifndef GUI_INTDATA_H_
#define GUI_INTDATA_H_

#include "../GUI_ExtData.h"

#include "stdint.h"
#include "stdbool.h"

typedef struct {
	uint8_t day;
	uint8_t month;
	uint16_t year;
} GUI_Data_ProductDetails;

typedef struct {
	const char * name;							// ex. "Milk" - name
	GUI_Data_ProductDetails product[30];		// details of each a copy
	uint32_t quantity;							// quantity
	bool toBuying;								// added to a shopping list
} GUI_Data_ProductType;

typedef struct {
	GUI_Data_ProductType products[30];
	//DATA_CATEGORY category;
	uint32_t count;				// number of products in category
} GUI_Data_ProductGroup;

extern GUI_Data_ProductGroup GUI_SortedProd[];

extern const char * GUI_GroupsNames[];

//SETTINGS
extern uint8_t GUI_RedTerm;
extern uint8_t GUI_YellowTerm;

void 	GUI_IntData_GroupData();
uint8_t GUI_IntData_GetGroups();
void 	GUI_IntData_SetProductList(GUI_Data_ProductList * list, uint32_t * length);

bool	GUI_IntData_AddProdToShopList(const char *name);
bool	GUI_IntData_DelProdFromShopList(const char *name);

#endif /* GUI_INTDATA_H_ */
