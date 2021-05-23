/*
 * intro.c
 *
 *  Created on: Dec 22, 2020
 *      Author: Kamil Wielgosz
 */

#include <conf_internal/styles.h>
#include "intro.h"
#include "main_menu.h"

LV_IMG_DECLARE(KI)

static lv_obj_t * screen;

static void intro_anim(void * var, lv_coord_t x){
	if(x == 500){
		MAIN_MENU_Show();
		return;
	}else if(x > 50)
		return;
	//lv_obj_set_y(var, x);
}

void INTRO_StartIntro(void){
	static lv_style_t blue_bg;
	static lv_style_t ki_s;
	static lv_anim_t anim;
	lv_style_init(&blue_bg);
	lv_style_set_bg_color(&blue_bg, LV_STATE_DEFAULT, lv_color_hex(0x006080));

	lv_style_set_text_font(&ki_s, LV_STATE_DEFAULT, &font_pl_20);
	lv_style_set_text_color(&ki_s, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	screen = lv_obj_create(NULL, NULL);
	lv_obj_add_style(screen, LV_OBJ_PART_MAIN, &blue_bg);


	lv_obj_t *label1 = lv_label_create(screen, NULL);
	lv_obj_t *ki = lv_label_create(screen, NULL);
	lv_obj_add_style(label1, LV_STATE_DEFAULT, &style_timelabel);
	lv_obj_add_style(ki, LV_STATE_DEFAULT, &ki_s);
	lv_obj_align(label1, NULL, LV_ALIGN_IN_BOTTOM_MID, -65, -40);
	lv_obj_align(ki, NULL, LV_ALIGN_IN_BOTTOM_MID, -75, -20);
	lv_label_set_text(label1, STR_INTRO_SCHOOL);
	lv_label_set_text(ki, STR_INTRO_DESC);
	lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);
	lv_label_set_align(ki, LV_LABEL_ALIGN_CENTER);

	lv_obj_t *img = lv_img_create(screen, NULL);
	lv_obj_set_pos(img, LV_HOR_RES_MAX/2 - 70, 50);
	lv_img_set_src(img, &KI);

	lv_anim_init(&anim);

	lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t) intro_anim);
	lv_anim_set_var(&anim, img);
	lv_anim_set_time(&anim, 3000);
	lv_anim_set_values(&anim, -150, 500);


	lv_scr_load(screen);
	lv_anim_start(&anim);
}
