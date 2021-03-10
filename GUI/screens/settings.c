
#include "settings.h"
#include "styles.h"
#include "fsl_debug_console.h"
#include "stdio.h"
#include "GUI_ExtData.h"
#include "GUI_IntData.h"

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
//static lv_obj_t * label_yellowterm;
static lv_obj_t * label_term;
//static lv_obj_t * label_termdays;
static lv_obj_t * btn_img_return;

static lv_obj_t * spinbox_red;
static lv_obj_t * btnred_plus;
static lv_obj_t * btnred_minus;
/*
static lv_obj_t * spinbox_yellow;
static lv_obj_t * btnyellow_plus;
static lv_obj_t * btnyellow_minus;
 */
static lv_obj_t * network_label;
static lv_obj_t * network_list;
static lv_obj_t * network_list_btn;
static lv_obj_t * network_refresh_btn;
static lv_obj_t * network_disconnect_btn;

static lv_obj_t * listMsgBox;
static const char * labelsMsgBox[] = {"OK", ""};

static lv_obj_t * kb_bkground;
static lv_obj_t * textarea;

static lv_obj_t * choosedSSID = NULL;

static uint8_t termRange;

LV_IMG_DECLARE(previous)
LV_IMG_DECLARE(refresh)
LV_IMG_DECLARE(disconnect)

static void setNetworks(void);


static void slider_brightness_evt_handler(lv_obj_t * slider, lv_event_t event);
static void btn_img_return_evt_handler(lv_obj_t * slider, lv_event_t event);


static void slider_brightness_evt_handler(lv_obj_t * slider, lv_event_t event){
	if(event == LV_EVENT_VALUE_CHANGED){
		static char buf[5];
		int16_t brightness = lv_slider_get_value(slider);
		snprintf(buf, 5, "%d", brightness);
		lv_label_set_text(label_slider_value, buf);
		if(GUI_SetBrightness != NULL)
			GUI_SetBrightness(brightness);
	}
}

