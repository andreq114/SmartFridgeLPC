/*
 * shoppinglist.c
 *
 *  Created on: Jan 1, 2021
 *      Author: Owner
 */

#include "shoppinglist.h"
#include "data.h"
#include "styles.h"
#include "GUI_ExtData.h"
#include "GUI_IntData.h"
#include "fsl_debug_console.h"

static lv_obj_t * screen;
static lv_obj_t * prev_screen;

static lv_obj_t * main_page;
static lv_obj_t * label_title;
static lv_obj_t * btn_img_return;
static lv_obj_t * btn_img_add_prod;
static lv_obj_t * list;
static lv_obj_t * list_btn;

static lv_obj_t * kb_bkground;
static lv_obj_t * textarea;

LV_IMG_DECLARE(previous);
LV_IMG_DECLARE(add);

static void list_btn_evt_handler(lv_obj_t * obj, lv_event_t event);

static void btn_img_return_evt_handler(lv_obj_t * obj, lv_event_t event){
	if(event == LV_EVENT_CLICKED){
		lv_scr_load(prev_screen);
	}
}

static void keyboard_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	lv_keyboard_def_event_cb(obj, event);
	if(event == LV_EVENT_CANCEL)
	{
		lv_obj_del(kb_bkground);
		kb_bkground = NULL;
		textarea = NULL;
	}
	else if(event == LV_EVENT_APPLY)
	{
		const char * inputText = lv_textarea_get_text(textarea);
		if(inputText[0] == '\0' || inputText == NULL)
		{
			lv_obj_del(kb_bkground);
			kb_bkground = NULL;
			textarea = NULL;
			return;
		}

		if(GUI_IntData_AddProdToShopList(inputText))
		{
			list_btn = lv_list_add_btn(list, NULL, inputText);
			lv_obj_set_event_cb(list_btn,list_btn_evt_handler);
			lv_obj_add_style(list_btn, LV_BTN_PART_MAIN, &style_font20);
			lv_obj_set_style_local_bg_color(list_btn, 0, LV_BTN_PART_MAIN, LV_COLOR_MAKE(0xF5, 0x77, 0x14));
		}
		lv_obj_del(kb_bkground);
		kb_bkground = NULL;
		textarea = NULL;
	}
}

static void btn_img_add_prod_evt_handler(lv_obj_t * obj, lv_event_t event){
	if(event == LV_EVENT_CLICKED)
	{
		kb_bkground = lv_obj_create(screen ,NULL);
		lv_obj_reset_style_list(kb_bkground, LV_OBJ_PART_MAIN);
		lv_obj_set_style_local_bg_color(kb_bkground, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
		lv_obj_set_pos(kb_bkground, 0, 0);
		lv_obj_set_size(kb_bkground, LV_HOR_RES, LV_VER_RES);
		lv_obj_set_style_local_bg_opa(kb_bkground, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);

		textarea = lv_textarea_create(kb_bkground, NULL);
		lv_obj_set_size(textarea, 250, 80);
		lv_obj_align(textarea, NULL, LV_ALIGN_IN_TOP_MID, 0, 40);
		lv_textarea_set_max_length(textarea, 32);
		lv_textarea_set_text(textarea, "");

		lv_obj_t * kb = lv_keyboard_create(kb_bkground, NULL);
		lv_keyboard_set_cursor_manage(kb, true);
		lv_obj_set_event_cb(kb, keyboard_evt_handler);
		lv_keyboard_set_textarea(kb, textarea);
	}
}

static void list_btn_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	if(event == LV_EVENT_LONG_PRESSED)
	{
		const char * product = lv_list_get_btn_text(obj);
		if(GUI_IntData_DelProdFromShopList(product))
		{
			lv_list_remove(list, lv_list_get_btn_index(list, obj));
		}
	}
}
static void setList(void){
	lv_list_clean(list);
	for(int i = 0; GUI_ShopList[i][0] != '\0' ; i++)
	{
		list_btn = lv_list_add_btn(list, NULL, GUI_ShopList[i]);
		lv_obj_set_event_cb(list_btn, list_btn_evt_handler);
		lv_obj_add_style(list_btn, LV_BTN_PART_MAIN, &style_font20);
		lv_obj_set_style_local_bg_color(list_btn, 0, LV_BTN_PART_MAIN, LV_COLOR_MAKE(0xF5, 0x77, 0x14));
	}
}

void SHOPLIST_Init(void){

	screen = lv_obj_create(NULL, NULL);
	lv_obj_add_style(screen, LV_STATE_DEFAULT, &style_borders);
	lv_obj_set_style_local_bg_color(screen, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	label_title = lv_label_create(screen, NULL);
	lv_label_set_text(label_title, STR_SHOPLIST_TITLE);
	lv_obj_set_auto_realign(label_title, true);
	lv_obj_align(label_title, screen, LV_ALIGN_IN_TOP_MID, 0, 10);
	lv_obj_add_style(label_title, LV_STATE_DEFAULT, &style_title);

	btn_img_return = lv_imgbtn_create(screen, NULL);
	lv_obj_align(btn_img_return, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);
	lv_obj_set_event_cb(btn_img_return, btn_img_return_evt_handler);
	lv_imgbtn_set_src(btn_img_return, LV_BTN_STATE_RELEASED , &previous);
	lv_imgbtn_set_src(btn_img_return, LV_BTN_STATE_PRESSED , &previous);

	btn_img_add_prod = lv_imgbtn_create(screen, btn_img_return);
	lv_obj_align(btn_img_add_prod, NULL, LV_ALIGN_IN_TOP_RIGHT, -5, 5);
	lv_obj_set_event_cb(btn_img_add_prod, btn_img_add_prod_evt_handler);
	lv_imgbtn_set_src(btn_img_add_prod, LV_BTN_STATE_RELEASED , &add);
	lv_imgbtn_set_src(btn_img_add_prod, LV_BTN_STATE_PRESSED , &add);


	main_page =  lv_page_create(screen, NULL);
	lv_obj_set_pos(main_page, 0, 40);
	lv_obj_set_size(main_page, LV_HOR_RES, LV_VER_RES - 40);
	lv_obj_add_style(main_page, LV_PAGE_PART_BG, &style_borders);

	list = lv_list_create(main_page, NULL);
	lv_obj_align(list, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_set_size(list, LV_HOR_RES - 20, LV_VER_RES - 60);
	lv_list_set_edge_flash(list, true);
	lv_page_set_scroll_propagation(list, true);
	lv_page_set_scrlbar_mode(list, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(list, LV_BTN_PART_MAIN, &style_borders);

	GUI_ShopList[0][0] = '\0';
}

void SHOPLIST_Show(lv_obj_t * previous_screen){
	prev_screen = previous_screen;
	setList();
	lv_scr_load(screen);
}
