/*
 * data.c
 *
 *  Created on: Dec 2, 2020
 *      Author: Owner
 */
#include "data.h"
#include "fsl_debug_console.h"

// "" - empty string at the end is required
//char * DATA_Networks[10] = {""};		EMPTY
//char * DATA_Networks[10] = {"WIFIPZ",""};

char * DATA_Networks[30] = {""};

char (*shoplist)[SHOPLIST_NAME_SIZE];


GUI_Data_ProductList  DATA_ProdList[20];

bool DATA_DataChanged= true;
uint32_t DATA_Date = 1609286400; // ok. 21;00 29.12.2020 xDD
uint32_t DATA_Datasize = 0;

// lista zakupow tab
//
//

