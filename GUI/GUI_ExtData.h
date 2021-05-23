 /*
 * GUI_ExtData.h
 *
 *  Created on: Jan 7, 2021
 *      Author: Kamil Wielgosz
 */

#ifndef GUI_EXTDATA_H_
#define GUI_EXTDATA_H_

#include "stdint.h"
#include "stdbool.h"

#define SHOPLIST_NAMES_LENGTH	10
#define SHOPLIST_NAME_SIZE 		50
#define NUMBER_CATEGORIES		10

typedef void( * scanNetFun )();
typedef bool( * connectNetFun )(const char*, const char*);
typedef void( * disconnectNetFun )();
typedef void( * setBrightnessFun )(uint8_t );
typedef void( * delProductFun )(uint8_t *);

enum {
    DATA_DAIRY,
	DATA_DRINKS,
	DATA_ALCOHOLS,
	DATA_SAUCES,
	DATA_SWEETS,
	DATA_FISHES,
	DATA_MEAT,
	DATA_FROZEN,
	DATA_PLANTS,
	DATA_END_OF_EXPIRY
};
typedef uint8_t GUI_DATA_CATEGORIES;

typedef struct {
	uint8_t uid[10];
	char name[49];
	char date[11];
	GUI_DATA_CATEGORIES category;
} GUI_Data_ProductList;


#endif /* GUI_EXTDATA_H_ */
