/*
 * GUI_IntData.c
 *
 *  Created on: Jan 7, 2021
 *      Author: Owner
 */

#include "GUI_IntData.h"
#include "GUI_ExtData.h"
#include "strings.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "fsl_debug_console.h"

uint8_t GUI_RedTerm = 2;
uint8_t GUI_YellowTerm = 7;


static GUI_Data_ProductList * products = NULL;
static uint32_t * size = NULL;
static uint8_t GUI_Groups;

GUI_Data_ProductGroup GUI_SortedProd[9];

const char * GUI_GroupsNames[] = {
		STR_TILES_NAMES_DAIRY,
		STR_TILES_NAMES_DRINKS,
		STR_TILES_NAMES_ALCOHOLS,
		STR_TILES_NAMES_SAUCES,
		STR_TILES_NAMES_SWEETS,
		STR_TILES_NAMES_FISHES,
		STR_TILES_NAMES_MEAT,
		STR_TILES_NAMES_FROZEN,
		STR_TILES_NAMES_PLANTS
};

static int hasProduct(GUI_DATA_CATEGORIES cat, const char * str){
	for(int j = 0; j < 30; j++)
	{
		if(!strcmp(str, GUI_SortedProd[cat].products[j].name))
		{
			return j;
		}
	}
	return -1;
}

static inline void addTerm(GUI_DATA_CATEGORIES cat, uint32_t index, GUI_Data_ProductDetails details){
	GUI_SortedProd[cat].products[index].product[GUI_SortedProd[cat].products[index].quantity] = details;
	GUI_SortedProd[cat].products[index].quantity++;
}

static GUI_Data_ProductDetails calcDate(GUI_Data_ProductList * product){
	GUI_Data_ProductDetails term;
	char * date_ptr = product->date;

	term.day = atoi(product->date);
	while(*(date_ptr)++ != '.');
	term.month = atoi(date_ptr);
	while(*(date_ptr)++ != '.');
	term.year = atoi(date_ptr);

	return term;
}

static void addProduct(GUI_Data_ProductList * product)
{
	GUI_DATA_CATEGORIES category = product->category;
	int index = hasProduct(category, product->name);
	GUI_Data_ProductDetails term = calcDate(product);;

	if( 0 <= index )
	{
		addTerm(category, index, term);
	}
	else
	{
		uint32_t amount_products = GUI_SortedProd[category].count;
		GUI_SortedProd[category].products[amount_products].name = product->name;
		GUI_SortedProd[category].products[amount_products].quantity = 0;
		addTerm(category, amount_products, term);
		GUI_SortedProd[category].count++;
	}
}

static void cleanSortedProd(){
	memset(GUI_SortedProd, 0, sizeof(GUI_Data_ProductGroup) * 9);
	GUI_Groups = 0;
}

void GUI_IntData_GroupData(void){
	if(size == NULL || products == NULL) return;

	cleanSortedProd();
	for(int i = 0; i < *size; i++){
		addProduct(&products[i]);
	}
	for(int j = 0; j < 9; j++)
	{
		if(GUI_SortedProd[j].count > 0){
			GUI_Groups++;
		}
	}
}

uint8_t GUI_IntData_GetGroups(){
	return GUI_Groups;
}

void GUI_IntData_SetProductList(GUI_Data_ProductList * list, uint32_t * length)
{
	products = list;
	size = length;
}

//bool	GUI_IntData_AddProdToShopList(const char *name)
//{
//	if((GUI_ShopListProducts + 1) == SHOPLIST_NAMES_LENGTH)
//		return false;
//
//	snprintf(GUI_ShopList[GUI_ShopListProducts], SHOPLIST_NAME_SIZE, "%s", name);
//	PRINTF("Dod:%s\r\n", GUI_ShopList[GUI_ShopListProducts]);
//	GUI_ShopList[GUI_ShopListProducts][SHOPLIST_NAME_SIZE - 1] = '\0';
//	GUI_ShopList[GUI_ShopListProducts + 1][0] = '\0';
//	GUI_ShopListProducts++;
//	if(GUI_ShopListChanged){
//		*GUI_ShopListChanged = true;
//	}
//	return true;
//}
//
//bool	GUI_IntData_DelProdFromShopList(const char *name)
//{
//	if(GUI_ShopListProducts == 0)
//		return false;
//
//	int i = 0;
//	for(; i < GUI_ShopListProducts ; i++)
//	{
//		if(GUI_ShopList[i][0] != '\0')
//		{
//			if(!strcmp(name, GUI_ShopList[i]))
//			{
//				break;
//			}
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//	for(; i < GUI_ShopListProducts ; i++)
//	{
//		snprintf(GUI_ShopList[i], SHOPLIST_NAME_SIZE - 1, "%s", GUI_ShopList[i + 1]);
//		PRINTF("Przes:%s\r\n", GUI_ShopList[i]);
//		GUI_ShopList[i][SHOPLIST_NAME_SIZE - 1] = '\0';
//		GUI_ShopList[i + 1][0] = '\0';
//	}
//	GUI_ShopListProducts--;
//	if(GUI_ShopListChanged){
//		*GUI_ShopListChanged = true;
//	}
//	return true;
//}

bool    GUI_IntData_AddProdToShopList(const char *name)
{
    int i = 0;
    for(i; GUI_ShopList[i][0] != '\0' ; i++);

    if((i + 1) == SHOPLIST_NAMES_LENGTH)
        return false;

    snprintf(GUI_ShopList[i], SHOPLIST_NAME_SIZE, "%s", name);
    PRINTF("Dod:%s\r\n", GUI_ShopList[i]);
    GUI_ShopList[i][SHOPLIST_NAME_SIZE - 1] = '\0';
    GUI_ShopList[i + 1][0] = '\0';
   	if(GUI_ShopListChanged){
    		*GUI_ShopListChanged = true;
    	}
    return true;
}

bool    GUI_IntData_DelProdFromShopList(const char *name)
{
    int j = 0;
    for(j; GUI_ShopList[j][0] != '\0' ; j++);

    if((j + 1) == SHOPLIST_NAMES_LENGTH)
        return false;

    int i = 0;
    for(; i < j ; i++)
    {
            if(!strcmp(name, GUI_ShopList[i]))
            {
                break;
            }
    }

    if(i == j)
    {
        return false;
    }

    for(; i < j ; i++)
    {
        snprintf(GUI_ShopList[i], SHOPLIST_NAME_SIZE - 1, "%s", GUI_ShopList[i + 1]);
        PRINTF("Przes:%s\r\n", GUI_ShopList[i]);
        GUI_ShopList[i][SHOPLIST_NAME_SIZE - 1] = '\0';
        GUI_ShopList[i + 1][0] = '\0';
    }
    	if(GUI_ShopListChanged){
    		*GUI_ShopListChanged = true;
    	}
    return true;
}
