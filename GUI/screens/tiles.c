/*
 * tiles.c
 *
 *  Created on: Dec 17, 2020
 *      Author: Kamil Wielgosz
 */

#include <conf_internal/GUI_IntData.h>
#include <tiles.h>
#include "stdio.h"
#include "GUI_ExtData.h"


// parent data
static lv_obj_t * parent = NULL;
static lv_obj_t * header = NULL;

// tileview
static lv_obj_t * tileview = NULL;
static icons icon_btns[9];
static lv_obj_t * menu = NULL;
static lv_point_t valid_pos[18];
static tiles_group tils_group[9];
static uint8_t groups = 0;
static GUI_DATA_CATEGORIES choosed_Group = 0;
static uint16_t choosed_Row = 0;
static uint16_t choosedTerm = 0;

// msgbox shoplistt
static bool openMsgBox = false;
static lv_obj_t * listMsgBox = NULL;
static const char * labelsMsgBox[] = {STR_TILES_MSGBTN_ADD, STR_TILES_MSGBTN_CAL, ""};

// msgbox delete
static lv_obj_t * delProdMsgBox = NULL;
static const char * labelsSafeModeMsgBox[] = {STR_TILES_MSGBTN_DEL, STR_TILES_MSGBTN_CAL, ""};

// msgbox desc
static lv_obj_t * descMsgBox;
static const char * labelsDescMsgBox[] = {STR_SHOPLIST_CONFIRM_MSG, ""};

// TO ANIM
static lv_anim_t anim;
static lv_obj_t * anim_page_t = NULL;
static lv_obj_t * anim_table = NULL;

// TASK
static lv_task_t * task = NULL;

LV_IMG_DECLARE(alcohols)
LV_IMG_DECLARE(dairy)
LV_IMG_DECLARE(drinks)
LV_IMG_DECLARE(fish)
LV_IMG_DECLARE(frozen)
LV_IMG_DECLARE(fruitvege)
LV_IMG_DECLARE(meat)
LV_IMG_DECLARE(sauces)
LV_IMG_DECLARE(sweets)
LV_IMG_DECLARE(klepsydra)

//
// Event functions
//
static void tileview_evt_handler(lv_obj_t * obj, lv_event_t event);
static void icon_btn_evt_handler(lv_obj_t * obj, lv_event_t event);
static void table_evt_handler(lv_obj_t * obj, lv_event_t event);
static void descMsgBox_evt_handler(lv_obj_t * obj, lv_event_t event);
static void msgBox_event_handler(lv_obj_t * obj, lv_event_t event);
static void termstable_evt_handler(lv_obj_t * obj, lv_event_t event);
//
// Helper functions
//
static void anim_start_shift(void * var, lv_coord_t n);
static void setIcon(int j, GUI_DATA_CATEGORIES cat, int align_x, int align_y, const lv_img_dsc_t * img);
static void setValidPos(void);
static void setIconBtns(void);
static void setTileCol(void);
static void refresh_task_handler(lv_task_t * task);
static void tiles_Refresh(void);


//
//	Init function
//
void TILES_Init(lv_obj_t * screen, lv_obj_t * title){

	parent = screen;
	header = title;
	groups = GUI_IntData_GetGroups();
	setValidPos();

	tileview = lv_tileview_create(screen, NULL);
	lv_tileview_set_edge_flash(tileview, true);
	lv_obj_set_pos(tileview, 0, 40);
	lv_tileview_set_valid_positions(tileview, valid_pos, groups*2);
	lv_obj_set_size(tileview, LV_HOR_RES, LV_VER_RES - 40);
	lv_obj_set_event_cb(tileview, tileview_evt_handler);
	lv_obj_add_style(tileview, LV_STATE_DEFAULT, &style_borders);

	setTileCol();
	lv_anim_init(&anim);
	task = lv_task_create(refresh_task_handler, 300, LV_TASK_PRIO_MID, &GUI_DataChanged);
}

