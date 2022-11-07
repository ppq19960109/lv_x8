

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static char roller_options[1024] = {0};
/**********************
 *  STATIC VARIABLES
 **********************/
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

static void btn_array_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
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
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, i);

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

    lv_obj_t *roller1 = lv_roller_create(mode_obj);
    lv_obj_set_size(roller1, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(roller1, &style, 0);
    lv_obj_add_style(roller1, &style2, LV_PART_SELECTED);
    lv_roller_set_options(roller1,
                          "上下加热\n"
                          "立体热风\n"
                          "蒸汽嫩烤\n"
                          "热风烧烤\n"
                          "空气炸",
                          LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(roller1, 5);
    lv_obj_add_event_cb(roller1, scroll_event_cb, LV_EVENT_ALL, mode_img);
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

    const char *options = temp_roller_options(40, 230);
    lv_obj_t *roller2 = lv_roller_create(temp_obj);
    lv_obj_set_size(roller2, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(roller2, &style, 0);
    lv_obj_add_style(roller2, &style2, LV_PART_SELECTED);
    lv_roller_set_options(roller2,
                          options,
                          LV_ROLLER_MODE_INFINITE);
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

    options = time_roller_options(0);
    lv_obj_t *roller3 = lv_roller_create(time_obj);
    lv_obj_set_size(roller3, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(roller3, &style, 0);
    lv_obj_add_style(roller3, &style2, LV_PART_SELECTED);
    lv_roller_set_options(roller3,
                          options,
                          LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(roller3, 5);
    lv_obj_add_event_cb(roller3, scroll_event_cb, LV_EVENT_ALL, time_img);
    //------------------------------
    const char *text[] = {"启动", "预约"};
    lv_obj_t *btn_array = lv_btn_array_create(page, text, 2, btn_array_event_cb);
    lv_obj_align_to(btn_array, back_bar, LV_ALIGN_OUT_BOTTOM_RIGHT, -60, 90);
}
