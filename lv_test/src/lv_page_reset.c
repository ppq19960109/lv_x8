

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
static void btn_event_cb(lv_event_t *e)
{
    lv_obj_t *current_target = lv_event_get_current_target(e);
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    switch (user_data)
    {
    case 0:
        break;
    }
}
void lv_page_reset_create(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);

    lv_obj_t *img = lv_img_create(page);
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 50);
    lv_img_set_src(img, themesImagesPath "icon_warn.png");

    lv_obj_t *label = lv_label_create(page);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, "此操作会将设备初始化\n清除内部空间中的数据");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 120);

    lv_obj_t *btn = lv_btn_create(page);
    lv_obj_set_size(btn, 180, 50);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -40);
    lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, (void *)0);

    label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0), 0);
    lv_label_set_text(label, "重置设备");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}
