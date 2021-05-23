/*
 * settings.c
 *
 *  Created on: Dec 28, 2020
 *      Author: Kamil Wielgosz
 */
 
#include <conf_internal/GUI_IntData.h>
#include <conf_internal/styles.h>
#include "settings.h"
#include "stdio.h"
#include "GUI_ExtData.h"

static lv_obj_t * screen;
static lv_obj_t * prev_screen;

static lv_obj_t * main_page;
static lv_obj_t * label_display;
static lv_obj_t * label_brightness;
static lv_obj_t * slider_brightness;
static lv_obj_t * label_slider_value;
static lv_obj_t * label_title;
static lv_obj_t * label_status;
static lv_obj_t * label_redterm;
static lv_obj_t * btn_img_return;

static lv_obj_t * spinbox_red;
static lv_obj_t * btnred_plus;
static lv_obj_t * btnred_minus;

static lv_obj_t * network_label;
static lv_obj_t * network_currentNet_label;
static lv_obj_t * network_list;
static lv_obj_t * network_list_btn;
static lv_obj_t * network_refresh_btn;
static lv_obj_t * network_disconnect_btn;

static lv_obj_t * safemode_label;
static lv_obj_t * safemode_switch;

static lv_obj_t * listMsgBox;
static const char * labelsMsgBox[] = {STR_SHOPLIST_CONFIRM_MSG, ""};

static lv_obj_t * kb_bkground;
static lv_obj_t * textarea;

static lv_obj_t * choosedSSID = NULL;
static lv_obj_t * oldSSID = NULL;


static lv_task_t * task;

static uint8_t termRange;

LV_IMG_DECLARE(previous)
LV_IMG_DECLARE(refresh)
LV_IMG_DECLARE(disconnect)

//
// Event functions
//
static void slider_brightness_evt_handler(lv_obj_t * obj, lv_event_t event);
static void btn_img_return_evt_handler(lv_obj_t * obj, lv_event_t event);
static void screen_evt_handler(lv_obj_t * obj, lv_event_t event);
static void spinbox_plus_evt_handler(lv_obj_t * obj, lv_event_t event);
static void btn_refresh_evt_handler(lv_obj_t * obj, lv_event_t event);
static void btn_disconnect_evt_handler(lv_obj_t * obj, lv_event_t event);
static void msgBox_event_handler(lv_obj_t * obj, lv_event_t event);
static void keyboard_evt_handler(lv_obj_t * obj, lv_event_t event);
static void list_btn_evt_handler(lv_obj_t * obj, lv_event_t event);
static void switch_evt_handler(lv_obj_t * obj, lv_event_t event);
//
// Helper functions
//
static void initDisplay(void);
static void initLabels(void);
static void initNetwork(void);
static void initSafeMode(void);
static void setNetworks(void);
static void connectNatwork_task(lv_task_t* task);

//
// Init functions
//

lv_obj_t * SETTINGS_Init(void){

	screen = lv_obj_create(NULL, NULL);
	lv_obj_add_style(screen, LV_STATE_DEFAULT, &style_borders);
	lv_obj_set_style_local_bg_color(screen, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	label_title = lv_label_create(screen, NULL);
	lv_label_set_text(label_title, STR_SETTINGS_TITLE);
	lv_obj_set_auto_realign(label_title, true);
	lv_obj_align(label_title, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
	lv_obj_add_style(label_title, LV_STATE_DEFAULT, &style_title);

	btn_img_return = lv_imgbtn_create(screen, NULL);
	lv_obj_align(btn_img_return, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);
	lv_obj_set_event_cb(btn_img_return, btn_img_return_evt_handler);
	lv_imgbtn_set_src(btn_img_return, LV_BTN_STATE_RELEASED , &previous);
	lv_imgbtn_set_src(btn_img_return, LV_BTN_STATE_PRESSED , &previous);

	main_page =  lv_page_create(screen, NULL);
	lv_obj_set_pos(main_page, 0, 40);
	lv_obj_set_size(main_page, LV_HOR_RES, LV_VER_RES - 40);
	lv_obj_add_style(main_page, LV_PAGE_PART_BG, &style_borders);
	lv_obj_set_event_cb(main_page, screen_evt_handler);


	kb_bkground = lv_obj_create(screen ,NULL);
	lv_obj_reset_style_list(kb_bkground, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_color(kb_bkground, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(kb_bkground, 0, 0);
	lv_obj_set_size(kb_bkground, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_style_local_bg_opa(kb_bkground, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);

	lv_obj_set_hidden(kb_bkground, true);


	initDisplay();
	initLabels();
	initNetwork();
	initSafeMode();

	termRange = GUI_TermRange;

	return screen;
}

void SETTINGS_Show(lv_obj_t * previous_screen){
	prev_screen = previous_screen;
	setNetworks();
	lv_scr_load(screen);
}

//
// Static functions
//

static void screen_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();
}

static void slider_brightness_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();
	if(event == LV_EVENT_VALUE_CHANGED){
		static char buf[5];
		int16_t brightness = lv_slider_get_value(obj);
		GUI_Brightness = brightness;
		snprintf(buf, 5, "%d", brightness);
		lv_label_set_text(label_slider_value, buf);
		if(GUI_SetBrightness != NULL)
			GUI_SetBrightness(brightness);
	}
}

static void btn_img_return_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_CLICKED){

		if(termRange != GUI_TermRange && GUI_DataChanged != NULL)
		{
			GUI_ChangeTermRange();
			*GUI_DataChanged = true;
			termRange = GUI_TermRange;
		}
		lv_scr_load(prev_screen);
	}
}

static void spinbox_plus_evt_handler(lv_obj_t * obj, lv_event_t event){

	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
	{
		lv_spinbox_increment(spinbox_red);
		GUI_TermRange = lv_spinbox_get_value(spinbox_red);
	}
}

static void spinbox_minus_event_cb(lv_obj_t * btn, lv_event_t event){

	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT){
		lv_spinbox_decrement(spinbox_red);
		GUI_TermRange = lv_spinbox_get_value(spinbox_red);
	}
}

