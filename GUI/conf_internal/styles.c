/*
 * styles.c
 *
 *  Created on: Dec 17, 2020
 *      Author: Kamil Wielgosz
 */

#include <conf_internal/styles.h>

lv_style_t style_borders;
lv_style_t style_tab;
lv_style_t style_font20;
lv_style_t style_title;
lv_style_t style_redterm;
lv_style_t style_yellowterm;
lv_style_t style_greenterm;
lv_style_t style_timelabel;

void STYLES_Init(void){

	lv_style_init(&style_borders);

	lv_style_set_border_color(&style_borders, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xF5, 0x77, 0x14));
	lv_style_set_border_color(&style_borders, LV_STATE_FOCUSED, LV_COLOR_MAKE(0xF5, 0x77, 0x14));
	lv_style_set_bg_color(&style_borders, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xF5, 0x77, 0x14));
	lv_style_set_text_font(&style_borders, LV_STATE_DEFAULT, &font_pl_20);

	/////////////////////////////////////////////////////
	lv_style_init(&style_tab);

	lv_style_set_text_color(&style_tab, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_tab, LV_STATE_DEFAULT, &font_pl_20);

	/////////////////////////////////////////////////////
	lv_style_init(&style_font20);

	lv_style_set_text_font(&style_font20, LV_STATE_DEFAULT, &font_pl_20);

	////////////////////////////
	lv_style_init(&style_title);

	lv_style_set_text_font(&style_title, LV_STATE_DEFAULT, &font_pl_20);
	lv_style_set_text_color(&style_title, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	/////////////////////////////
	lv_style_init(&style_redterm);

	lv_style_set_text_font(&style_redterm, LV_STATE_DEFAULT, &font_pl_20);
	lv_style_set_text_color(&style_redterm, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xcc, 0x00, 0x00));
	//lv_style_set_bg_color(&style_redterm,  LV_STATE_DEFAULT, LV_COLOR_RED);

	////////////////////////////////////////////
	lv_style_init(&style_yellowterm);

	lv_style_set_text_font(&style_yellowterm, LV_STATE_DEFAULT, &font_pl_20);
	lv_style_set_text_color(&style_yellowterm, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
	//lv_style_set_bg_color(&style_yellowterm,  LV_STATE_DEFAULT, LV_COLOR_YELLOW);

	//////////////////////////////////////////////
	lv_style_init(&style_greenterm);

	lv_style_set_text_font(&style_greenterm, LV_STATE_DEFAULT, &font_pl_20);
	lv_style_set_text_color(&style_greenterm, LV_STATE_DEFAULT, LV_COLOR_GREEN);

	/////////////////////////////////////////////
	lv_style_init(&style_timelabel);

		lv_style_set_text_font(&style_timelabel, LV_STATE_DEFAULT, &font_pl_28);
		lv_style_set_text_color(&style_timelabel, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}
