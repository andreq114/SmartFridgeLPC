/*
 * GUI.c
 *
 *  Created on: Jan 7, 2021
 *      Author: Owner
 */


#include "GUI.h"
#include "GUI_ExtData.h"
#include "GUI_IntData.h"
#include "main_menu.h"
#include "settings.h"
#include "shoppinglist.h"
#include "styles.h"
#include "stddef.h"

void GUI_Init(void)
{
	STYLES_Init();
	MAIN_MENU_Init();
	SETTINGS_Init();
	SHOPLIST_Init();
}

void 	GUI_SetProductList(GUI_Data_ProductList * productlist, uint32_t * size)
{
	GUI_IntData_SetProductList(productlist, size);
}
void 	GUI_SetDate(uint32_t * date)
{
	GUI_Date = date;
}
void 	GUI_SetDataChanged(bool * checker)
{
	GUI_DataChanged = checker;
}
void 	GUI_SetNetworkList(char ** list)
{
	GUI_NetworkList = list;
}

void	GUI_SetShopListChanged(volatile bool * checker)
{
	GUI_ShopListChanged = checker;
}
/*
 * return: table of strings, '\0' defines the end
 */
char	(*GUI_GetShopList())[SHOPLIST_NAME_SIZE]
{
	return GUI_ShopList;
}

void	GUI_SetScanNetworkEvent(scanNetFun fun)
{
	GUI_ScanNetworks = fun;
}

void	GUI_SetConnectNetworkEvent(connectNetFun fun)
{
	GUI_ConnectNetwork = fun;
}

void	GUI_SetDisconnectNetworkEvent(disconnectNetFun fun)
{
	GUI_DisconnectNetwork = fun;
}
