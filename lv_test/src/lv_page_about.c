

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"
/*********************
 *      DEFINES
 *********************/
static lv_obj_t *about_list;
/**********************
 *  STATIC VARIABLES
 **********************/
static void about_bind_event_cb(lv_event_t *e)
{
    lv_obj_t *current_target = lv_event_get_current_target(e);
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d current_target:%p target:%p\n", __func__, e->code);
    switch (user_data)
    {
    case 0:
        break;
    }
    lv_obj_clean(lv_layer_top());
}
static lv_obj_t *lv_about_bind_dialog(const char *topText, const char *centerText)
{
    lv_obj_t *obj = lv_obj_create(lv_layer_top());
    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(obj, LV_OPA_60, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000), 0);

    lv_obj_t *popup = lv_obj_create(obj);
    lv_obj_set_size(popup, 730, 350);
    lv_obj_set_style_bg_opa(popup, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(popup, lv_color_hex(themesPopupWindowColor), 0);
    lv_obj_center(popup);

    lv_obj_t *close_obj = lv_obj_create(popup);
    lv_obj_set_size(close_obj, 80, 80);
    lv_obj_align(close_obj, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_event_cb(close_obj, about_bind_event_cb, LV_EVENT_CLICKED, (void *)0);
    lv_obj_t *close_img = lv_img_create(close_obj);
    lv_obj_align(close_img, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(close_img, themesImagesPath "icon_window_close.png");

    lv_obj_t *label = lv_label_create(popup);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, topText);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 25);

    lv_obj_t *qrcode_img = lv_img_create(popup);
    lv_obj_align(qrcode_img, LV_ALIGN_CENTER, -150, 10);
    lv_img_set_src(qrcode_img, recipesImagesPath "QrCode.png");

    label = lv_label_create(popup);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, centerText);
    lv_obj_align(label, LV_ALIGN_CENTER, 150, 10);
    return obj;
}

static void about_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *current_target = lv_event_get_current_target(e);
    lv_obj_t *target = lv_event_get_target(e);
    // const char *user_data = lv_event_get_user_data(e);
    int index = lv_obj_get_index(target);
    LV_LOG_USER("%s,code:%d index:%d\n", __func__, e->code, index);
    if (code == LV_EVENT_CLICKED)
    {
        if (index == 3)
        {
            lv_about_bind_dialog("??????APP", "????????????APP   ????????????\n??????????????????  ????????????");
        }
        else if (index == 4)
        {
        }
    }
}

lv_obj_t *lv_about_list_create(const char *key, const char *value)
{
    lv_obj_t *obj = lv_obj_create(about_list);
    lv_obj_set_size(obj, LV_PCT(100), 65);

    lv_obj_t *label = lv_label_create(obj);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, key);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, 0);

    label = lv_label_create(obj);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, value);
    lv_obj_align(label, LV_ALIGN_RIGHT_MID, -20, 0);

    lv_obj_t *divider = lv_divider_create(obj);
    lv_obj_align(divider, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_add_event_cb(obj, about_event_handler, LV_EVENT_LONG_PRESSED, NULL);
    return obj;
}

lv_obj_t *lv_about2_list_create(const char *key)
{
    lv_obj_t *obj = lv_btn_create(about_list);
    lv_obj_set_size(obj, LV_PCT(100), 70);

    lv_obj_t *label = lv_label_create(obj);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, key);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, 0);

    lv_obj_t *img = lv_img_create(obj);
    lv_img_set_src(img, themesImagesPath "icon_more.png");
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -20, 0);

    lv_obj_t *divider = lv_divider_create(obj);
    lv_obj_align(divider, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_add_event_cb(obj, about_event_handler, LV_EVENT_CLICKED, NULL);
    return obj;
}
void lv_page_about_create(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);

    about_list = lv_list_create(page);
    lv_obj_set_size(about_list, LV_PCT(100), LV_PCT(100));
    lv_obj_align(about_list, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_about_list_create("????????????:", "?????????");
    lv_about_list_create("????????????:", "IIZ(T/Y)X8GCZ01");
    lv_about_list_create("??????ID:", "12345678");
    lv_about2_list_create("????????????APP");
    lv_about2_list_create("????????????");
}
