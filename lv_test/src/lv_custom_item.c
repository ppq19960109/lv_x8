

/*********************
 *      INCLUDES
 *********************/
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static char roller_options[1024] = {0};
/**********************
 *  STATIC VARIABLES
 **********************/
int lv_custom_isSteam(steamoven_t *steamoven)
{
    steamoven_attr_t *step;
    if (steamoven->attr_len == 0)
    {
        step = &steamoven->attr[0];
        if (step->mode == 1 || step->mode == 3 || step->mode == 4 || step->mode == 65 || step->mode == 66 || step->mode == 68)
        {
            return 1;
        }
    }
    else
    {
        for (int i = 0; i < steamoven->attr_len; ++i)
        {
            step = &steamoven->attr[i];
            if (step->mode == 1 || step->mode == 3 || step->mode == 4 || step->mode == 65 || step->mode == 66 || step->mode == 68)
            {
                return 1;
            }
        }
    }
    return 0;
}

lv_obj_t *lv_custom_image_button_create(lv_obj_t *parent, const void *img_src, lv_coord_t width, lv_coord_t height,
                                        lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *image_button = lv_obj_create(parent);
    lv_obj_set_size(image_button, width, height);

    lv_obj_t *img = lv_img_create(image_button);
    lv_img_set_src(img, img_src);
    lv_obj_align(img, LV_ALIGN_CENTER, x, y);
    return image_button;
}
lv_obj_t *lv_custom_text_btn_create(lv_obj_t *parent, const char *text)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 140, 50);
    lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn, 25, 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return btn;
}
static void set_angle(void *img, int32_t v)
{
    lv_img_set_angle(img, v);
}
lv_obj_t *lv_rotate_anim(lv_obj_t *obj, const int run)
{
    lv_anim_t *anim = lv_anim_get(obj, NULL);
    LV_LOG_USER("%s,run:%d anim:%p\n", __func__, run, anim);
    if (run)
    {
        if (anim == NULL)
        {
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, obj);
            lv_anim_set_exec_cb(&a, set_angle);
            lv_anim_set_values(&a, 0, 3600);
            lv_anim_set_time(&a, 6000);
            lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
            lv_anim_start(&a);
        }
    }
    else
    {
        if (anim != NULL)
            lv_anim_del(obj, set_angle);
    }
    return NULL;
}
char *temp_roller_options(int min_temp, int max_temp)
{
    int i = 0, index = 0;
    for (i = min_temp; i < max_temp; ++i)
    {
        sprintf(&roller_options[index], "%d\n", i);
        if (i < 10)
            index += 2;
        else if (i < 100)
            index += 3;
        else
            index += 4;
    }
    sprintf(&roller_options[index], "%d", max_temp);
    return roller_options;
}
char *time_roller_options(int max_time)
{
    int i = 0, index = 0;
    if (max_time <= 0)
        max_time = 300;
    for (i = 1; i <= 120; ++i)
    {
        sprintf(&roller_options[index], "%d\n", i);
        if (i < 10)
            index += 2;
        else if (i < 100)
            index += 3;
        else
            index += 4;
    }
    for (i = 125; i < max_time; i += 5)
    {
        sprintf(&roller_options[index], "%d\n", i);
        index += 4;
    }
    sprintf(&roller_options[index], "%d", max_time);
    LV_LOG_USER("%s,index:%d", __func__, index);
    return roller_options;
}
lv_obj_t *radiobutton_create(lv_obj_t *parent, const char *txt)
{
    lv_obj_t *obj = lv_checkbox_create(parent);
    lv_checkbox_set_text(obj, txt);
    lv_obj_set_style_text_font(obj, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(themesTextColor), LV_PART_INDICATOR);
    lv_obj_set_style_border_color(obj, lv_color_hex(themesTextColor), LV_PART_INDICATOR);
    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR);
    lv_obj_set_style_pad_column(obj, 20, 0);
    return obj;
}
lv_obj_t *lv_custom_btn_array_create(lv_obj_t *parent, const char *text[], const char count, lv_event_cb_t event_cb)
{
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 140, 50 * count + 55);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    for (int i = 0; i < count; ++i)
    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 140, 50);
        lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
        lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
        lv_obj_set_style_radius(btn, 25, 0);
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, (void *)i);

        lv_obj_t *label = lv_label_create(btn);
        lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
        lv_label_set_text(label, text[i]);
        lv_obj_center(label);
    }
    return cont;
}
lv_obj_t *lv_custom_cook_dialog(const char *content, lv_event_cb_t event_cb)
{
    lv_obj_t *obj = lv_obj_create(lv_layer_top());
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

    lv_obj_t *btn1 = lv_custom_text_btn_create(obj, "取消");
    lv_obj_add_event_cb(btn1, event_cb, LV_EVENT_CLICKED, (void *)1);
    lv_obj_align(btn1, LV_ALIGN_BOTTOM_MID, -130, -25);

    lv_obj_t *btn2 = lv_custom_text_btn_create(obj, "开始");
    lv_obj_add_event_cb(btn2, event_cb, LV_EVENT_CLICKED, (void *)2);
    lv_obj_align(btn2, LV_ALIGN_BOTTOM_MID, 130, -25);

    lv_obj_t *radio_btn = radiobutton_create(obj, "下次不再提示");
    lv_obj_align(radio_btn, LV_ALIGN_TOP_MID, 0, 212);
    return obj;
}
lv_obj_t *lv_custom_dialog2(const char *content, const char *cancel, const char *confirm, lv_event_cb_t event_cb)
{
    lv_obj_t *obj = lv_obj_create(lv_layer_top());
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

    lv_obj_t *btn1 = lv_custom_text_btn_create(obj, cancel);
    lv_obj_add_event_cb(btn1, event_cb, LV_EVENT_CLICKED, (void *)1);
    lv_obj_align(btn1, LV_ALIGN_BOTTOM_MID, -130, -25);

    lv_obj_t *btn2 = lv_custom_text_btn_create(obj, confirm);
    lv_obj_add_event_cb(btn2, event_cb, LV_EVENT_CLICKED, (void *)2);
    lv_obj_align(btn2, LV_ALIGN_BOTTOM_MID, 130, -25);

    return obj;
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
lv_obj_t *lv_custom_reserve_dialog(const char *content, lv_event_cb_t event_cb)
{
    lv_obj_t *obj = lv_obj_create(lv_layer_top());
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
    lv_label_set_text(label_content, content);
    lv_obj_align(label_content, LV_ALIGN_TOP_LEFT, 32, 140);

    lv_obj_t *label_content2 = lv_label_create(obj);
    lv_obj_set_style_text_font(label_content2, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_content2, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_content2, "后启动");
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
    const char *options = temp_roller_options(0, 12);
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

    lv_obj_t *btn2 = lv_custom_text_btn_create(obj, "预约");
    lv_obj_add_event_cb(btn2, event_cb, LV_EVENT_CLICKED, (void *)2);
    lv_obj_align(btn2, LV_ALIGN_BOTTOM_MID, 130, -25);
    return obj;
}

int lv_get_reserve_dialog_time(lv_obj_t *reserve_dialog)
{
    lv_obj_t *hour_roller = lv_obj_get_child(lv_obj_get_child(reserve_dialog, -4), -1);
    lv_obj_t *minute_roller = lv_obj_get_child(lv_obj_get_child(reserve_dialog, -3), -1);
    unsigned char hour_time_index = lv_roller_get_selected(hour_roller);
    unsigned char minute_time_index = lv_roller_get_selected(minute_roller);
    return hour_time_index * 60 + minute_time_index;
}

lv_obj_t *lv_divider_create(lv_obj_t *parent)
{
    lv_obj_t *divider = lv_obj_create(parent);
    lv_obj_set_size(divider, LV_PCT(100), 2);
    lv_obj_set_style_bg_opa(divider, 15, 0);
    lv_obj_set_style_bg_color(divider, lv_color_hex(0xE7E7E7), 0);
    return divider;
}

lv_obj_t *lv_page_indicator_create(lv_obj_t *parent, int count)
{
    lv_obj_t *cont_row = lv_obj_create(parent);
    lv_obj_set_size(cont_row, 15 * count + 15 * (count - 1), 15);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *element;
    for (int i = 0; i < count; ++i)
    {
        element = lv_btn_create(cont_row);
        lv_obj_set_size(element, 15, 15);
        lv_obj_set_style_bg_color(element, lv_color_hex(0x6F6F6F), 0);
        lv_obj_set_style_bg_color(element, lv_color_hex(0xffffff), LV_STATE_CHECKED);
        lv_obj_set_style_bg_opa(element, LV_OPA_100, 0);
        lv_obj_set_style_radius(element, LV_RADIUS_CIRCLE, 0);
    }
    return cont_row;
}
void lv_page_indicator_check(lv_obj_t *page_indicator, int index)
{
    int count = lv_obj_get_child_cnt(page_indicator);
    lv_obj_t *child;
    for (int i = 0; i < count; ++i)
    {
        child = lv_obj_get_child(page_indicator, i);
        if (i == index)
        {
            lv_obj_add_state(child, LV_STATE_CHECKED);
        }
        else if (lv_obj_has_state(child, LV_STATE_CHECKED))
        {
            lv_obj_clear_state(child, LV_STATE_CHECKED);
        }
    }
}