

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static char roller_options[1024] = {0};
lv_obj_t *roller1, *roller2, *roller3;

steamoven_mode_t left_steamoven_mode[] = {
    {
        mode_index : 0,
        mode : 40,
        temp : 150,
        mintemp : 50,
        maxtemp : 200,
        time : 60,
        maxtime : 0,
        name : "蒸汽嫩烤"
    },
    {
        mode_index : 1,
        mode : 35,
        temp : 200,
        mintemp : 50,
        maxtemp : 230,
        time : 60,
        maxtime : 0,
        name : "热风烧烤"
    },
    {
        mode_index : 2,
        mode : 42,
        temp : 220,
        mintemp : 200,
        maxtemp : 230,
        time : 30,
        maxtime : 180,
        name : "空气速炸"
    },
    {
        mode_index : 3,
        mode : 4,
        temp : 120,
        mintemp : 101,
        maxtemp : 120,
        time : 20,
        maxtime : 0,
        name : "高温蒸"
    },
    {
        mode_index : 4,
        mode : 36,
        temp : 180,
        mintemp : 50,
        maxtemp : 230,
        time : 120,
        maxtime : 0,
        name : "上下加热"
    },
    {
        mode_index : 5,
        mode : 38,
        temp : 180,
        mintemp : 50,
        maxtemp : 230,
        time : 120,
        maxtime : 0,
        name : "立体热风"
    },
};
/**********************
 *  STATIC VARIABLES
 **********************/
steamoven_mode_t *get_steamoven_mode(unsigned char mode_index)
{
    for (int i = 0; i < sizeof(left_steamoven_mode) / sizeof(left_steamoven_mode[0]); ++i)
    {
        if (left_steamoven_mode[i].mode_index == mode_index)
            return &left_steamoven_mode[i];
    }
    return NULL;
}
static char *temp_roller_options(int min_temp, int max_temp)
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
static char *time_roller_options(int max_time)
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
void mode_change(char mode_index)
{
    steamoven_mode_t *steamoven_mode = get_steamoven_mode(mode_index);
    const char *options = temp_roller_options(steamoven_mode->mintemp, steamoven_mode->maxtemp);
    lv_roller_set_options(roller2,
                          options,
                          LV_ROLLER_MODE_INFINITE);
    lv_roller_set_selected(roller2, steamoven_mode->temp - steamoven_mode->mintemp, LV_ANIM_ON);

    options = time_roller_options(steamoven_mode->maxtime);
    lv_roller_set_options(roller3,
                          options,
                          LV_ROLLER_MODE_INFINITE);
    lv_roller_set_selected(roller3, steamoven_mode->time - 1, LV_ANIM_ON);
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
    return obj;
}
static void dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *obj = lv_event_get_current_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        lv_obj_clean(lv_layer_top());
        break;
    case 2:
    {
        char buf[8];
        unsigned char mode_index = lv_roller_get_selected(roller1);
        // lv_roller_get_selected_str(roller1, buf, sizeof(buf));
        unsigned char temp_index = lv_roller_get_selected(roller2);
        unsigned char time_index = lv_roller_get_selected(roller3);
        LV_LOG_USER("%s,roller index:%d,%d,%d\n", __func__, mode_index, temp_index, time_index);
        steamoven_t steamoven = {0};
        steamoven_mode_t *steamoven_mode = get_steamoven_mode(mode_index);
        steamoven.attr[0].mode = steamoven_mode->mode;
        steamoven.attr[0].temp = temp_index + steamoven_mode->mintemp;
        steamoven.attr[0].time = time_index + 1;
        set_cook_toServer(&steamoven);
    }
    break;
    }
}