static void btn_refresh_evt_handler(lv_obj_t * obj, lv_event_t event){
	if(event == LV_EVENT_CLICKED){
		if(GUI_ScanNetworks)
		{
			GUI_ScanNetworks();
			setNetworks();
		}	
	}
}

static void btn_disconnect_evt_handler(lv_obj_t * obj, lv_event_t event){
	if(event == LV_EVENT_CLICKED){
		if(GUI_DisconnectNetwork){
			GUI_DisconnectNetwork();
			if(choosedSSID)
			{
				char buf[50];
				snprintf(buf, 50, "%s %s", STR_SETTINGS_CONNECTEDNETWORK, STR_SETTINGS_NETWORK_NONE);
				lv_label_set_text(network_currentNet_label, buf);
				choosedSSID = NULL;
			}
		}
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
	}
}

static void connectNatwork_task(lv_task_t* task)
{
	const char * pswd = NULL;
	pswd = lv_textarea_get_text(textarea);

	if(GUI_ConnectNetwork(lv_list_get_btn_text(choosedSSID), pswd))
	{
		char buf[40];
		snprintf(buf, 40, "%s %s", STR_SETTINGS_CONNECTEDNETWORK, GUI_ActualSSID);
		lv_label_set_text(network_currentNet_label, buf);
	}else
	{
		choosedSSID = oldSSID;
	}
	lv_obj_clean(kb_bkground);
	lv_obj_set_hidden(kb_bkground, true);
	textarea = NULL;
}


static void keyboard_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();

	lv_keyboard_def_event_cb(obj, event);
	if(event == LV_EVENT_CANCEL)
	{
		lv_obj_clean(kb_bkground);
		lv_obj_set_hidden(kb_bkground, true);
		textarea = NULL;
		choosedSSID = oldSSID;
	}
	else if(event == LV_EVENT_APPLY)
	{
		if(GUI_ConnectNetwork != NULL)
		{
			lv_obj_t * child = lv_obj_get_child(kb_bkground, NULL);
			while(child) {
				lv_obj_set_hidden(child, true);
				child = lv_obj_get_child(kb_bkground, child);
			}

			lv_obj_t * label_connect = lv_label_create(kb_bkground, NULL);
			lv_label_set_text(label_connect, STR_SETTINGS_CONNECTING);
			lv_obj_set_auto_realign(label_connect, true);
			lv_obj_align(label_connect, NULL, LV_ALIGN_CENTER, 0, 0);
			lv_obj_add_style(label_connect, LV_STATE_DEFAULT, &style_title);


			task = lv_task_create(connectNatwork_task, 3000, LV_TASK_PRIO_HIGHEST, NULL);
			lv_task_once(task);

		}
		else
		{
			choosedSSID = oldSSID;
		}

	}
}


