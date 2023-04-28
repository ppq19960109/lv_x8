

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *roller_scroll[4], *label_temp, *label_steam;

static char *surging_model[] = {"小", "中", "大"};
static steamoven_mode_t steam_mode[] = {
    {
        mode_index : 0,
        mode : 40,
        temp : 150,
        mintemp : 50,
        maxtemp : 200,
        time : 60,
        maxtime : 0,
        name : "营养蒸"
    },
    {
        mode_index : 1,
        mode : 4,
        temp : 120,
        mintemp : 101,
        maxtemp : 120,
        time : 20,
        maxtime : 0,
        name : "高温蒸"
    },
    {
        mode_index : 2,
        mode : 35,
        temp : 2,
        mintemp : 1,
        maxtemp : 10,
        temp_model : surging_model,
        time : 60,
        maxtime : 0,
        name : "澎湃蒸"
    },
};
/**********************
 *  STATIC VARIABLES
 **********************/
steamoven_mode_t *get_steamoven_mode(unsigned char mode_index)
{
    for (int i = 0; i < sizeof(steam_mode) / sizeof(steam_mode[0]); ++i)
    {
        if (steam_mode[i].mode_index == mode_index)
            return &steam_mode[i];
    }
    return NULL;
}
static void cook_start(const int reserve_time)
{
    int mode_index = (int)lv_cycle_scroll_get_selected(roller_scroll[0])->user_data;
    int temp = (int)lv_cycle_scroll_get_selected(roller_scroll[1])->user_data;
    int time = (int)lv_cycle_scroll_get_selected(roller_scroll[2])->user_data;
    LV_LOG_USER("%s,roller index:%d,%d,%d\n", __func__, mode_index, temp, time);
    // steamoven_mode_t *steamoven_mode = get_steamoven_mode(mode_index);
    // steamoven_t steamoven = {0};
    // steamoven.attr[0].mode = steamoven_mode->mode;
    // steamoven.attr[0].temp = temp_index + steamoven_mode->mintemp;
    // steamoven.attr[0].time = time_index + 1;
    // steamoven.orderTime = reserve_time;
    // set_cook_toServer(&steamoven);
}
lv_obj_t *mode_roller_scroll_child_create(lv_obj_t *parent, const char *text, const int num)
{
    lv_obj_t *btn = lv_obj_create(parent);
    lv_obj_set_size(btn, LV_PCT(100), LV_PCT(33));

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label, g_robam_font.FZLTHC_44.font, 0);
    lv_obj_set_style_text_font(label, g_robam_font.FZLTHC_60.font, LV_STATE_CHECKED);
    if (text == NULL)
        lv_label_set_text_fmt(label, "%d", num);
    else
        lv_label_set_text(label, text);

    lv_obj_center(label);
    return btn;
}
static void mode_change(char mode_index)
{
    steamoven_mode_t *steamoven_mode = get_steamoven_mode(mode_index);
    int i;
    lv_obj_t *child;
    if (steamoven_mode->temp_model == NULL)
    {
        lv_obj_clean(roller_scroll[1]);
        for (i = steamoven_mode->mintemp; i <= steamoven_mode->maxtemp; ++i)
        {
            child = mode_roller_scroll_child_create(roller_scroll[1], NULL, i);
            child->user_data = (void *)i;
        }
        cycle_scroll_change(roller_scroll[1], steamoven_mode->temp - steamoven_mode->mintemp);

        lv_obj_add_flag(roller_scroll[3], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(roller_scroll[1], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_steam, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_temp, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(roller_scroll[1], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(roller_scroll[3], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_temp, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_steam, LV_OBJ_FLAG_HIDDEN);
    }

    lv_obj_clean(roller_scroll[2]);
    int time = 0;
    if (steamoven_mode->maxtime <= 0)
        time = 200;
    for (i = 1; i <= time; ++i)
    {
        child = mode_roller_scroll_child_create(roller_scroll[2], NULL, i);
        child->user_data = (void *)i;
    }
    cycle_scroll_change(roller_scroll[2], steamoven_mode->time - 1);
}

static void dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *obj = lv_event_get_current_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
    }
    break;
    }
    clean_manual_layer();
}
static void reserve_dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *obj = lv_event_get_current_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
        int orderTime = lv_get_reserve_dialog_time();
        LV_LOG_USER("%s,orderTime:%d\n", __func__, orderTime);
        // left_cook_start(orderTime);
    }
    break;
    }
    clean_manual_layer();
}

