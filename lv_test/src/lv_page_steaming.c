

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

static lv_obj_t *lv_text_btn_create(lv_obj_t *parent, const char *text, const void *user_data)
{
    lv_obj_t *btn1 = lv_btn_create(parent);
    lv_obj_set_size(btn1, 90, 45);
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0x191919), 0);
    lv_obj_set_style_bg_color(btn1, lv_color_hex(themesTextColor), LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(btn1, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn1, 45, 0);
    // lv_obj_add_event_cb(btn1, hood_speed_event_cb, LV_EVENT_CLICKED, user_data);

    lv_obj_t *label = lv_label_create(btn1);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x7C7C7C), 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_CHECKED);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return btn1;
}

static lv_obj_t *lv_steam_item_create(lv_obj_t *parent, const char *text)
{
    lv_obj_t *item = lv_img_create(parent);
    lv_img_set_src(item, themesImagesPath "icon_steam_runing_background.png");

    lv_obj_t *add_img = lv_img_create(item);
    lv_img_set_src(add_img, themesImagesPath "icon_cook_add.png");
    lv_obj_align(add_img, LV_ALIGN_TOP_MID, 0, 86);

    lv_obj_t *label = lv_label_create(item);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(themesTextColor2), 0);
    lv_label_set_text(label, text);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 183);
    return item;
}

void lv_page_steaming_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t *back_bar = lv_page_back_bar_init(page, "蒸烤箱", NULL);

    lv_obj_t *cont_row = lv_obj_create(page);
    lv_obj_set_size(cont_row, 290 * 2 + 112, 290);
    lv_obj_align_to(cont_row, back_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_steam_item_create(cont_row,"左腔");
    lv_steam_item_create(cont_row,"右腔");
}