static void list_btn_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_CLICKED)
	{
		if(obj != choosedSSID)
		{
			oldSSID = choosedSSID;
			choosedSSID = obj;

			lv_obj_set_hidden(kb_bkground, false);

			textarea = lv_textarea_create(kb_bkground, NULL);
			lv_obj_set_size(textarea, 250, 80);
			lv_obj_align(textarea, NULL, LV_ALIGN_IN_TOP_MID, 0, 40);
			lv_textarea_set_max_length(textarea, 32);
			lv_textarea_set_text(textarea, "");
			lv_obj_add_style(textarea, LV_PAGE_PART_BG, &style_borders);

			lv_obj_t * label = lv_label_create(kb_bkground, NULL);
			lv_obj_add_style(label, LV_STATE_DEFAULT, &style_title);
			lv_label_set_text(label, STR_SETTINGS_NETWORKMSG_TITLE);
			lv_obj_align(label, textarea, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

			lv_obj_t * kb = lv_keyboard_create(kb_bkground, NULL);
			lv_keyboard_set_cursor_manage(kb, true);
			lv_obj_set_event_cb(kb, keyboard_evt_handler);
			lv_keyboard_set_textarea(kb, textarea);
			lv_obj_add_style(kb, LV_KEYBOARD_PART_BG, &style_borders);
		}
		else
		{
			lv_obj_set_hidden(kb_bkground, false);
			lv_obj_clean(kb_bkground);

			listMsgBox = lv_msgbox_create(kb_bkground, NULL);
			lv_msgbox_add_btns(listMsgBox, labelsMsgBox);
			lv_msgbox_set_text(listMsgBox, STR_SETTINGS_MSGBOX_TITLE);
			lv_obj_align(listMsgBox, NULL, LV_ALIGN_CENTER, 0, 0);
			lv_obj_set_event_cb(listMsgBox, msgBox_event_handler);
			lv_obj_add_style(listMsgBox, LV_MSGBOX_PART_BG, &style_borders);
			lv_obj_add_style(listMsgBox, LV_MSGBOX_PART_BTN_BG, &style_font20);
		}
	}
}

static void switch_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	if(event == LV_EVENT_VALUE_CHANGED)
	{
		GUI_SafeMode = lv_switch_get_state(obj);
	}
}

static void setNetworks(void){
	lv_list_clean(network_list);
	for(int i = 0; GUI_NetworkList[i][0] != '\0'  ; i++)
	{
		network_list_btn = lv_list_add_btn(network_list, NULL, GUI_NetworkList[i]);
		lv_obj_set_event_cb(network_list_btn, list_btn_evt_handler);
		lv_obj_add_style(network_list_btn, LV_BTN_PART_MAIN, &style_font20);

		if(!strcmp(GUI_ActualSSID, GUI_NetworkList[i]))
			choosedSSID = network_list_btn;
	}

	char buf[80];

	if(GUI_ActualSSID == NULL || GUI_ActualSSID[0] == '\0')
	{
		snprintf(buf, 80, "%s %s", STR_SETTINGS_CONNECTEDNETWORK, STR_SETTINGS_NETWORK_NONE);
		lv_label_set_text(network_currentNet_label, buf);
	}
	else
	{
		snprintf(buf, 80, "%s %s", STR_SETTINGS_CONNECTEDNETWORK, GUI_ActualSSID);
		lv_label_set_text(network_currentNet_label, buf);
	}
}

