/*
 * data.h
 *
 *  Created on: Nov 30, 2020
 *      Author: Owner
 */

#ifndef DATA_H_
#define DATA_H_

#include "stdint.h"
#include "stdbool.h"
#include "GUI_ExtData.h"


extern char * DATA_Networks[];

extern GUI_Data_ProductList  DATA_ProdList[25];

extern bool DATA_DataChanged;
extern uint32_t DATA_Date; // ok. 21;00 29.12.2020 xDD
extern uint32_t DATA_Datasize;
extern char (*shoplist)[SHOPLIST_NAME_SIZE];


#endif /* DATA_H_ */
