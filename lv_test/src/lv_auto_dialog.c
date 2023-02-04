

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
    lv_obj_t *layer = lv_layer_top();
    if (layer->user_data > 0)
    {
        layer->user_data = 0;
    }
    lv_obj_clean(layer);
}
lv_obj_t *lv_dialog1(lv_obj_t *layer, const char *content, const char *cancel, const char *confirm, const char *top_img, lv_event_cb_t event_cb)
{
    layer->user_data = 1;
    lv_obj_t *bg = lv_obj_create(layer);
    lv_obj_set_size(bg, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(bg, LV_OPA_60, 0);
    lv_obj_set_style_bg_color(bg, lv_color_hex(0x00), 0);

    lv_obj_t *obj = lv_obj_create(bg);
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
    if (top_img == NULL)
        lv_img_set_src(icon_img, themesImagesPath "icon_warn.png");
    else
        lv_img_set_src(icon_img, top_img);

    lv_obj_t *label_content = lv_label_create(obj);
    lv_obj_set_style_text_font(label_content, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_content, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_content, content);
    lv_obj_align(label_content, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_recolor(label_content, true);
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
static void auto_dialog_define_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    clean_auto_layer();
}
lv_obj_t *lv_auto_dialog1(const char *content, const char *cancel, const char *confirm, lv_event_cb_t event_cb)
{
    clean_auto_layer();
    lv_obj_t *layer = get_auto_layer();
    if (event_cb == NULL)
        event_cb = auto_dialog_define_event_cb;
    return lv_dialog1(layer, content, cancel, confirm, NULL, event_cb);
}
static void hoodOff_dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    int user_data = (int)lv_event_get_user_data(e);
    if (user_data == 2)
        set_num_toServer("HoodSpeed", 0);
    clean_auto_layer();
}
lv_obj_t *lv_auto_dialog3()
{
    clean_auto_layer();
    lv_obj_t *layer = get_auto_layer();
    lv_obj_t *obj = lv_auto_dialog1("烟机自动延时#E68855 3分钟#\n后关闭,清除余烟", "好的", "立即关闭#E68855 3分钟#", hoodOff_dialog_event_cb);
    lv_obj_t *confirm = lv_obj_get_child(obj, -1);
    lv_obj_set_width(confirm, 280);
    layer->user_data = 3;
    return obj;
}
void lv_auto_dialog3_update(const char *content, const char *confirm)
{
    lv_obj_t *layer = get_auto_layer();
    if (layer->user_data != 3)
        return;
    lv_obj_t *obj = lv_obj_get_child(lv_obj_get_child(layer, 0), 0);
    lv_obj_t *content_text = lv_obj_get_child(obj, 2);
    lv_label_set_text(content_text, content);
    lv_obj_t *confirm_text = lv_obj_get_child(lv_obj_get_child(obj, -1), 0);
    lv_label_set_text(confirm_text, confirm);
}
void lv_auto_dialog3_close()
{
    lv_obj_t *layer = get_auto_layer();
    if (layer->user_data != 3)
        return;
    clean_auto_layer();
}