/*
 * main_menu.c
 *
 *  Created on: Dec 28, 2020
 *      Author: Kamil Wielgosz
 */

#include <conf_internal/GUI_IntData.h>
#include <conf_internal/styles.h>
#include "main_menu.h"
#include "tiles.h"
#include "intro.h"
#include "settings.h"
#include "shoppinglist.h"
#include "time.h"
#include "stdio.h"


static lv_obj_t * screen;

static lv_obj_t * title;
static lv_obj_t * time_label;
static lv_obj_t * btn_settings;
static lv_obj_t * btn_shop;


// TASK
static lv_task_t * time_task;

static bool timeColonShowed = true;
static int oldMins = 0;
static int oldHours = 0;

LV_IMG_DECLARE(settings_img)
LV_IMG_DECLARE(shopcart_img)
LV_IMG_DECLARE(settings_white_img)
LV_IMG_DECLARE(shopcart_white_img)

//
// Static functions
//
static void settings_btn_event_handler(lv_obj_t * obj, lv_event_t event);
static void shoplist_btn_event_handler(lv_obj_t * obj, lv_event_t event);
static void refreshtime_task_handler(lv_task_t * task);
static void initTopToolbar(void);

//
// Global functions
//
void MAIN_MENU_Init(void){

	screen = lv_obj_create(NULL, NULL);
	lv_obj_add_style(screen, LV_STATE_DEFAULT, &style_borders);
	lv_obj_set_style_local_bg_color(screen, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	// load firs GUI_ProdList from server
	//GUI_IntData_GroupData(data, 14);
	//
	initTopToolbar();
	TILES_Init(screen, title);

	time_task = lv_task_create(refreshtime_task_handler, 500, LV_TASK_PRIO_MID, NULL);

#if SHOW_INTRO == 1
	INTRO_StartIntro();
#else
	MAIN_MENU_Show();
#endif
}

void MAIN_MENU_Show(void)
{
	lv_scr_load(screen);
}

//
// Static functions
//

static void settings_btn_event_handler(lv_obj_t * obj, lv_event_t event){
	if(event == LV_EVENT_CLICKED)
		SETTINGS_Show(screen);
}

static void shoplist_btn_event_handler(lv_obj_t * obj, lv_event_t event){
	if(event == LV_EVENT_CLICKED)
		SHOPLIST_Show(screen);
}


static void refreshtime_task_handler(lv_task_t * task){
	struct tm *time;

	time = localtime(GUI_Date);
	char buff[6];
	if(timeColonShowed){
		sprintf(buff, "%02d %02d", oldHours, oldMins);
		timeColonShowed = false;
		if(GUI_ExtinctionTimeTask < 0)
		{
			if(GUI_SetBrightness) GUI_SetBrightness(5);
		}
		else
			GUI_ExtinctionTimeTask--;
	}
	else
	{
		sprintf(buff, "%02d:%02d", time->tm_hour, time->tm_min);
		oldMins = time->tm_min;
		oldHours = time->tm_hour;
		timeColonShowed = true;
	}
	lv_label_set_text(time_label, buff);

}

static void initTopToolbar(void){
	title = lv_label_create(screen, NULL);;
	lv_label_set_long_mode(title, LV_LABEL_LONG_SROLL);
	lv_obj_set_width(title, 250);
	lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
	lv_label_set_text(title, STR_MENU_TITLE);
	lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
	lv_obj_add_style(title, LV_STATE_DEFAULT, &style_title);

	time_label = lv_label_create(screen, NULL);
	lv_obj_set_width(time_label, 150);
	lv_label_set_align(time_label, LV_LABEL_ALIGN_CENTER);
	lv_label_set_text(time_label, "empty");
	lv_obj_align(time_label, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);
	lv_obj_add_style(time_label, LV_STATE_DEFAULT, &style_timelabel);

	btn_settings = lv_imgbtn_create(screen, NULL);
	lv_obj_align(btn_settings, NULL, LV_ALIGN_IN_TOP_RIGHT, 60, 5);
	lv_obj_set_event_cb(btn_settings, settings_btn_event_handler);
	lv_imgbtn_set_src(btn_settings, LV_BTN_STATE_RELEASED , &settings_white_img);
	lv_imgbtn_set_src(btn_settings, LV_BTN_STATE_PRESSED , &settings_white_img);

	btn_shop = lv_imgbtn_create(screen, NULL);
	lv_obj_align(btn_shop, btn_settings, LV_ALIGN_OUT_LEFT_MID, 50, 0);
	lv_obj_set_event_cb(btn_shop, shoplist_btn_event_handler);
	lv_imgbtn_set_src(btn_shop, LV_BTN_STATE_RELEASED , &shopcart_white_img);
	lv_imgbtn_set_src(btn_shop, LV_BTN_STATE_PRESSED , &shopcart_white_img);

}