static void btn_array_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%ld\n", __func__, e->code, user_data);
    if (user_data == 0)
    {
        lv_manual_cook_dialog("请将食物放入左腔,水箱中加满水", dialog_event_cb);
    }
    else
    {
        lv_manual_reserve_dialog("左腔将在", "后启动", "预约", 12, reserve_dialog_event_cb);
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
static void scroll_child_select_cb(lv_obj_t *child, char select, char select_end)
{
    lv_obj_t *child2 = lv_obj_get_child(child, 0);
    if (select)
    {
        lv_obj_add_state(child, LV_STATE_CHECKED);
        lv_obj_add_state(child2, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(child, LV_STATE_CHECKED);
        lv_obj_clear_state(child2, LV_STATE_CHECKED);
    }
    lv_obj_center(child2);
    if (select_end)
    {
        if (lv_obj_get_parent(child) == roller_scroll[0])
        {
            mode_change((int)child->user_data);
        }
    }
}
static void bottom_bar_event_cb(lv_event_t *e)
{
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    if (user_data == 0)
    {
    }
    else
    {
        // lv_theme_switch(1);
        lv_100ask_page_manager_set_open_page(NULL, "page_steaming");
    }
}
void lv_page_steam_mode_init(lv_obj_t *page)
{
    lv_page_back_bar_init(page, "返回", NULL, NULL);
    lv_page_bottom_bar_init(page, "预约", "开始烹饪", bottom_bar_event_cb);

    lv_obj_t *cont_row = lv_obj_create(page);
    lv_obj_set_size(cont_row, 190 * 3 + 90 * 2, 242);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 106);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    label_temp = lv_label_create(page);
    lv_obj_set_style_text_color(label_temp, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_temp, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_temp, "℃");
    lv_obj_align_to(label_temp, cont_row, LV_ALIGN_CENTER, 80, -10);

    label_steam = lv_label_create(page);
    lv_obj_set_style_text_color(label_steam, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_steam, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_steam, "蒸汽");
    lv_obj_align_to(label_steam, cont_row, LV_ALIGN_CENTER, 70, 12);

    lv_obj_t *label_time = lv_label_create(page);
    lv_obj_set_style_text_color(label_time, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_time, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_time, "min");
    lv_obj_align_to(label_time, cont_row, LV_ALIGN_RIGHT_MID, 25, 12);

    //------------------------------
    static lv_cycle_scroll_t lv_cycle_scroll = {0};
    lv_cycle_scroll.cb = scroll_child_select_cb;
    lv_cycle_scroll.cycle_flag = 1;
    lv_cycle_scroll.mask_flag = 1;
    int i;
    roller_scroll[0] = lv_cycle_scroll_create(cont_row, 190, LV_PCT(100), LV_FLEX_FLOW_COLUMN, &lv_cycle_scroll);
    lv_obj_clear_flag(roller_scroll[0], LV_OBJ_FLAG_SCROLL_ELASTIC);
    // lv_obj_clear_flag(roller_scroll[0], LV_OBJ_FLAG_SCROLL_MOMENTUM);
    // lv_obj_add_flag(roller_scroll[0], LV_OBJ_FLAG_SCROLL_ONE);
    // 子对象
    for (i = 0; i < sizeof(steam_mode) / sizeof(steam_mode[0]); ++i)
    {
        lv_obj_t *child = mode_roller_scroll_child_create(roller_scroll[0], steam_mode[i].name, 0);
        child->user_data = (void *)i;
    }
    lv_obj_t *bg_line = lv_img_create(cont_row);
    lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
    lv_obj_set_size(bg_line, 2, LV_PCT(100));
    //----------------------------------------------------------------------------
    roller_scroll[1] = lv_cycle_scroll_create(cont_row, 180, LV_PCT(100), LV_FLEX_FLOW_COLUMN, &lv_cycle_scroll);
    lv_obj_clear_flag(roller_scroll[0], LV_OBJ_FLAG_SCROLL_ELASTIC);
    roller_scroll[3] = lv_cycle_scroll_create(cont_row, 180, LV_PCT(100), LV_FLEX_FLOW_COLUMN, &lv_cycle_scroll);
    for (i = 0; i < sizeof(surging_model) / sizeof(surging_model[0]); ++i)
    {
        lv_obj_t *child = mode_roller_scroll_child_create(roller_scroll[3], surging_model[i], 0);
        child->user_data = (void *)i;
    }
    //----------------------------------------------------------------------------
    bg_line = lv_img_create(cont_row);
    lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
    lv_obj_set_size(bg_line, 2, LV_PCT(100));
    roller_scroll[2] = lv_cycle_scroll_create(cont_row, 180, LV_PCT(100), LV_FLEX_FLOW_COLUMN, &lv_cycle_scroll);
    // lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLL_MOMENTUM);

    cycle_scroll_change(roller_scroll[0], 0);
}
