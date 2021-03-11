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


GUI_Data_ProductList  DATA_ProdList[20] = {{1, "Mleko 1l", "22.01.2021", 0},
		{2, "Pepsi", "11.11.2022", 1},
		{3, "Fanta", "22.02.2022", 1},
		{4, "Piwo", "15.01.2021", 2},
		{6, "Piwo", "11.12.2021", 2},
		{5, "Keczup", "31.06.2021", 3},
		{7, "Czekolada", "11.11.2022", 4},
		{8, "Czekolada", "02.10.2022", 4},
		{8, "Baton", "13.10.2022", 4},
		{8, "Łosoś", "29.12.2020", 5},
		{8, "Mięso", "15.11.2022", 6},
		{11, "Lody śmietankowe", "12.03.2023", 7},
		{12, "Frytki mrożone", "31.12.2020", 7},
		{13, "Truskawki pakowane", "01.01.2021", 8}};

bool DATA_DataChanged= true;
uint32_t DATA_Date = 1609286400; // ok. 21;00 29.12.2020 xDD
uint32_t DATA_Datasize = 13;

// lista zakupow tab
//
//