//
//	Static functions
//
static void refresh_task_handler(lv_task_t * task){
	bool * change = GUI_DataChanged;
	if(*change){
		GUI_IntData_GroupData();
		tiles_Refresh();
		*change = false;
	}
}


static void tiles_Refresh(void){
	groups = GUI_IntData_GetGroups();
	setValidPos();
	lv_obj_t * child = lv_obj_get_child(tileview, NULL);
	lv_obj_clean(child);
	lv_label_set_text(header, STR_MENU_TITLE);
	lv_tileview_set_valid_positions(tileview, valid_pos, groups*2);
	setTileCol();
}

static void tileview_evt_handler(lv_obj_t * obj, lv_event_t event){

	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_VALUE_CHANGED){
		lv_tileview_ext_t * a = (lv_tileview_ext_t *)obj->ext_attr;
		if(a->act_id.x != 0){
			lv_label_set_text(header,GUI_GroupsNames[icon_btns[a->act_id.y].category]);
			lv_obj_align(header, NULL, LV_ALIGN_IN_TOP_MID, 0, 12);
		}
		else if(a->act_id.x == 0 && a->act_id.y == 0)
		{
			lv_label_set_text(header, STR_MENU_TITLE);
			lv_obj_align(header, NULL, LV_ALIGN_IN_TOP_MID, 0, 12);
		}else if(a->act_id.x == 0 && a->act_id.y > 0){
			lv_tileview_set_tile_act(tileview, 0, 0, LV_ANIM_ON);
		}

	}
}


static void icon_btn_evt_handler(lv_obj_t * obj, lv_event_t event){

	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_LONG_PRESSED)
	{
		for(int i = 0; i < NUMBER_CATEGORIES; i++)
		{
			if(icon_btns[i].icon == obj){
				GUI_DATA_CATEGORIES *cat = &icon_btns[i].category;
				openMsgBox = true;
				lv_obj_t * bkground = lv_obj_create(parent ,NULL);
				lv_obj_reset_style_list(bkground, LV_OBJ_PART_MAIN);
				lv_obj_set_style_local_bg_color(bkground, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
				lv_obj_set_pos(bkground, 0, 0);
				lv_obj_set_size(bkground, LV_HOR_RES, LV_VER_RES);
				lv_obj_set_style_local_bg_opa(bkground, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);

				char buff[80];
				snprintf(buff, 80, "%s: %s",STR_TILES_MSGBOX_CAT ,GUI_GroupsNames[*cat]);
				descMsgBox = lv_msgbox_create(bkground, NULL);
				lv_msgbox_add_btns(descMsgBox, labelsDescMsgBox);
				lv_msgbox_set_text(descMsgBox, buff);
				lv_obj_align(descMsgBox, NULL, LV_ALIGN_CENTER, 0, 0);
				lv_obj_set_event_cb(descMsgBox, descMsgBox_evt_handler);
				lv_obj_add_style(descMsgBox, LV_MSGBOX_PART_BG, &style_borders);
				lv_obj_add_style(descMsgBox, LV_MSGBOX_PART_BTN_BG, &style_font20);
				return;
			}
		}
	}
	else if(event == LV_EVENT_CLICKED){
		for(int i = 0; i < NUMBER_CATEGORIES; i++)
		{
			if(icon_btns[i].icon == obj){
				lv_tileview_set_tile_act(tileview, 1, i, LV_ANIM_ON);
				return;
			}
		}
	}
}


/*
 * if n < 0 then shift to right
 * 	  n > 0 to left
 */

static void anim_start_shift(void * var, lv_coord_t n)
{

	
	if(n > 0)
	{
		// show
		lv_obj_set_size(anim_page_t, 490 - n, LV_VER_RES - 70);
		lv_obj_align_x(var, NULL, LV_ALIGN_IN_TOP_MID, 320 - n);
		lv_table_set_col_width(anim_table, 0, 370 - n);
	}
	else
	{
		//hide
		lv_obj_set_size(anim_page_t, 460 + n, LV_VER_RES - 70);
		lv_obj_align_x(var, NULL, LV_ALIGN_IN_TOP_MID, 320 + n);
		lv_table_set_col_width(anim_table, 0, 350 + n);
		if(n == -1) lv_obj_set_hidden(var, true);
	}
}

