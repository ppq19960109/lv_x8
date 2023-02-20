

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
    // lv_obj_t * target      = lv_event_get_target(e);
    char * user_data = lv_event_get_user_data(e);
    lv_page_back_replace_page(NULL, user_data);
}

static lv_obj_t * lv_text_btn_create(lv_obj_t * parent, const char * text, char * user_data, const int checkd)
{
    lv_obj_t * btn1 = lv_btn_create(parent);
    lv_obj_set_size(btn1, 150, LV_PCT(100));
    lv_obj_add_event_cb(btn1, event_cb, LV_EVENT_CLICKED, user_data);

    lv_obj_t * label = lv_label_create(btn1);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_24, 0);
    if(checkd)
        lv_obj_set_style_text_color(label, lv_color_hex(themesTextColor), 0);
    else {
        lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    }
    lv_label_set_text(label, text);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 3);
    if(checkd) {
        lv_obj_t * navigation_bar = lv_img_create(btn1);
        lv_img_set_src(navigation_bar, themesImagesPath "navigation_bar_text_background.png");
        lv_obj_align(navigation_bar, LV_ALIGN_CENTER, 0, 0);
    }
    return btn1;
}
lv_obj_t * lv_page_top_bar_init(lv_obj_t * parent, const int index)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t * top_bar = lv_img_create(parent);
    lv_img_set_src(top_bar, themesImagesPath "navigation_bar.png");
    lv_obj_align(top_bar, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t * cont_row = lv_obj_create(top_bar);
    lv_obj_set_size(cont_row, 470, 48);
    lv_obj_align(cont_row, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_AROUND);

    lv_text_btn_create(cont_row, "烟机灶具", "page_hood", index == 0);
    lv_text_btn_create(cont_row, "蒸烤箱", "page_steamoven", index == 1);
    lv_text_btn_create(cont_row, "智慧菜谱", "page_smartrecipes", index == 2);
    return top_bar;
}
