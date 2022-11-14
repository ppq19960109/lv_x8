

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
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
static steamoven_mode_t *get_steamoven_mode(unsigned char mode_index)
{
    for (int i = 0; i < sizeof(left_steamoven_mode) / sizeof(left_steamoven_mode[0]); ++i)
    {
        if (left_steamoven_mode[i].mode_index == mode_index)
            return &left_steamoven_mode[i];
    }
    return NULL;
}
static void left_cook_start(const int reserve_time)
{
    unsigned char mode_index = lv_roller_get_selected(roller1);
    unsigned char temp_index = lv_roller_get_selected(roller2);
    unsigned char time_index = lv_roller_get_selected(roller3);
    LV_LOG_USER("%s,roller index:%d,%d,%d\n", __func__, mode_index, temp_index, time_index);
    steamoven_mode_t *steamoven_mode = get_steamoven_mode(mode_index);
    steamoven_t steamoven = {0};
    steamoven.attr[0].mode = steamoven_mode->mode;
    steamoven.attr[0].temp = temp_index + steamoven_mode->mintemp;
    steamoven.attr[0].time = time_index + 1;
    steamoven.orderTime = reserve_time;
    set_cook_toServer(&steamoven);
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

static void dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *obj = lv_event_get_current_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
        left_cook_start(0);
    }
    break;
    }
    lv_obj_clean(lv_layer_top());
}
static void reserve_dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *obj = lv_event_get_current_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
        lv_obj_t *reserve_dialog = lv_obj_get_child(lv_layer_top(), 0);
        int orderTime = lv_get_reserve_dialog_time(reserve_dialog);
        LV_LOG_USER("%s,orderTime:%d\n", __func__, orderTime);
        left_cook_start(orderTime);
    }
    break;
    }
    lv_obj_clean(lv_layer_top());
}

static void btn_array_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    if (user_data == 0)
    {
        lv_custom_cook_dialog("请将食物放入左腔,水箱中加满水", dialog_event_cb);
    }
    else
    {
        lv_custom_reserve_dialog("左腔将在", reserve_dialog_event_cb);
    }
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

    //------------------------------
    lv_obj_t *mode_obj = lv_obj_create(cont_row);
    lv_obj_set_size(mode_obj, 291, LV_PCT(100));
    lv_obj_t *mode_img = lv_img_create(mode_obj);
    lv_img_set_src(mode_img, themesImagesPath "steamoven/mode_roll_background.png");
    lv_obj_align(mode_img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(mode_img, LV_OBJ_FLAG_HIDDEN);

    roller1 = lv_roller_create(mode_obj);
    lv_obj_set_size(roller1, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(roller1, &roller_style_unselected, 0);
    lv_obj_add_style(roller1, &roller_style_selected, LV_PART_SELECTED);
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
    lv_obj_add_style(roller2, &roller_style_unselected, 0);
    lv_obj_add_style(roller2, &roller_style_selected, LV_PART_SELECTED);

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
    lv_obj_add_style(roller3, &roller_style_unselected, 0);
    lv_obj_add_style(roller3, &roller_style_selected, LV_PART_SELECTED);

    lv_roller_set_visible_row_count(roller3, 5);
    lv_obj_add_event_cb(roller3, scroll_event_cb, LV_EVENT_ALL, time_img);

    mode_change(0);
    //------------------------------
    const char *text[] = {"启动", "预约"};
    lv_obj_t *btn_array = lv_custom_btn_array_create(page, text, 2, btn_array_event_cb);
    lv_obj_align_to(btn_array, back_bar, LV_ALIGN_OUT_BOTTOM_RIGHT, -60, 90);
}