static void descMsgBox_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_DELETE && obj == descMsgBox)
	{
		lv_obj_del_async(lv_obj_get_parent(descMsgBox));
		descMsgBox = NULL;
		openMsgBox = false;
	}
	else if(event == LV_EVENT_VALUE_CHANGED && openMsgBox == true)
	{
		lv_msgbox_start_auto_close(obj, 0);
	}
}

static void msgBox_event_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_DELETE)
	{
		lv_obj_del_async(lv_obj_get_parent(obj));
		listMsgBox = NULL;
		delProdMsgBox = NULL;
		openMsgBox = false;
	}
	else if(event == LV_EVENT_VALUE_CHANGED && openMsgBox == true)
	{
		const uint32_t * btn = lv_event_get_data();
		if(*btn == 0)
		{
			if(GUI_SafeMode && obj == delProdMsgBox)
			{
				uint8_t * uid = GUI_SortedProd[choosed_Group].products[choosed_Row].product[choosedTerm].uid;
				if(GUI_DeleteProduct)
					GUI_DeleteProduct(uid);
			}
			else
				GUI_IntData_AddProdToShopList(GUI_SortedProd[choosed_Group].products[choosed_Row].name);
		}

		lv_msgbox_start_auto_close(obj, 0);
	}
}

static void table_evt_handler(lv_obj_t * obj, lv_event_t event)
{

	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_LONG_PRESSED || event == LV_EVENT_CLICKED || event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_PRESSED || event == LV_EVENT_RELEASED){
		lv_coord_t group, x;
		lv_tileview_get_tile_act(tileview, &x , &group);
		lv_table_set_row_cnt(obj, GUI_SortedProd[icon_btns[group].category].count + 1);
		if(event == LV_EVENT_LONG_PRESSED)
		{
			if(isShopListFull())
			{
				openMsgBox = true;
				lv_obj_t * bkground = lv_obj_create(parent ,NULL);
				lv_obj_reset_style_list(bkground, LV_OBJ_PART_MAIN);
				lv_obj_set_style_local_bg_color(bkground, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
				lv_obj_set_pos(bkground, 0, 0);
				lv_obj_set_size(bkground, LV_HOR_RES, LV_VER_RES);
				lv_obj_set_style_local_bg_opa(bkground, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);

				descMsgBox = lv_msgbox_create(bkground, NULL);
				lv_msgbox_add_btns(descMsgBox, labelsDescMsgBox);
				lv_obj_align(descMsgBox, NULL, LV_ALIGN_CENTER, 0, 0);
				lv_obj_set_event_cb(descMsgBox, descMsgBox_evt_handler);
				lv_obj_add_style(descMsgBox, LV_MSGBOX_PART_BG, &style_borders);
				lv_obj_add_style(descMsgBox, LV_MSGBOX_PART_BTN_BG, &style_font20);
				lv_msgbox_set_text(descMsgBox, STR_SHOPLIST_LIST_FULL);
				return;
			}
			lv_coord_t temp, tile;
			uint16_t tmp;
			lv_tileview_get_tile_act(tileview, &temp , &tile);
			choosed_Group = icon_btns[tile].category;
			if(lv_table_get_pressed_cell(obj, &choosed_Row, &tmp) == LV_RES_OK)
			{
				// protection before click hidden row at the end table "lvgl bug"
				if(choosed_Row == lv_table_get_row_cnt(obj)) return;

				// title col
				if(choosed_Row == 0) return;

				// simplification to moves on arrays
				choosed_Row--;
				openMsgBox = true;

				lv_obj_t * bkground = lv_obj_create(parent ,NULL);
				lv_obj_reset_style_list(bkground, LV_OBJ_PART_MAIN);
				lv_obj_set_style_local_bg_color(bkground, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
				lv_obj_set_pos(bkground, 0, 0);
				lv_obj_set_size(bkground, LV_HOR_RES, LV_VER_RES);
				lv_obj_set_style_local_bg_opa(bkground, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
				lv_obj_align(bkground, NULL, LV_ALIGN_CENTER, 0, 0);

				char buff[100];

				snprintf(buff, 100, "%s %s?", STR_TILES_MSGBTN_TITLE,
						GUI_SortedProd[choosed_Group].products[choosed_Row].name);

				listMsgBox = lv_msgbox_create(bkground, NULL);
				lv_obj_align(listMsgBox, NULL, LV_ALIGN_CENTER, 0, -30);
				lv_obj_set_event_cb(listMsgBox, msgBox_event_handler);
				lv_obj_add_style(listMsgBox, LV_MSGBOX_PART_BG, &style_borders);
				lv_obj_add_style(listMsgBox, LV_MSGBOX_PART_BTN_BG, &style_font20);
				lv_msgbox_add_btns(listMsgBox, labelsMsgBox);
				lv_msgbox_set_text(listMsgBox, buff);

			}

		}
		else if(event == LV_EVENT_CLICKED){

			if(openMsgBox)
				return;

			uint16_t y;

			if(lv_table_get_pressed_cell(obj, &choosed_Row, &y) == LV_RES_OK)
			{

				// protection before click hidden row at the end table "lvgl bug"
				if(choosed_Row == lv_table_get_row_cnt(obj)) return;

				// title col = no acion
				if(choosed_Row == 0) return;

				// simplification to moves on arrays
				choosed_Row--;

				for(int i=1; i< lv_table_get_row_cnt(obj); i++){
					for(int j=0; j< lv_table_get_col_cnt(obj); j++){
						lv_table_set_cell_type(obj, i, j, LV_TABLE_PART_CELL1);
					}
				}


				choosed_Group = icon_btns[group].category;

				lv_obj_t * tile = tils_group[group].tile;
				lv_obj_t * container = lv_obj_get_child(tile, NULL);
				lv_obj_t * label;
				lv_obj_t * det_page;
				if(choosed_Group == DATA_END_OF_EXPIRY)
				{
					label = lv_obj_get_child(container, NULL);
					det_page = lv_obj_get_child(container, label);
				}
				else
					det_page = lv_obj_get_child(container, NULL);

				lv_obj_t * tab_page = lv_obj_get_child(container, det_page);
				lv_obj_t * det_tab = lv_obj_get_child(lv_obj_get_child(det_page, NULL), NULL);


				anim_page_t = tab_page;
				anim_table = obj;


				// hide terms table
				if(tils_group[group].selected_row == choosed_Row){
					tils_group[group].selected_row = -1;
					lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t) anim_start_shift);
					lv_anim_set_var(&anim, det_page);
					lv_anim_set_time(&anim, 1200);
					lv_anim_set_values(&anim, -180, -1);
					lv_anim_start(&anim);
					return;
				}

				lv_table_set_cell_type(obj, choosed_Row+1, 0, LV_TABLE_PART_CELL2);
				lv_table_set_cell_type(obj, choosed_Row+1, 1, LV_TABLE_PART_CELL2);


				int quantity = GUI_SortedProd[choosed_Group].products[choosed_Row].quantity;

				lv_obj_set_hidden(det_page, false);

				lv_table_set_row_cnt(det_tab, quantity+1);
				lv_table_set_cell_value(det_tab, 0, 0, STR_TILES_TAB_COL_TERM);
				lv_table_set_cell_align(det_tab, 0, 0, LV_LABEL_ALIGN_CENTER);

				for(int i=0; i < quantity; i++){
					GUI_Data_ProductDetails * details = &GUI_SortedProd[choosed_Group].products[choosed_Row].product[i];
					char term[11];
					sprintf(term, "%02d.%02d.%d", details->day, details->month, details->year);
					lv_table_set_cell_value(det_tab, i+1, 0, term);

					if(details->redTerm)
					{
						lv_table_set_cell_type(det_tab, i+1, 0, LV_TABLE_PART_CELL2);
						continue;
					}
					if(details->yellowTerm)
					{
						lv_table_set_cell_type(det_tab, i+1, 0, LV_TABLE_PART_CELL3);
						continue;
					}
					lv_table_set_cell_type(det_tab, i+1, 0, LV_TABLE_PART_CELL4);
				}


				// reveal term table
				if(tils_group[group].selected_row == -1){
					lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t) anim_start_shift);
					lv_anim_set_var(&anim, det_page);
					lv_anim_set_time(&anim, 1200);
					lv_anim_set_values(&anim, 1, 180);
					lv_anim_start(&anim);
				}

				tils_group[group].selected_row = choosed_Row;

			}

		}
	}
}

