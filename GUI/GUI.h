
#ifndef GUI_H_
#define GUI_H_

#include "GUI_ExtData.h"


void 	GUI_Init(void);
void 	GUI_SetProductList(GUI_Data_ProductList * productlist, uint32_t * size);
void 	GUI_SetDate(uint32_t * date);
void 	GUI_SetDataChanged(bool * checker);
void 	GUI_SetNetworkList(char* * list);
void	GUI_SetShopListChanged(volatile bool * checker);
char  	(*GUI_GetShopList(void))[SHOPLIST_NAME_SIZE];
void	GUI_SetScanNetworkEvent(scanNetFun fun);
void	GUI_SetConnectNetworkEvent(connectNetFun fun);
void	GUI_SetDisconnectNetworkEvent(disconnectNetFun fun);
#endif
