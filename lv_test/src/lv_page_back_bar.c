

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static void event_cb(lv_event_t * e)
{
    // LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t * target = lv_event_get_current_target(e); // lv_event_get_target(e);
    lv_page_back_previous_page();
    // lv_page_back_top_page();
}

lv_obj_t * lv_page_back_bar_init(lv_obj_t * parent, const char * name, const char * title)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t * back_bar = lv_obj_create(parent);
    lv_obj_set_size(back_bar, LV_PCT(100), 55);
    lv_obj_set_align(back_bar, LV_ALIGN_TOP_MID);

    lv_obj_t * back_button =
        lv_custom_image_button_create(back_bar, themesImagesPath "back_button_background.png", 60, LV_PCT(100), 0, 0);

    lv_obj_add_event_cb(back_button, event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * back_line = lv_obj_create(back_bar);
    lv_obj_set_size(back_line, LV_PCT(100), 1);
    lv_obj_set_align(back_line, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_bg_color(back_line, lv_color_hex(0x3A3A3A), 0);
    lv_obj_set_style_bg_opa(back_line, LV_OPA_100, 0);

    if(name != NULL) {
        lv_obj_t * label_name = lv_label_create(back_bar);
        lv_obj_set_style_text_font(label_name, &lv_font_SiYuanHeiTi_Normal_24, 0);
        lv_label_set_text(label_name, name);
        lv_obj_set_style_text_color(label_name, lv_color_hex(themesTextColor2), 0);
        lv_obj_align_to(label_name, back_button, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    }
    if(title != NULL) {
        lv_obj_t * label_title = lv_label_create(back_bar);
        lv_obj_set_style_text_font(label_title, &lv_font_SiYuanHeiTi_Normal_30, 0);
        lv_label_set_text(label_title, title);
        lv_obj_set_style_text_color(label_title, lv_color_hex(themesTextColor2), 0);
        lv_obj_align_to(label_title, back_button, LV_ALIGN_CENTER, 0, 0);
    }
    return back_bar;
}
