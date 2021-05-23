
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
void	GUI_SetScanNetworkFun(scanNetFun fun);
void	GUI_SetConnectNetworkFun(connectNetFun fun);
void	GUI_SetDisconnectNetworkFun(disconnectNetFun fun);
void	GUI_SetActualSSID(const char* ssid);
void	GUI_SetBrightnessFun(setBrightnessFun fun);
void	GUI_SetDelProductFun(delProductFun fun);
#endif
