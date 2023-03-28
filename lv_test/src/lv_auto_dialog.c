

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
lv_obj_t *get_auto_layer(void)
{
    return lv_layer_top();
}
void clean_auto_layer(void)
{
    lv_obj_t *layer = lv_layer_top();
    if ((long)layer->user_data > 0)
    {
        layer->user_data = (void *)0;
    }
    lv_obj_clean(layer);
}
lv_obj_t *lv_dialog1(lv_obj_t *layer, const char *content, const char *cancel, const char *confirm, const char *top_img, const char *top_text, lv_event_cb_t event_cb)
{
    layer->user_data = (void *)AUTO_INDEX_GENERAL;
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

    if (top_text == NULL)
    {
        lv_obj_t *icon_img = lv_img_create(obj);
        lv_obj_align(icon_img, LV_ALIGN_TOP_MID, 0, 55);
        if (top_img == NULL)
            lv_img_set_src(icon_img, themesImagesPath "icon_warn.png");
        else
            lv_img_set_src(icon_img, top_img);
    }
    else
    {
        lv_obj_t *label_top = lv_label_create(obj);
        lv_obj_set_style_text_font(label_top, &lv_font_SiYuanHeiTi_Normal_30, 0);
        lv_obj_set_style_text_color(label_top, lv_color_hex(0xffffff), 0);
        lv_label_set_text(label_top, top_text);
        lv_obj_align(label_top, LV_ALIGN_TOP_MID, 0, 55);
    }

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
    return lv_dialog1(layer, content, cancel, confirm, NULL, NULL, event_cb);
}
static void hoodOff_dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    long user_data = (long)lv_event_get_user_data(e);
    if (user_data == 2)
        set_num_toServer("HoodSpeed", 0);
    clean_auto_layer();
}
lv_obj_t *lv_auto_dialog3(void)
{
    clean_auto_layer();
    lv_obj_t *layer = get_auto_layer();
    lv_obj_t *obj = lv_auto_dialog1("烟机自动延时#E68855 3分钟#\n后关闭,清除余烟", "好的", "立即关闭#E68855 3分钟#", hoodOff_dialog_event_cb);
    lv_obj_t *confirm = lv_obj_get_child(obj, -1);
    lv_obj_set_width(confirm, 280);
    layer->user_data = (void *)AUTO_INDEX_HOODOFF;
    return obj;
}
void lv_auto_dialog3_update(const char *content, const char *confirm)
{
    lv_obj_t *layer = get_auto_layer();
    if ((long)layer->user_data != AUTO_INDEX_HOODOFF)
        return;
    lv_obj_t *obj = lv_obj_get_child(lv_obj_get_child(layer, 0), 0);
    lv_obj_t *content_text = lv_obj_get_child(obj, 2);
    lv_label_set_text(content_text, content);
    lv_obj_t *confirm_text = lv_obj_get_child(lv_obj_get_child(obj, -1), 0);
    lv_label_set_text(confirm_text, confirm);
}
void lv_auto_dialog3_close(void)
{
    lv_obj_t *layer = get_auto_layer();
    if ((long)layer->user_data != AUTO_INDEX_HOODOFF)
        return;
    clean_auto_layer();
}
static lv_obj_t *lv_screen_saver_dialog(lv_obj_t *parent)
{
    lv_obj_t *screen_img = lv_img_create(parent);
    lv_img_set_src(screen_img, themesImagesPath "screen_saver1.png");
    return screen_img;
}
void lv_auto_screen_dialog4(const char index)
{
    clean_auto_layer();
    lv_obj_t *layer = get_auto_layer();
    lv_screen_saver_dialog(layer);
    layer->user_data = (void *)AUTO_INDEX_SCREEN;
    lv_obj_add_flag(layer, LV_OBJ_FLAG_CLICKABLE);
}
void lv_auto_screen_dialog4_close(void)
{
    lv_obj_t *layer = get_auto_layer();
    if ((long)layer->user_data != AUTO_INDEX_SCREEN)
        return;
    clean_auto_layer();
    lv_obj_clear_flag(layer, LV_OBJ_FLAG_CLICKABLE);
}
static void set_angle(void *img, int32_t v)
{
    lv_img_set_angle(img, v);
}
void lv_auto_upgrade_dialog5(const char *title)
{
    clean_auto_layer();
    lv_obj_t *layer = get_auto_layer();
    if ((long)layer->user_data == AUTO_INDEX_UPGRADE)
        return;
    layer->user_data = (void *)AUTO_INDEX_UPGRADE;

    lv_obj_t *bg = lv_obj_create(layer);
    lv_obj_set_size(bg, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(bg, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(bg, lv_color_hex(themesPopupWindowColor), 0);

    lv_obj_t *label = lv_label_create(bg);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_50, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(themesTextColor), 0);
    char buf[40];
    sprintf(buf, "%s升级中,请勿断电!", title);
    lv_label_set_text(label, buf);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 50);

    lv_obj_t *img = lv_img_create(bg);
    lv_img_set_src(img, themesImagesPath "icon_loading_big.png");
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 155);
    lv_obj_update_layout(img);
    lv_img_set_pivot(img, lv_obj_get_width(img) / 2, lv_obj_get_height(img) / 2);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, img);
    lv_anim_set_exec_cb(&a, set_angle);
    lv_anim_set_values(&a, 0, 3600);
    lv_anim_set_time(&a, 8000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);

    label = lv_label_create(bg);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, "0%%");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 320);

    lv_obj_t *progress_bar = lv_bar_create(bg);
    lv_obj_set_size(progress_bar, 650, 15);
    lv_obj_align(progress_bar, LV_ALIGN_CENTER, 0, 0);
    lv_bar_set_range(progress_bar, 0, 100);
    lv_bar_set_value(progress_bar, 0, LV_ANIM_OFF);

    lv_obj_add_style(progress_bar, &slider_style_main, LV_PART_MAIN);
    lv_obj_add_style(progress_bar, &slider_style_indicator, LV_PART_INDICATOR);
    lv_obj_align(progress_bar, LV_ALIGN_BOTTOM_MID, 0, -100);
}
void lv_auto_upgrade_dialog5_close(void)
{
    lv_obj_t *layer = get_auto_layer();
    if ((long)layer->user_data == AUTO_INDEX_UPGRADE)
    {
        clean_auto_layer();
    }
}
void lv_auto_upgrade_dialog5_progress(const int progress, const char *title)
{
    lv_obj_t *layer = get_auto_layer();
    if ((long)layer->user_data != AUTO_INDEX_UPGRADE)
    {
        lv_auto_upgrade_dialog5(title);
    }
    lv_obj_t *bg = lv_obj_get_child(layer, 0);
    lv_obj_t *progress_bar = lv_obj_get_child(bg, -1);
    lv_obj_t *progress_label = lv_obj_get_child(bg, -2);
    char buf[8];
    sprintf(buf, "%d%%", progress);
    lv_label_set_text(progress_label, buf);
    lv_bar_set_value(progress_bar, progress, LV_ANIM_OFF);
}
static void upgrade_confirm_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    long user_data = (long)lv_event_get_user_data(e);
    if (user_data == 2)
    {
        
    }
    clean_auto_layer();
}
void lv_auto_upgrade_confirm_dialog6(const char *version)
{
    clean_auto_layer();
    lv_obj_t *layer = get_auto_layer();
    layer->user_data = (void *)AUTO_INDEX_UPGRADE_CONFIRM;
    char buf[120];
    sprintf(buf, "检查到最新版本V%s\n请问是否立即更新?", version);
    lv_dialog1(layer, buf, "取消", "升级", NULL, "系统更新", upgrade_confirm_event_cb);
}
void lv_auto_upgrade_confirm_dialog6_close(void)
{
    lv_obj_t *layer = get_auto_layer();
    if ((long)layer->user_data == AUTO_INDEX_UPGRADE_CONFIRM)
    {
        clean_auto_layer();
    }
}