static void initDisplay(void)
{

	label_display = lv_label_create(main_page, NULL);
	lv_label_set_text(label_display, STR_SETTINGS_DISPLAY);
	lv_obj_add_style(label_display, LV_STATE_DEFAULT, &style_font20);
	lv_obj_align(label_display, main_page, LV_ALIGN_IN_TOP_MID, 0, 20);

	label_brightness = lv_label_create(main_page, NULL);
	lv_label_set_text(label_brightness, STR_SETTINGS_BRIGHTNESS);
	lv_obj_add_style(label_brightness, LV_STATE_DEFAULT, &style_font20);
	lv_obj_align(label_brightness, label_display, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

	slider_brightness = lv_slider_create(main_page, NULL);
	lv_obj_set_width(slider_brightness, 350);
	lv_obj_set_event_cb(slider_brightness, slider_brightness_evt_handler);
	lv_slider_set_range(slider_brightness, 20 , 100);
	lv_slider_set_value(slider_brightness, 100, LV_ANIM_OFF);
	lv_obj_align(slider_brightness, label_brightness, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

	label_slider_value = lv_label_create(main_page, NULL);
	lv_label_set_text(label_slider_value, "100");
	lv_obj_set_auto_realign(label_slider_value, true);
	lv_obj_align(label_slider_value, slider_brightness, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
	lv_obj_add_style(label_slider_value, LV_LABEL_PART_MAIN, &style_font20);
}

static void initLabels(void)
{
	label_status = lv_label_create(main_page, NULL);
	lv_label_set_text(label_status, STR_SETTINGS_STATUS);
	lv_obj_align(label_status, main_page, LV_ALIGN_IN_TOP_MID, 0, 170);
	lv_obj_add_style(label_status, LV_LABEL_PART_MAIN, &style_font20);

	label_redterm = lv_label_create(main_page, NULL);
	lv_label_set_text(label_redterm, STR_SETTINGS_DAYS_TO_END);
	lv_obj_align(label_redterm, label_status, LV_ALIGN_OUT_LEFT_MID, -20, 40);
	lv_obj_add_style(label_redterm, LV_LABEL_PART_MAIN, &style_yellowterm);

	spinbox_red = lv_spinbox_create(main_page, NULL);
	lv_spinbox_set_digit_format(spinbox_red, 2, 0);
	lv_spinbox_set_range(spinbox_red, 0, 10);
	lv_spinbox_set_value(spinbox_red, DEFAULT_TERM_RANGE);
	lv_obj_set_width(spinbox_red, 40);
	lv_obj_align(spinbox_red, label_redterm, LV_ALIGN_IN_RIGHT_MID, 275, 0);

	lv_coord_t h = lv_obj_get_height(spinbox_red);
	btnred_plus = lv_btn_create(main_page, NULL);
	lv_obj_set_size(btnred_plus, h, h);
	lv_obj_align(btnred_plus, spinbox_red, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_theme_apply(btnred_plus, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btnred_plus, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
	lv_obj_set_event_cb(btnred_plus, spinbox_plus_evt_handler);

	btnred_minus = lv_btn_create(main_page, btnred_plus);
	lv_obj_align(btnred_minus, spinbox_red, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_local_value_str(btnred_minus, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
	lv_obj_set_event_cb(btnred_minus, spinbox_minus_event_cb);
}

static void initNetwork(void)
{
	network_label = lv_label_create(main_page, NULL);
	lv_label_set_text(network_label, STR_SETTINGS_NETWORK);
	lv_obj_align(network_label, main_page,  LV_ALIGN_CENTER, 0, 160);
	lv_obj_add_style(network_label, LV_LABEL_PART_MAIN, &style_font20);

	network_currentNet_label = lv_label_create(main_page, NULL);
	lv_obj_align(network_currentNet_label, network_label,  LV_ALIGN_OUT_BOTTOM_LEFT, -175, 10);
	lv_obj_add_style(network_currentNet_label, LV_LABEL_PART_MAIN, &style_font20);

	network_list = lv_list_create(main_page, NULL);
	lv_obj_align(network_list, network_label, LV_ALIGN_OUT_BOTTOM_LEFT, -175, 50);
	lv_obj_set_size(network_list, 380, 120);
	lv_list_set_edge_flash(network_list, true);
	lv_page_set_scroll_propagation(network_list, true);
	//lv_obj_add_style(list, LV_BTN_PART_MAIN, &style_borders);

	network_refresh_btn = lv_imgbtn_create(main_page, NULL);
	lv_obj_align(network_refresh_btn, network_list, LV_ALIGN_OUT_TOP_RIGHT, 120, 50);
	lv_obj_set_event_cb(network_refresh_btn, btn_refresh_evt_handler);
	lv_imgbtn_set_src(network_refresh_btn, LV_BTN_STATE_RELEASED , &refresh);
	lv_imgbtn_set_src(network_refresh_btn, LV_BTN_STATE_PRESSED , &refresh);

	network_disconnect_btn = lv_imgbtn_create(main_page, NULL);
	lv_obj_align(network_disconnect_btn, network_list, LV_ALIGN_OUT_TOP_RIGHT, 120, 100);
	lv_obj_set_event_cb(network_disconnect_btn, btn_disconnect_evt_handler);
	lv_imgbtn_set_src(network_disconnect_btn, LV_BTN_STATE_RELEASED , &disconnect);
	lv_imgbtn_set_src(network_disconnect_btn, LV_BTN_STATE_PRESSED , &disconnect);
}

static void initSafeMode(void)
{
	safemode_label = lv_label_create(main_page, NULL);
	lv_label_set_text(safemode_label, STR_SETTINGS_SAFEMODE);
	lv_obj_align(safemode_label, network_list, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 30);
	lv_obj_add_style(safemode_label, LV_LABEL_PART_MAIN, &style_font20);

	safemode_switch = lv_switch_create(main_page, NULL);
	lv_obj_align(safemode_switch, safemode_label, LV_ALIGN_OUT_RIGHT_MID, 260, 0);
	lv_obj_set_event_cb(safemode_switch, switch_evt_handler);
}
