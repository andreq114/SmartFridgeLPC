/*
 * tiles.h
 *
 *  Created on: Dec 17, 2020
 *      Author: Owner
 */

#ifndef TILES_H_
#define TILES_H_

#include "lvgl.h"
#include "styles.h"
#include "data.h"
#include "config.h"


typedef struct {
	lv_obj_t * icon;
	GUI_DATA_CATEGORIES category;
}icons;

typedef struct {
	lv_obj_t * tile;
	int8_t selected_row;		// -1 if not selected
}tiles_group;

void TILES_Init(lv_obj_t * screen, lv_obj_t * title);
void TILES_Refresh(void);

#endif /* TILES_H_ */
