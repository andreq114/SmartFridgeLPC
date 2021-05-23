/*
 * shoppinglist.c
 *
 *  Created on: Jan 1, 2021
 *      Author: Kamil Wielgosz
 */

#include <conf_internal/GUI_IntData.h>
#include <conf_internal/styles.h>
#include "shoppinglist.h"
#include "GUI_ExtData.h"

static lv_obj_t * screen;
static lv_obj_t * prev_screen;

static lv_obj_t * main_page;
static lv_obj_t * label_title;
static lv_obj_t * btn_img_return;
static lv_obj_t * btn_img_add_prod;
static lv_obj_t * list;
static lv_obj_t * list_btn;

static lv_obj_t * kb_bkground;
static lv_obj_t * listMsgBox;
static const char * labelsMsgBox[] = {STR_SHOPLIST_CONFIRM_MSG, ""};
static lv_obj_t * textarea;

LV_IMG_DECLARE(previous);
LV_IMG_DECLARE(add);

//
// Event functions
//
static void screen_evt_handler(lv_obj_t * obj, lv_event_t event);
static void list_btn_evt_handler(lv_obj_t * obj, lv_event_t event);
static void btn_img_return_evt_handler(lv_obj_t * obj, lv_event_t event);
static void msgBox_event_handler(lv_obj_t * obj, lv_event_t event);
static void keyboard_evt_handler(lv_obj_t * obj, lv_event_t event);
static void btn_img_add_prod_evt_handler(lv_obj_t * obj, lv_event_t event);

//
// Helper functions
//
static void setList(void);

//
// Init functions
//

void SHOPLIST_Init(void){

	screen = lv_obj_create(NULL, NULL);
	lv_obj_add_style(screen, LV_STATE_DEFAULT, &style_borders);
	lv_obj_set_style_local_bg_color(screen, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_event_cb(screen, screen_evt_handler);

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
	lv_obj_set_event_cb(main_page, screen_evt_handler);


	list = lv_list_create(main_page, NULL);
	lv_obj_align(list, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_set_size(list, LV_HOR_RES - 20, LV_VER_RES - 60);
	lv_list_set_edge_flash(list, true);
	lv_page_set_scroll_propagation(list, true);
	lv_page_set_scrlbar_mode(list, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(list, LV_BTN_PART_MAIN, &style_borders);
	lv_obj_set_event_cb(list, screen_evt_handler);

	kb_bkground = lv_obj_create(screen ,NULL);
	lv_obj_reset_style_list(kb_bkground, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_color(kb_bkground, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(kb_bkground, 0, 0);
	lv_obj_set_size(kb_bkground, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_style_local_bg_opa(kb_bkground, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_80);
	lv_obj_set_hidden(kb_bkground, true);

	GUI_ShopList[0][0] = '\0';
}

void SHOPLIST_Show(lv_obj_t * previous_screen){
	prev_screen = previous_screen;
	setList();
	lv_scr_load(screen);
}

//
// Static functions
//

static void screen_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();
}


static void btn_img_return_evt_handler(lv_obj_t * obj, lv_event_t event){
	if(event == LV_EVENT_CLICKED){
		lv_scr_load(prev_screen);
	}
}

static void msgBox_event_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_DELETE && obj == listMsgBox)
	{
		lv_obj_set_hidden(kb_bkground, true);
		listMsgBox = NULL;
	}
	else if(event == LV_EVENT_VALUE_CHANGED)
	{
		lv_msgbox_start_auto_close(obj, 0);
		lv_obj_set_hidden(kb_bkground, true);
	}
}

static void keyboard_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();

	lv_keyboard_def_event_cb(obj, event);
	if(event == LV_EVENT_CANCEL)
	{
		lv_obj_clean(kb_bkground);
		textarea = NULL;
		lv_obj_set_hidden(kb_bkground, true);
	}
	else if(event == LV_EVENT_APPLY)
	{
		const char * inputText = lv_textarea_get_text(textarea);
		if(inputText[0] == '\0' || inputText == NULL)
		{
			lv_obj_clean(kb_bkground);
			lv_obj_set_hidden(kb_bkground, true);
			textarea = NULL;
			return;
		}

		if(GUI_IntData_AddProdToShopList(inputText))
		{
			list_btn = lv_list_add_btn(list, NULL, inputText);
			lv_obj_set_event_cb(list_btn,list_btn_evt_handler);
			lv_obj_add_style(list_btn, LV_BTN_PART_MAIN, &style_font20);
			lv_obj_set_style_local_bg_color(list_btn, 0, LV_BTN_PART_MAIN, LV_COLOR_MAKE(0xF5, 0x77, 0x14));
			lv_obj_clean(kb_bkground);
			lv_obj_set_hidden(kb_bkground, true);
			textarea = NULL;
		}
	}
}

static void btn_img_add_prod_evt_handler(lv_obj_t * obj, lv_event_t event){
	if(event == LV_EVENT_CLICKED)
	{
		lv_obj_set_hidden(kb_bkground, false);
		if(isShopListFull())
		{

			listMsgBox = lv_msgbox_create(kb_bkground, NULL);
			lv_msgbox_add_btns(listMsgBox, labelsMsgBox);
			lv_msgbox_set_text(listMsgBox, STR_SHOPLIST_LIST_FULL);
			lv_obj_align(listMsgBox, NULL, LV_ALIGN_CENTER, 0, 0);
			lv_obj_set_event_cb(listMsgBox, msgBox_event_handler);
			lv_obj_add_style(listMsgBox, LV_MSGBOX_PART_BG, &style_borders);
			lv_obj_add_style(listMsgBox, LV_MSGBOX_PART_BTN_BG, &style_font20);
			return;
		}

		textarea = lv_textarea_create(kb_bkground, NULL);
		lv_obj_set_size(textarea, 250, 80);
		lv_obj_align(textarea, NULL, LV_ALIGN_IN_TOP_MID, 0, 40);
		lv_textarea_set_max_length(textarea, 32);
		lv_textarea_set_text(textarea, "");
		lv_obj_add_style(textarea, LV_PAGE_PART_BG, &style_borders);

		lv_obj_t * label = lv_label_create(kb_bkground, NULL);
		lv_label_set_text(label, STR_SHOPLIST_PUTNAME);
		lv_obj_set_auto_realign(label, true);
		lv_obj_align(label, textarea, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
		lv_obj_add_style(label, LV_STATE_DEFAULT, &style_title);

		lv_obj_t * kb = lv_keyboard_create(kb_bkground, NULL);
		lv_keyboard_set_cursor_manage(kb, true);
		lv_obj_set_event_cb(kb, keyboard_evt_handler);
		lv_keyboard_set_textarea(kb, textarea);
		lv_obj_add_style(kb, LV_KEYBOARD_PART_BG, &style_borders);
	}
}

static void list_btn_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();

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
	for(int i = 0; i < SHOPLIST_NAMES_LENGTH && GUI_ShopList[i][0] != '\0' ; i++)
	{
		list_btn = lv_list_add_btn(list, NULL, GUI_ShopList[i]);
		lv_obj_set_event_cb(list_btn, list_btn_evt_handler);
		lv_obj_add_style(list_btn, LV_BTN_PART_MAIN, &style_font20);
		lv_obj_set_style_local_bg_color(list_btn, 0, LV_BTN_PART_MAIN, LV_COLOR_MAKE(0xF5, 0x77, 0x14));
	}
}