static void btn_img_return_evt_handler(lv_obj_t * slider, lv_event_t event){
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

static void spinboxred_plus_event_cb(lv_obj_t * btn, lv_event_t event){
	if(event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
	{
		lv_spinbox_increment(spinbox_red);
		GUI_TermRange = lv_spinbox_get_value(spinbox_red);
	}
}

static void spinboxred_minus_event_cb(lv_obj_t * btn, lv_event_t event){
	if(event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT){
		lv_spinbox_decrement(spinbox_red);
		GUI_TermRange = lv_spinbox_get_value(spinbox_red);
	}
}
/*
static void spinboxyellow_plus_event_cb(lv_obj_t * btn, lv_event_t event){
	if(event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT){
		lv_spinbox_increment(spinbox_yellow);
		int var = lv_spinbox_get_value(spinbox_yellow);
		GUI_YellowTerm = var;
	}
}

static void spinboxyellow_minus_event_cb(lv_obj_t * btn, lv_event_t event){
	if(event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT){
		lv_spinbox_decrement(spinbox_yellow);
		int var = lv_spinbox_get_value(spinbox_yellow);
		GUI_YellowTerm = var;
	}
}
 */
static void btn_refresh_event_cb(lv_obj_t * btn, lv_event_t event){
	if(event == LV_EVENT_CLICKED){
		if(GUI_ScanNetworks)
		{
			GUI_ScanNetworks();
			setNetworks();
		}	
	}
}

static void btn_disconnect_event_cb(lv_obj_t * btn, lv_event_t event){
	if(event == LV_EVENT_CLICKED){
		if(GUI_DisconnectNetwork){
			GUI_DisconnectNetwork();
			if(choosedSSID)
			{
				lv_obj_set_style_local_text_color(choosedSSID, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
				choosedSSID = NULL;
			}
		}
	}
}

static void msgBox_event_handler(lv_obj_t * obj, lv_event_t event)
{
	if(event == LV_EVENT_DELETE && obj == listMsgBox)
	{
		lv_obj_del_async(lv_obj_get_parent(listMsgBox));
		listMsgBox = NULL;
	}
	else if(event == LV_EVENT_VALUE_CHANGED)
	{
		lv_msgbox_start_auto_close(obj, 0);
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
		choosedSSID = NULL;
	}
	else if(event == LV_EVENT_APPLY)
	{
		const char * pswd = NULL;
		pswd = lv_textarea_get_text(textarea);

		if(GUI_ConnectNetwork != NULL)
		{
			if(GUI_ConnectNetwork(lv_list_get_btn_text(choosedSSID), pswd))
			{
				lv_obj_set_style_local_text_color(choosedSSID, 0, LV_STATE_DEFAULT, LV_COLOR_GREEN);
			}else
			{
				lv_obj_set_style_local_text_color(choosedSSID, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
				choosedSSID = NULL;
			}
		}
		else
		{
			lv_obj_set_style_local_text_color(choosedSSID, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
			choosedSSID = NULL;
		}
		lv_obj_del(kb_bkground);
		kb_bkground = NULL;
		textarea = NULL;
	}
}


static void list_btn_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	if(event == LV_EVENT_CLICKED)
	{
		if(obj != choosedSSID)
		{
			if(choosedSSID)
				lv_obj_set_style_local_text_color(choosedSSID, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);

			choosedSSID = obj;

			kb_bkground = lv_obj_create(screen ,NULL);
			lv_obj_reset_style_list(kb_bkground, LV_OBJ_PART_MAIN);
			lv_obj_set_style_local_bg_color(kb_bkground, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
			lv_obj_set_pos(kb_bkground, 0, 0);
			lv_obj_set_size(kb_bkground, LV_HOR_RES, LV_VER_RES);
			lv_obj_set_style_local_bg_opa(kb_bkground, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_80);

			textarea = lv_textarea_create(kb_bkground, NULL);
			lv_obj_set_size(textarea, 250, 80);
			lv_obj_align(textarea, NULL, LV_ALIGN_IN_TOP_MID, 0, 40);
			lv_textarea_set_max_length(textarea, 32);
			lv_textarea_set_text(textarea, "");

			lv_obj_t * label = lv_label_create(kb_bkground, NULL);
			lv_obj_add_style(label, LV_STATE_DEFAULT, &style_title);
			lv_label_set_text(label, STR_SETTINGS_NETWORKMSG_TITLE);
			lv_obj_align(label, textarea, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

			lv_obj_t * kb = lv_keyboard_create(kb_bkground, NULL);
			lv_keyboard_set_cursor_manage(kb, true);
			lv_obj_set_event_cb(kb, keyboard_evt_handler);
			lv_keyboard_set_textarea(kb, textarea);
		}
		else
		{
			kb_bkground = lv_obj_create(screen ,NULL);
			lv_obj_reset_style_list(kb_bkground, LV_OBJ_PART_MAIN);
			lv_obj_set_style_local_bg_color(kb_bkground, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
			lv_obj_set_pos(kb_bkground, 0, 0);
			lv_obj_set_size(kb_bkground, LV_HOR_RES, LV_VER_RES);
			lv_obj_set_style_local_bg_opa(kb_bkground, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);

			listMsgBox = lv_msgbox_create(kb_bkground, NULL);
			lv_msgbox_add_btns(listMsgBox, labelsMsgBox);
			lv_msgbox_set_text(listMsgBox, STR_SETTINGS_MSGBOX_TITLE);
			lv_obj_align(listMsgBox, NULL, LV_ALIGN_CENTER, 0, 0);
			lv_obj_set_event_cb(listMsgBox, msgBox_event_handler);
			lv_obj_add_style(listMsgBox, LV_MSGBOX_PART_BG, &style_font20);
			lv_obj_add_style(listMsgBox, LV_MSGBOX_PART_BTN_BG, &style_font20);
		}
	}
}

static void initDisplay(void)
{

	label_display = lv_label_create(main_page, NULL);
	lv_label_set_text(label_display, "WYŚWIETLACZ");
	lv_obj_add_style(label_display, LV_STATE_DEFAULT, &style_font20);
	lv_obj_align(label_display, main_page, LV_ALIGN_IN_TOP_MID, 0, 20);

	label_brightness = lv_label_create(main_page, NULL);
	lv_label_set_text(label_brightness, "Jasność");
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

	label_term = lv_label_create(main_page, NULL);
	lv_label_set_text(label_term, STR_SETTINGS_LABELS);
	lv_obj_align(label_term, main_page, LV_ALIGN_IN_TOP_LEFT, 10, 200);
	lv_obj_add_style(label_term, LV_LABEL_PART_MAIN, &style_font20);

	label_redterm = lv_label_create(main_page, NULL);
	lv_label_set_text(label_redterm, STR_SETTINGS_DAYS_TO_END);
	lv_obj_align(label_redterm, label_term, LV_ALIGN_IN_TOP_LEFT, 10, 50);
	lv_obj_add_style(label_redterm, LV_LABEL_PART_MAIN, &style_yellowterm);

	spinbox_red = lv_spinbox_create(main_page, NULL);
	lv_spinbox_set_digit_format(spinbox_red, 2, 0);
	//lv_spinbox_set_rollover(spinbox_red, true);
	lv_spinbox_set_range(spinbox_red, 0, 10);
	lv_spinbox_set_value(spinbox_red, DEFAULT_TERM_RANGE);
	lv_obj_set_width(spinbox_red, 40);
	lv_obj_align(spinbox_red, label_redterm, LV_ALIGN_IN_RIGHT_MID, 259, 0);

	lv_coord_t h = lv_obj_get_height(spinbox_red);
	btnred_plus = lv_btn_create(main_page, NULL);
	lv_obj_set_size(btnred_plus, h, h);
	lv_obj_align(btnred_plus, spinbox_red, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_theme_apply(btnred_plus, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btnred_plus, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
	lv_obj_set_event_cb(btnred_plus, spinboxred_plus_event_cb);

	btnred_minus = lv_btn_create(main_page, btnred_plus);
	lv_obj_align(btnred_minus, spinbox_red, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_local_value_str(btnred_minus, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
	lv_obj_set_event_cb(btnred_minus, spinboxred_minus_event_cb);
	/*
	spinbox_yellow = lv_spinbox_create(main_page, spinbox_red);
	lv_spinbox_set_digit_format(spinbox_yellow, 2, 0);
	lv_spinbox_set_range(spinbox_yellow, 0, 20);
	lv_spinbox_set_value(spinbox_yellow, 6);
	lv_obj_align(spinbox_yellow, label_yellowterm, LV_ALIGN_IN_RIGHT_MID, 338, 0);

	btnyellow_plus = lv_btn_create(main_page, btnred_plus);
	lv_obj_align(btnyellow_plus, spinbox_yellow, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_event_cb(btnyellow_plus, spinboxyellow_plus_event_cb);

	btnyellow_minus = lv_btn_create(main_page, btnred_minus);
	lv_obj_align(btnyellow_minus, spinbox_yellow, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_event_cb(btnyellow_minus, spinboxyellow_minus_event_cb);
	 */
}

static void setNetworks(void){
	lv_list_clean(network_list);
	for(int i = 0; GUI_NetworkList[i][0] != '\0'  ; i++)
	{
		network_list_btn = lv_list_add_btn(network_list, NULL, GUI_NetworkList[i]);
		lv_obj_set_event_cb(network_list_btn, list_btn_evt_handler);
		lv_obj_add_style(network_list_btn, LV_BTN_PART_MAIN, &style_font20);
		if(GUI_ActualSSID)
		{
			if(!strcmp(GUI_ActualSSID, GUI_NetworkList[i]))
			{
				lv_obj_set_style_local_text_color(network_list_btn, 0, LV_STATE_DEFAULT, LV_COLOR_GREEN);
				choosedSSID = network_list_btn;
			}
		}
		//lv_obj_set_style_local_bg_color(network_list_btn, 0, LV_BTN_PART_MAIN, LV_COLOR_MAKE(0xF5, 0x77, 0x14));
	}
}

static void initNetwork(void)
{
	network_label = lv_label_create(main_page, NULL);
	lv_label_set_text(network_label, STR_SETTINGS_NETWORK);
	lv_obj_align(network_label, main_page,  LV_ALIGN_CENTER, 0, 200);
	lv_obj_add_style(network_label, LV_LABEL_PART_MAIN, &style_font20);

	network_list = lv_list_create(main_page, NULL);
	lv_obj_align(network_list, network_label, LV_ALIGN_OUT_BOTTOM_LEFT, -210, 10);
	lv_obj_set_size(network_list, 380, 120);
	lv_list_set_edge_flash(network_list, true);
	lv_page_set_scroll_propagation(network_list, true);
	//lv_obj_add_style(list, LV_BTN_PART_MAIN, &style_borders);

	network_refresh_btn = lv_imgbtn_create(main_page, NULL);
	lv_obj_align(network_refresh_btn, network_list, LV_ALIGN_OUT_TOP_RIGHT, 120, 50);
	lv_obj_set_event_cb(network_refresh_btn, btn_refresh_event_cb);
	lv_imgbtn_set_src(network_refresh_btn, LV_BTN_STATE_RELEASED , &refresh);
	lv_imgbtn_set_src(network_refresh_btn, LV_BTN_STATE_PRESSED , &refresh);

	network_disconnect_btn = lv_imgbtn_create(main_page, NULL);
	lv_obj_align(network_disconnect_btn, network_list, LV_ALIGN_OUT_TOP_RIGHT, 120, 100);
	lv_obj_set_event_cb(network_disconnect_btn, btn_disconnect_event_cb);
	lv_imgbtn_set_src(network_disconnect_btn, LV_BTN_STATE_RELEASED , &disconnect);
	lv_imgbtn_set_src(network_disconnect_btn, LV_BTN_STATE_PRESSED , &disconnect);
}

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

	initDisplay();
	initLabels();
	initNetwork();

	termRange = GUI_TermRange;

	return screen;
}

void SETTINGS_Show(lv_obj_t * previous_screen){
	prev_screen = previous_screen;
	setNetworks();
	//lv_label_set_text(label_yellowterm, STR_SETTINGS_LABELS_YELLOW);		// problem z wyswetlaniem
	lv_scr_load(screen);
}
