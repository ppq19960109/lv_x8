

/*********************
 *      INCLUDES
 *********************/
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC VARIABLES
 **********************/
lv_obj_t *get_auto_layer()
{
    return lv_layer_top();
}
void clean_auto_layer()
{
    lv_obj_clean(lv_layer_top());
}
lv_obj_t *lv_dialog1(lv_obj_t *layer, const char *content, const char *cancel, const char *confirm, lv_event_cb_t event_cb)
{
    lv_obj_set_style_bg_opa(layer, LV_OPA_60, 0);
    lv_obj_set_style_bg_color(layer, lv_color_hex(0x00), 0);

    lv_obj_t *obj = lv_obj_create(layer);
    lv_obj_set_size(obj, 730, 350);
    lv_obj_center(obj);
    lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(themesPopupWindowColor), 0);
    lv_obj_set_style_radius(obj, 10, 0);

    lv_obj_t *close_obj = lv_obj_create(obj);
    lv_obj_set_size(close_obj, 80, 80);
    lv_obj_align(close_obj, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_event_cb(close_obj, event_cb, LV_EVENT_CLICKED, (void *)0);
    lv_obj_t *close_img = lv_img_create(close_obj);
    lv_obj_align(close_img, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(close_img, themesImagesPath "icon_window_close.png");

    lv_obj_t *icon_img = lv_img_create(obj);
    lv_obj_align(icon_img, LV_ALIGN_TOP_MID, 0, 55);
    lv_img_set_src(icon_img, themesImagesPath "icon_warn.png");

    lv_obj_t *label_content = lv_label_create(obj);
    lv_obj_set_style_text_font(label_content, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_content, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_content, content);
    lv_obj_align(label_content, LV_ALIGN_CENTER, 0, 0);
    if (cancel != NULL)
    {
        lv_obj_t *btn1 = lv_custom_text_btn_create(obj, cancel);
        lv_obj_add_event_cb(btn1, event_cb, LV_EVENT_CLICKED, (void *)1);
        lv_obj_align(btn1, LV_ALIGN_BOTTOM_MID, -130, -25);
    }

    lv_obj_t *btn2 = lv_custom_text_btn_create(obj, confirm);
    lv_obj_add_event_cb(btn2, event_cb, LV_EVENT_CLICKED, (void *)2);
    if (cancel != NULL)
        lv_obj_align(btn2, LV_ALIGN_BOTTOM_MID, 130, -25);
    else
        lv_obj_align(btn2, LV_ALIGN_BOTTOM_MID, 0, -25);

    return obj;
}
lv_obj_t *lv_auto_dialog1(const char *content, const char *cancel, const char *confirm, lv_event_cb_t event_cb)
{
    clean_auto_layer();
    lv_obj_t *layer = get_auto_layer();
    return lv_dialog1(layer, content, cancel, confirm, event_cb);
}