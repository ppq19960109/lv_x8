

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
#if 1
lv_obj_t *get_manual_layer(void)
{
    return lv_scr_act();
}
void clean_manual_layer(void)
{
    lv_obj_t *layer = lv_scr_act();
    LV_LOG_USER("%s,child_cnt:%d user_data:%ld", __func__, lv_obj_get_child_cnt(layer), (long)layer->user_data);
    if ((long)layer->user_data > 0)
    {
        layer->user_data = (void *)0;
        lv_obj_del(lv_obj_get_child(layer, -1));
    }
}
#else
lv_obj_t *get_manual_layer(void)
{
    if (manual_scr == NULL)
        manual_scr = lv_obj_create(NULL);
    LV_LOG_USER("%s,manual_scr:%p main_scr:%p", __func__, manual_scr, main_scr);
    lv_scr_load(manual_scr);
    return manual_scr;
}
void clean_manual_layer(void)
{
    LV_LOG_USER("%s,manual_scr:%p main_scr:%p", __func__, manual_scr, main_scr);
    lv_scr_load(main_scr);
    lv_obj_clean(manual_scr);
}
#endif
static void manual_dialog_define_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    clean_manual_layer();
}
static void scroll_event_cb(lv_event_t *e)
{
    lv_obj_t *user_data = lv_event_get_user_data(e);
    if (e->code == LV_EVENT_PRESSED)
    {
        lv_obj_clear_flag(user_data, LV_OBJ_FLAG_HIDDEN);
    }
    else if (e->code == LV_EVENT_RELEASED)
    {
        lv_obj_add_flag(user_data, LV_OBJ_FLAG_HIDDEN);
    }
}
lv_obj_t *lv_manual_reserve_dialog(const char *text_before, const char *text_after, const char *text_confirm, const int hour, lv_event_cb_t event_cb)
{
    lv_obj_t *layer = get_manual_layer();
    layer->user_data = (void *)2;
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

    lv_obj_t *label_content = lv_label_create(obj);
    lv_obj_set_style_text_font(label_content, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_content, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_content, text_before);
    lv_obj_align(label_content, LV_ALIGN_TOP_LEFT, 32, 140);

    lv_obj_t *label_content2 = lv_label_create(obj);
    lv_obj_set_style_text_font(label_content2, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_content2, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_content2, text_after);
    lv_obj_align(label_content2, LV_ALIGN_TOP_RIGHT, -45, 140);

    lv_obj_t *time_obj = lv_obj_create(obj);
    lv_obj_set_size(time_obj, 200, 222);
    lv_obj_align(time_obj, LV_ALIGN_TOP_LEFT, 170, 80);
    lv_obj_t *time_img = lv_img_create(time_obj);
    lv_img_set_src(time_img, themesImagesPath "steamoven/roll_background.png");
    lv_obj_align(time_img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(time_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_t *time_label = lv_label_create(time_obj);
    lv_obj_set_style_text_font(time_label, &lv_font_SiYuanHeiTi_Normal_34, 0);
    lv_obj_set_style_text_color(time_label, lv_color_hex(themesTextColor), 0);
    lv_label_set_text(time_label, "小时");
    lv_obj_align(time_label, LV_ALIGN_CENTER, 60, -35);

    lv_obj_t *minute_time_obj = lv_obj_create(obj);
    lv_obj_set_size(minute_time_obj, 200, 222);
    lv_obj_align(minute_time_obj, LV_ALIGN_TOP_LEFT, 380, 80);
    lv_obj_t *minute_time_img = lv_img_create(minute_time_obj);
    lv_img_set_src(minute_time_img, themesImagesPath "steamoven/roll_background.png");
    lv_obj_align(minute_time_img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(minute_time_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_t *minute_time_label = lv_label_create(minute_time_obj);
    lv_obj_set_style_text_font(minute_time_label, &lv_font_SiYuanHeiTi_Normal_34, 0);
    lv_obj_set_style_text_color(minute_time_label, lv_color_hex(themesTextColor), 0);
    lv_label_set_text(minute_time_label, "分钟");
    lv_obj_align(minute_time_label, LV_ALIGN_CENTER, 60, -35);

    lv_obj_t *roller1 = lv_roller_create(time_obj);
    lv_obj_add_style(roller1, &roller_style_unselected, 0);
    lv_obj_add_style(roller1, &roller_style_selected, LV_PART_SELECTED);
    lv_obj_set_size(roller1, LV_PCT(100), LV_PCT(100));
    lv_roller_set_visible_row_count(roller1, 3);
    lv_obj_add_event_cb(roller1, scroll_event_cb, LV_EVENT_ALL, time_img);
    const char *options = temp_roller_options(0, hour);
    lv_roller_set_options(roller1,
                          options,
                          LV_ROLLER_MODE_INFINITE);
    lv_roller_set_selected(roller1, 0, LV_ANIM_ON);

    lv_obj_t *roller3 = lv_roller_create(minute_time_obj);
    lv_obj_set_size(roller3, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(roller3, &roller_style_unselected, 0);
    lv_obj_add_style(roller3, &roller_style_selected, LV_PART_SELECTED);
    lv_roller_set_visible_row_count(roller3, 3);
    lv_obj_add_event_cb(roller3, scroll_event_cb, LV_EVENT_ALL, minute_time_img);
    options = temp_roller_options(0, 59);
    lv_roller_set_options(roller3,
                          options,
                          LV_ROLLER_MODE_INFINITE);
    lv_roller_set_selected(roller3, 0, LV_ANIM_ON);

    lv_obj_t *btn1 = lv_custom_text_btn_create(obj, "取消");
    lv_obj_add_event_cb(btn1, event_cb, LV_EVENT_CLICKED, (void *)1);
    lv_obj_align(btn1, LV_ALIGN_BOTTOM_MID, -130, -25);

    lv_obj_t *btn2 = lv_custom_text_btn_create(obj, text_confirm);
    lv_obj_add_event_cb(btn2, event_cb, LV_EVENT_CLICKED, (void *)2);
    lv_obj_align(btn2, LV_ALIGN_BOTTOM_MID, 130, -25);
    return obj;
}
int lv_get_reserve_dialog_time(void)
{
    lv_obj_t *layer = lv_scr_act();
    if ((long)layer->user_data != 2)
        return -1;
    lv_obj_t *reserve_dialog = lv_obj_get_child(layer, -1);
    lv_obj_t *hour_roller = lv_obj_get_child(lv_obj_get_child(reserve_dialog, -4), -1);
    lv_obj_t *minute_roller = lv_obj_get_child(lv_obj_get_child(reserve_dialog, -3), -1);
    unsigned char hour_time_index = lv_roller_get_selected(hour_roller);
    unsigned char minute_time_index = lv_roller_get_selected(minute_roller);
    return hour_time_index * 60 + minute_time_index;
}

lv_obj_t *lv_manual_cook_dialog(const char *content, lv_event_cb_t event_cb)
{
    lv_obj_t *layer = get_manual_layer();
    lv_obj_t *obj = lv_dialog1(layer, content, "取消", "开始", NULL, event_cb);
    lv_obj_t *radio_btn = radiobutton_create(obj, "下次不再提示");
    lv_obj_align(radio_btn, LV_ALIGN_TOP_MID, 0, 212);
    return obj;
}
lv_obj_t *lv_manual_dialog1(const char *content, const char *cancel, const char *confirm, lv_event_cb_t event_cb)
{
    lv_obj_t *layer = get_manual_layer();
    return lv_dialog1(layer, content, cancel, confirm, NULL, event_cb);
}