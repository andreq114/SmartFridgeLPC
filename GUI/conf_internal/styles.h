/*
 * styles.h
 *
 *  Created on: Dec 17, 2020
 *      Author: Kamil Wielgosz
 */

#ifndef STYLES_H_
#define STYLES_H_

#include "lvgl.h"
#include "config.h"

LV_FONT_DECLARE(font_pl_20);
LV_FONT_DECLARE(font_pl_28);
LV_FONT_DECLARE(font_pl_12);

extern lv_style_t style_borders;
extern lv_style_t style_tab;
extern lv_style_t style_font20;
extern lv_style_t style_title;
extern lv_style_t style_redterm;
extern lv_style_t style_yellowterm;
extern lv_style_t style_greenterm;
extern lv_style_t style_timelabel;

void STYLES_Init(void);

#endif /* STYLES_H_ */