static void lv_example_dialog(void)
{
    lv_obj_t *obj = lv_obj_create(lv_layer_top());
    lv_obj_set_size(obj, 730, 350);
    lv_obj_center(obj);
    lv_obj_add_event_cb(obj, dialog_event_cb, LV_EVENT_CLICKED, (void *)0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(themesPopupWindowColor), 0);
    lv_obj_set_style_radius(obj, 10, 0);

    lv_obj_t *close_obj = lv_obj_create(obj);
    lv_obj_set_size(close_obj, 80, 80);
    lv_obj_align(close_obj, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_event_cb(close_obj, dialog_event_cb, LV_EVENT_CLICKED, (void *)0);
    lv_obj_t *close_img = lv_img_create(close_obj);
    lv_obj_align(close_img, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(close_img, themesImagesPath "icon_window_close.png");

    lv_obj_t *icon_img = lv_img_create(obj);
    lv_obj_align(icon_img, LV_ALIGN_TOP_MID, 0, 55);
    lv_img_set_src(icon_img, themesImagesPath "icon_warn.png");

    lv_obj_t *label_content = lv_label_create(obj);
    lv_obj_set_style_text_font(label_content, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_content, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_content, "请将食物放入右腔,水箱中加满水");
    lv_obj_align(label_content, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *radio_btn = radiobutton_create(obj, "下次不再提示");
    lv_obj_align(radio_btn, LV_ALIGN_TOP_MID, 0, 212);

    lv_obj_t *btn1 = lv_custom_text_btn_create(obj, "取消");
    lv_obj_add_event_cb(btn1, dialog_event_cb, LV_EVENT_CLICKED, (void *)1);
    lv_obj_align(btn1, LV_ALIGN_BOTTOM_MID, -130, -25);

    lv_obj_t *btn2 = lv_custom_text_btn_create(obj, "开始");
    lv_obj_add_event_cb(btn2, dialog_event_cb, LV_EVENT_CLICKED, (void *)2);
    lv_obj_align(btn2, LV_ALIGN_BOTTOM_MID, 130, -25);
}

static void btn_array_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    lv_example_dialog();
}
static lv_obj_t *lv_btn_array_create(lv_obj_t *parent, const char *text[], const char count, lv_event_cb_t event_cb)
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

static void scroll_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *user_data = lv_event_get_user_data(e);
    if (e->code == LV_EVENT_PRESSED)
    {
        LV_LOG_USER("%s,code:%d\n", __func__, e->code);
        lv_obj_clear_flag(user_data, LV_OBJ_FLAG_HIDDEN);
    }
    else if (e->code == LV_EVENT_RELEASED)
    {
        LV_LOG_USER("%s,code:%d\n", __func__, e->code);
        lv_obj_add_flag(user_data, LV_OBJ_FLAG_HIDDEN);
        if (lv_obj_has_state(target, LV_STATE_CHECKED))
        {
            char mode_index = lv_roller_get_selected(target);
            mode_change(mode_index);
        }
    }
}

void lv_page_steam_left_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t *back_bar = lv_page_back_bar_init(page, "左腔蒸烤", NULL);

    lv_obj_t *cont_row = lv_obj_create(page);
    lv_obj_set_size(cont_row, 291 + 226 * 2 + 20 * 2, 281);
    lv_obj_align_to(cont_row, back_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 80, 30);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    static lv_style_t style, style2;
    lv_style_set_text_font(&style, &lv_font_SiYuanHeiTi_Normal_30);
    lv_style_set_text_color(&style, lv_color_hex(0xffffff));
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_line_space(&style, 16);

    lv_style_set_text_font(&style2, &lv_font_SiYuanHeiTi_Normal_34);
    lv_style_set_text_color(&style2, lv_color_hex(themesTextColor));
    lv_style_set_text_align(&style2, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_line_space(&style2, 16);
    //------------------------------
    lv_obj_t *mode_obj = lv_obj_create(cont_row);
    lv_obj_set_size(mode_obj, 291, LV_PCT(100));
    lv_obj_t *mode_img = lv_img_create(mode_obj);
    lv_img_set_src(mode_img, themesImagesPath "steamoven/mode_roll_background.png");
    lv_obj_align(mode_img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(mode_img, LV_OBJ_FLAG_HIDDEN);

    roller1 = lv_roller_create(mode_obj);
    lv_obj_set_size(roller1, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(roller1, &style, 0);
    lv_obj_add_style(roller1, &style2, LV_PART_SELECTED);
    lv_roller_set_options(roller1,
                          "蒸汽嫩烤\n"
                          "热风烧烤\n"
                          "空气速炸\n"
                          "高温蒸\n"
                          "上下加热\n"
                          "立体热风",
                          LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(roller1, 5);
    lv_obj_add_event_cb(roller1, scroll_event_cb, LV_EVENT_ALL, mode_img);
    lv_obj_add_state(roller1, LV_STATE_CHECKED);
    //----------------------------------------------------------------------------
    lv_obj_t *temp_obj = lv_obj_create(cont_row);
    lv_obj_set_size(temp_obj, 226, LV_PCT(100));
    lv_obj_t *temp_img = lv_img_create(temp_obj);
    lv_img_set_src(temp_img, themesImagesPath "steamoven/roll_background.png");
    lv_obj_align(temp_img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(temp_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_t *temp_label = lv_label_create(temp_obj);
    lv_obj_set_style_text_font(temp_label, &lv_font_SiYuanHeiTi_Normal_34, 0);
    lv_obj_set_style_text_color(temp_label, lv_color_hex(themesTextColor), 0);
    lv_label_set_text(temp_label, "度");
    lv_obj_align(temp_label, LV_ALIGN_CENTER, 60, -8);

    roller2 = lv_roller_create(temp_obj);
    lv_obj_set_size(roller2, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(roller2, &style, 0);
    lv_obj_add_style(roller2, &style2, LV_PART_SELECTED);

    lv_roller_set_visible_row_count(roller2, 5);
    lv_obj_add_event_cb(roller2, scroll_event_cb, LV_EVENT_ALL, temp_img);
    //----------------------------------------------------------------------------
    lv_obj_t *time_obj = lv_obj_create(cont_row);
    lv_obj_set_size(time_obj, 226, LV_PCT(100));
    lv_obj_t *time_img = lv_img_create(time_obj);
    lv_img_set_src(time_img, themesImagesPath "steamoven/roll_background.png");
    lv_obj_align(time_img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(time_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_t *time_label = lv_label_create(time_obj);
    lv_obj_set_style_text_font(time_label, &lv_font_SiYuanHeiTi_Normal_34, 0);
    lv_obj_set_style_text_color(time_label, lv_color_hex(themesTextColor), 0);
    lv_label_set_text(time_label, "分钟");
    lv_obj_align(time_label, LV_ALIGN_CENTER, 60, -8);

    roller3 = lv_roller_create(time_obj);
    lv_obj_set_size(roller3, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(roller3, &style, 0);
    lv_obj_add_style(roller3, &style2, LV_PART_SELECTED);

    lv_roller_set_visible_row_count(roller3, 5);
    lv_obj_add_event_cb(roller3, scroll_event_cb, LV_EVENT_ALL, time_img);

    mode_change(0);
    //------------------------------
    const char *text[] = {"启动", "预约"};
    lv_obj_t *btn_array = lv_btn_array_create(page, text, 2, btn_array_event_cb);
    lv_obj_align_to(btn_array, back_bar, LV_ALIGN_OUT_BOTTOM_RIGHT, -60, 90);
}