static void termstable_evt_handler(lv_obj_t * obj, lv_event_t event)
{
	GUI_IntData_ExtinctionHandler();

	if(event == LV_EVENT_LONG_PRESSED && GUI_SafeMode)
	{
		lv_coord_t temp, tile;
		uint16_t tmp;
		lv_tileview_get_tile_act(tileview, &temp , &tile);
		choosed_Group = icon_btns[tile].category;
		if(lv_table_get_pressed_cell(obj, &choosedTerm, &tmp) == LV_RES_OK)
		{
			if(choosedTerm == 0) return;

			choosedTerm--;
			openMsgBox = true;
			lv_obj_t * bkground = lv_obj_create(parent ,NULL);
			lv_obj_reset_style_list(bkground, LV_OBJ_PART_MAIN);
			lv_obj_set_style_local_bg_color(bkground, 0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
			lv_obj_set_pos(bkground, 0, 0);
			lv_obj_set_size(bkground, LV_HOR_RES, LV_VER_RES);
			lv_obj_set_style_local_bg_opa(bkground, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
			lv_obj_align(bkground, NULL, LV_ALIGN_CENTER, 0, 0);

			char buff[100];

			snprintf(buff, 100, "%s %s %02d.%02d.%d?", STR_TILES_MSGBTN_SMODE_TITLE,
					GUI_SortedProd[choosed_Group].products[choosed_Row].name,
					GUI_SortedProd[choosed_Group].products[choosed_Row].product[choosedTerm].day,
					GUI_SortedProd[choosed_Group].products[choosed_Row].product[choosedTerm].month,
					GUI_SortedProd[choosed_Group].products[choosed_Row].product[choosedTerm].year);

			delProdMsgBox = lv_msgbox_create(bkground, NULL);
			lv_obj_align(delProdMsgBox, NULL, LV_ALIGN_CENTER, 0, -30);
			lv_obj_set_event_cb(delProdMsgBox, msgBox_event_handler);
			lv_obj_add_style(delProdMsgBox, LV_MSGBOX_PART_BG, &style_borders);
			lv_obj_add_style(delProdMsgBox, LV_MSGBOX_PART_BTN_BG, &style_font20);
			lv_msgbox_add_btns(delProdMsgBox, labelsSafeModeMsgBox);
			lv_msgbox_set_text(delProdMsgBox, buff);
		}
	}
}

static void setIcon(int j, GUI_DATA_CATEGORIES cat, int align_x, int align_y, const lv_img_dsc_t * img){
	icon_btns[j].icon = lv_imgbtn_create(menu, NULL);
	lv_obj_set_pos(icon_btns[j].icon, align_x, align_y);
	lv_obj_set_event_cb(icon_btns[j].icon, icon_btn_evt_handler);
	lv_imgbtn_set_src(icon_btns[j].icon, LV_BTN_STATE_RELEASED , img);
	lv_imgbtn_set_src(icon_btns[j].icon, LV_BTN_STATE_PRESSED , img);
	icon_btns[j].category = cat;
}

static void setValidPos(void){
	for(int i = 1; i <= groups; i++){
		valid_pos[i].x = 1;
		valid_pos[i].y = i - 1;
		valid_pos[i + groups].x = 0;
		valid_pos[i + groups].y = i;
	}

	valid_pos[0].x = 0;
	valid_pos[0].y = 0;
}

static void setIconBtns(void){

	int j = 0;
	int align_x = 30;
	int align_y = 30;
	for(int i = 0; i < NUMBER_CATEGORIES; i++){
		if(align_x >= 480){
			align_y += 100;
			align_x = 30;
		}
		if(GUI_SortedProd[i].count > 0)
		{
			GUI_DATA_CATEGORIES cat = i;
			switch(i)
			{

			case 0:
				setIcon(j, cat, align_x, align_y, &dairy);
				j++;
				break;
			case 1:
				setIcon(j, cat, align_x, align_y, &drinks);
				j++;
				break;
			case 2:
				setIcon(j, cat, align_x, align_y, &alcohols);
				j++;
				break;
			case 3:
				setIcon(j, cat, align_x, align_y, &sauces);
				j++;
				break;
			case 4:
				setIcon(j, cat, align_x, align_y, &sweets);
				j++;
				break;
			case 5:
				setIcon(j, cat, align_x, align_y, &fish);
				j++;
				break;
			case 6:
				setIcon(j, cat, align_x, align_y, &meat);
				j++;
				break;
			case 7:
				setIcon(j, cat, align_x, align_y, &frozen);
				j++;
				break;
			case 8:
				setIcon(j, cat, align_x, align_y, &fruitvege);
				j++;
				break;
			case 9:
				setIcon(j, cat, align_x, align_y, &klepsydra);
				j++;
				break;
			default:
				continue;
			}
			align_x += 90;
		}
	}
}

static void setTileCol(void){

	menu = lv_obj_create(tileview, NULL);
	lv_obj_set_size(menu, LV_HOR_RES, LV_VER_RES - 40);
	lv_obj_add_style(menu, LV_TABLE_PART_BG, &style_borders);
	lv_tileview_add_element(tileview, menu);

	if(groups == 0)
	{
		lv_obj_t * label_status = lv_label_create(menu, NULL);
		lv_label_set_text(label_status, STR_TILES_EMPTY_GROUPS);
		lv_obj_align(label_status, NULL, LV_ALIGN_CENTER, 0, 0);
		lv_obj_add_style(label_status, LV_LABEL_PART_MAIN, &style_font20);
		return;
	}

	setIconBtns();
	int axis_y_shift = 0;
	for(int i = 0; i<groups; i++){
		lv_obj_t * a  = lv_page_create(tileview, NULL);
		tils_group[i].tile = a;
		tils_group[i].selected_row = -1;

		lv_obj_set_size(tils_group[i].tile, LV_HOR_RES, LV_VER_RES - 40);
		lv_obj_set_pos(tils_group[i].tile, LV_HOR_RES, axis_y_shift);
		lv_tileview_add_element(tileview, tils_group[i].tile);
		axis_y_shift += LV_VER_RES - 40;
		lv_obj_add_style(tils_group[i].tile, LV_PAGE_PART_BG, &style_borders);
		lv_page_set_scrlbar_mode(tils_group[i].tile, LV_SCROLLBAR_MODE_OFF);
		lv_page_set_scroll_propagation(tils_group[i].tile, true);
	}
	for(int i = 0; i < groups; i++){
		GUI_DATA_CATEGORIES category = icon_btns[i].category;
		uint16_t lines = GUI_SortedProd[category].count;
		if(!lines)
			continue;
		lines++; // additional row for title col
		lv_obj_t * page_t = lv_page_create(tils_group[i].tile , NULL);
		lv_obj_set_size(page_t, 460, LV_VER_RES - 70);
		lv_obj_add_style(page_t, LV_PAGE_PART_BG, &style_borders);
		lv_page_set_scrlbar_mode(page_t, LV_SCROLLBAR_MODE_OFF);
		lv_page_set_scroll_propagation(page_t, true);
		lv_obj_set_drag_parent(page_t, true);

		lv_obj_t * table = lv_table_create(page_t, NULL);
		lv_obj_set_event_cb(table, table_evt_handler);
		lv_obj_add_style(table, LV_TABLE_PART_BG, &style_borders);
		lv_table_set_col_cnt(table, 2);
		lv_table_set_row_cnt(table, lines);
		lv_table_set_col_width(table, 0, 350);
		lv_table_set_col_width(table, 1, 75);
		lv_obj_align(table, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
		lv_obj_set_drag_parent(table, true);
		lv_obj_set_parent_event(table, true);

		for(int j = 0; j < lines-1; j++){
			char str[5];
			sprintf(str,"%d",GUI_SortedProd[category].products[j].quantity);
			lv_table_set_cell_align(table, j+1, 1, LV_LABEL_ALIGN_CENTER);
			lv_table_set_cell_value(table, j+1, 1, str);
			lv_table_set_cell_value(table, j+1, 0, GUI_SortedProd[category].products[j].name);
		}
		lv_obj_add_style(table, LV_TABLE_PART_CELL2, &style_tab);
		lv_obj_add_style(table, LV_TABLE_PART_CELL1, &style_font20);

		lv_table_set_cell_value(table, 0, 0, STR_TILES_TAB_COL_NAME);
		lv_table_set_cell_value(table, 0, 1, STR_TILES_TAB_COL_COUNT);
		lv_table_set_cell_align(table, 0, 0, LV_LABEL_ALIGN_CENTER);

		lv_obj_t * detail_page = lv_page_create(tils_group[i].tile, NULL);
		lv_obj_align(detail_page, NULL, LV_ALIGN_IN_TOP_MID, 270, 0);
		lv_obj_set_size(detail_page, 160, LV_VER_RES - 120);
		lv_obj_add_style(detail_page, LV_PAGE_PART_BG, &style_borders);
		lv_page_set_scroll_propagation(detail_page, true);
		lv_page_set_scrlbar_mode(detail_page, LV_SCROLLBAR_MODE_OFF);
		lv_obj_set_hidden(detail_page, true);

		lv_obj_t * det_tab = lv_table_create(detail_page, NULL);
		lv_obj_add_style(det_tab, LV_TABLE_PART_BG, &style_borders);
		lv_obj_add_style(det_tab, LV_TABLE_PART_CELL2, &style_redterm);
		lv_obj_add_style(det_tab, LV_TABLE_PART_CELL3, &style_yellowterm);
		lv_obj_add_style(det_tab, LV_TABLE_PART_CELL4, &style_greenterm);
		lv_obj_add_style(det_tab, LV_TABLE_PART_CELL1, &style_font20);

		lv_table_set_col_cnt(det_tab, 1);
		lv_table_set_col_width(det_tab, 0, 120);
		lv_obj_set_drag_parent(det_tab, true);
		lv_obj_set_event_cb(det_tab, termstable_evt_handler);

		if(category == DATA_END_OF_EXPIRY)
		{
			lv_obj_align(page_t, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 20);
			lv_obj_align(detail_page, NULL, LV_ALIGN_IN_TOP_MID, 250, 20);

			lv_obj_t * label_status = lv_label_create(tils_group[i].tile, NULL);
			char buf[20];
			snprintf(buf, 20, "%s %d", STR_SETTINGS_DAYS_TO_END, GUI_TermRange);
			lv_label_set_text(label_status, buf);
			lv_obj_align(label_status, tils_group[i].tile, LV_ALIGN_IN_TOP_MID, 0, 0);
			lv_obj_add_style(label_status, LV_LABEL_PART_MAIN, &style_font20);
		}
	}
}

