/*
 * GUI.c
 *
 *  Created on: Jan 7, 2021
 *      Author: Kamil Wielgosz
 */


#include <conf_internal/GUI_IntData.h>
#include <conf_internal/styles.h>
#include "GUI.h"
#include "GUI_ExtData.h"
#include "conf_internal/GUI_IntData.h"
#include "main_menu.h"
#include "settings.h"
#include "shoppinglist.h"
#include "tiles.h"
#include "stddef.h"

void GUI_Init(void)
{
	INTERNALDATA_INIT();
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
 * return: table of strings, '\0' defines the end if the list is not full
 */
char	(*GUI_GetShopList())[SHOPLIST_NAME_SIZE]
{
	return GUI_ShopList;
}

void	GUI_SetScanNetworkFun(scanNetFun fun)
{
	GUI_ScanNetworks = fun;
}

void	GUI_SetConnectNetworkFun(connectNetFun fun)
{
	GUI_ConnectNetwork = fun;
}

void	GUI_SetDisconnectNetworkFun(disconnectNetFun fun)
{
	GUI_DisconnectNetwork = fun;
}

void	GUI_SetActualSSID(const char* ssid)
{
	GUI_ActualSSID = ssid;
}

void	GUI_SetBrightnessFun(setBrightnessFun fun)
{
	GUI_SetBrightness = fun;
}

void	GUI_SetDelProductFun(delProductFun fun)
{
	GUI_DeleteProduct = fun;
}
