/*
 * GUI_IntData.c
 *
 *  Created on: Jan 7, 2021
 *      Author: Owner
 */

#include "GUI_IntData.h"
#include "GUI_ExtData.h"
#include "config.h"
#include "strings.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "time.h"
#include "fsl_debug_console.h"

uint8_t GUI_TermRange = DEFAULT_TERM_RANGE;


static GUI_Data_ProductList * products = NULL;
static uint32_t * size = NULL;
static uint8_t GUI_Groups;

GUI_Data_ProductGroup GUI_SortedProd[NUMBER_CATEGORIES];


uint32_t * 			GUI_Date = NULL;
bool * 				GUI_DataChanged= NULL;
volatile bool * 	GUI_ShopListChanged = NULL;
char** 				GUI_NetworkList = NULL;
char 				GUI_ShopList[SHOPLIST_NAMES_LENGTH][SHOPLIST_NAME_SIZE];
const char * 		GUI_ActualSSID = NULL;
uint32_t 			GUI_ShopListProducts = 0;
scanNetFun 			GUI_ScanNetworks = NULL;
connectNetFun 		GUI_ConnectNetwork = NULL;
disconnectNetFun 	GUI_DisconnectNetwork = NULL;
setBrightnessFun 	GUI_SetBrightness = NULL;

const char * GUI_GroupsNames[] = {
		STR_TILES_NAMES_DAIRY,
		STR_TILES_NAMES_DRINKS,
		STR_TILES_NAMES_ALCOHOLS,
		STR_TILES_NAMES_SAUCES,
		STR_TILES_NAMES_SWEETS,
		STR_TILES_NAMES_FISHES,
		STR_TILES_NAMES_MEAT,
		STR_TILES_NAMES_FROZEN,
		STR_TILES_NAMES_PLANTS,
		STR_TILES_NAMES_END
};

static int hasProduct(GUI_DATA_CATEGORIES cat, const char * str){

	for(int j = 0; j < GUI_SortedProd[cat].count; j++)
	{
		if(!strcmp(str, GUI_SortedProd[cat].products[j].name))
		{
			return j;
		}
	}
	return -1;
}

static void addTerm(GUI_DATA_CATEGORIES cat, uint32_t index, GUI_Data_ProductDetails * details){
	GUI_SortedProd[cat].products[index].product[GUI_SortedProd[cat].products[index].quantity].day = details->day;
	GUI_SortedProd[cat].products[index].product[GUI_SortedProd[cat].products[index].quantity].month = details->month;
	GUI_SortedProd[cat].products[index].product[GUI_SortedProd[cat].products[index].quantity].redTerm = details->redTerm;
	GUI_SortedProd[cat].products[index].product[GUI_SortedProd[cat].products[index].quantity].year = details->year;
	GUI_SortedProd[cat].products[index].product[GUI_SortedProd[cat].products[index].quantity].yellowTerm = details->yellowTerm;

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


	if(GUI_Date == NULL)
	{
		term.redTerm = false;
		term.yellowTerm = false;
	}
	else
	{
		struct tm time;
		time.tm_mday = term.day;
		time.tm_mon = term.month - 1;
		time.tm_year = term.year - 1900;
		time.tm_sec = 0;
		time.tm_min = 0;
		time.tm_hour = 0;
		int product_time = mktime(&time);
		int different = product_time - *GUI_Date;

		term.redTerm = false;
		term.yellowTerm = false;

		if(different < 0)
		{
			term.redTerm = true;
		}
		else if(different / SECONDS_IN_DAY < GUI_TermRange)
		{
			term.yellowTerm = true;
		}
	}
	return term;
}

static void addProduct(GUI_DATA_CATEGORIES cat, GUI_Data_ProductDetails * details, GUI_Data_ProductList * product)
{
	int index = hasProduct(cat, product->name);
	if( 0 <= index )
	{
		addTerm(cat, index, details);
	}
	else
	{
		uint32_t amount_products = GUI_SortedProd[cat].count;
		GUI_SortedProd[cat].products[amount_products].name = product->name;
		GUI_SortedProd[cat].products[amount_products].quantity = 0;
		addTerm(cat, amount_products, details);
		GUI_SortedProd[cat].count++;
	}

}

static void groupProduct(GUI_Data_ProductList * product)
{
	GUI_DATA_CATEGORIES category = product->category;
	GUI_Data_ProductDetails terms = calcDate(product);

	addProduct(category, &terms, product);

	if(terms.redTerm || terms.yellowTerm)
	{
		addProduct(DATA_END_OF_EXPIRY, &terms, product);
	}
}

static void cleanSortedProd(){
	memset(GUI_SortedProd, 0, sizeof(GUI_Data_ProductGroup) * NUMBER_CATEGORIES);
	GUI_Groups = 0;
}

void GUI_IntData_GroupData(void){
	if(size == NULL || products == NULL) return;

	cleanSortedProd();
	for(int i = 0; i < *size; i++){
		groupProduct(&products[i]);
	}
	for(int j = 0; j < NUMBER_CATEGORIES; j++)
	{
		if(GUI_SortedProd[j].count > 0){
			GUI_Groups++;
		}
	}
}

void GUI_ChangeTermRange(void)
{
	void* ptr = &GUI_SortedProd[DATA_END_OF_EXPIRY];
	memset(ptr, 0, sizeof(GUI_Data_ProductGroup));
	for(int i = 0; i < *size; i++)
	{
		GUI_Data_ProductDetails terms = calcDate(&products[i]);

		if(terms.redTerm || terms.yellowTerm)
		{
			addProduct(DATA_END_OF_EXPIRY, &terms, &products[i]);
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


bool    GUI_IntData_AddProdToShopList(const char *name)
{
	int i = 0;
	for(i; GUI_ShopList[i][0] != '\0' ; ) i++;

	if((i + 1) == SHOPLIST_NAMES_LENGTH)
		return false;

	snprintf(GUI_ShopList[i], SHOPLIST_NAME_SIZE, "%s", name);
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
	for(j; GUI_ShopList[j][0] != '\0' ; ) j++;

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
		GUI_ShopList[i][SHOPLIST_NAME_SIZE - 1] = '\0';
		GUI_ShopList[i + 1][0] = '\0';
	}
	if(GUI_ShopListChanged){
		*GUI_ShopListChanged = true;
	}
	return true;
}
