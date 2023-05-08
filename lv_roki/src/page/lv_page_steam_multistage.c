

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/
typedef struct
{
    unsigned char mode;
    unsigned char vapour;
    unsigned short temp;
    unsigned short lower_temp;
    unsigned short time;
} multistage_para_t;
static multistage_para_t multistage_para[3] = {0};
static int listLastIndex = 0;
static int listResetIndex = -1;
static lv_obj_t *roller1, *roller2, *roller3;
static lv_obj_t *page_cont_row, *bottom_bar;
/**********************
 *  STATIC VARIABLES
 **********************/
static void multistage_cook_start(const int reserve_time)
{
    steamoven_t steamoven = {0};
    for (int i = 0; i < 3; ++i)
    {
        steamoven.attr[i].mode = multistage_para[i].mode;
        steamoven.attr[i].temp = multistage_para[i].temp;
        steamoven.attr[i].time = multistage_para[i].time;
    }
    steamoven.orderTime = reserve_time;
    set_cook_toServer(&steamoven);
}
static void multistage_update(void)
{
    lv_obj_t *parent;
    lv_obj_t *child[7];
    char buf[16];
    int i, j;
    LV_LOG_USER("%s,page_cont_row:%p listLastIndex:%d\n", __func__, page_cont_row, listLastIndex);
    for (i = 0; i < 3; ++i)
    {
        parent = lv_obj_get_child(page_cont_row, i);
        for (j = 0; j < 7; ++j)
        {
            child[j] = lv_obj_get_child(parent, j);
        }

        if (listLastIndex > i)
        {
            lv_obj_add_flag(child[0], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[1], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(child[2], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(child[6], LV_OBJ_FLAG_HIDDEN);

            lv_label_set_text(child[2], workModeName(multistage_para[i].mode));
            if (multistage_para[i].vapour == 0)
                lv_obj_add_flag(child[3], LV_OBJ_FLAG_HIDDEN);
            else
            {
                lv_obj_clear_flag(child[3], LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text(lv_obj_get_child(child[3], 0), vapour_model[multistage_para[i].vapour - 1]);
            }
            sprintf(buf, "%d", multistage_para[i].temp);
            lv_label_set_text(lv_obj_get_child(child[4], 0), buf);
            if (multistage_para[i].lower_temp == 0)
                lv_obj_add_flag(child[5], LV_OBJ_FLAG_HIDDEN);
            else
            {
                lv_obj_clear_flag(child[5], LV_OBJ_FLAG_HIDDEN);
                sprintf(buf, "%d", multistage_para[i].lower_temp);
                lv_label_set_text(lv_obj_get_child(child[5], 0), buf);
            }
            sprintf(buf, "%d", multistage_para[i].time);
            lv_label_set_text(lv_obj_get_child(child[6], 0), buf);
        }
        else
        {
            lv_obj_clear_flag(child[0], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(child[1], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[2], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[3], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[4], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[5], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[6], LV_OBJ_FLAG_HIDDEN);
            if (listLastIndex == i)
            {
                lv_obj_set_style_opa(parent, LV_OPA_100, 0);
            }
            else
            {
                lv_obj_set_style_opa(parent, LV_OPA_40, 0);
            }
        }
    }
}
static void roller_click_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%ld\n", __func__, e->code, user_data);
    switch (user_data)
    {
    case 0:
    case 1:

        break;
    case 2:
    {
        unsigned char mode_index = lv_roller_get_selected(roller1);
        unsigned char temp_index = lv_roller_get_selected(roller2);
        unsigned char time_index = lv_roller_get_selected(roller3);
        LV_LOG_USER("%s,roller index:%d,%d,%d\n", __func__, mode_index, temp_index, time_index);
        steamoven_mode_t *steamoven_mode = get_steamoven_mode(mode_index);
        if (listResetIndex >= 0)
        {
            multistage_para[listResetIndex].mode = steamoven_mode->mode;
            multistage_para[listResetIndex].temp = temp_index + steamoven_mode->mintemp;
            multistage_para[listResetIndex].time = time_index + 1;
        }
        else
        {
            multistage_para[listLastIndex].mode = steamoven_mode->mode;
            multistage_para[listLastIndex].temp = temp_index + steamoven_mode->mintemp;
            multistage_para[listLastIndex].time = time_index + 1;
            ++listLastIndex;
        }
        multistage_update();
    }
    break;
    }
    lv_obj_clean(lv_layer_top());
}
static void mode_change(char mode_index)
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
    else if (e->code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    }
}
static void steam_left_roller_create(lv_obj_t *parent)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, 1066, 350);
    lv_obj_center(obj);

    lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(themesPopupWindowColor), 0);
    lv_obj_set_style_radius(obj, 10, 0);

    lv_obj_t *close_obj = lv_obj_create(obj);
    lv_obj_set_size(close_obj, 80, 80);
    lv_obj_align(close_obj, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_event_cb(close_obj, roller_click_event_cb, LV_EVENT_CLICKED, (void *)0);
    lv_obj_t *close_img = lv_img_create(close_obj);
    lv_obj_align(close_img, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(close_img, themesImagesPath "icon_window_close.png");

    lv_obj_t *cont_row = lv_obj_create(obj);
    lv_obj_set_size(cont_row, 291 + 226 * 2 + 20 * 2, 281);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, -10, 15);
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
    lv_label_set_text(temp_label, "℃");
    lv_obj_align(temp_label, LV_ALIGN_CENTER, 50, -6);

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
    lv_obj_align(time_label, LV_ALIGN_CENTER, 70, -6);

    roller3 = lv_roller_create(time_obj);
    lv_obj_set_size(roller3, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(roller3, &roller_style_unselected, 0);
    lv_obj_add_style(roller3, &roller_style_selected, LV_PART_SELECTED);
    // lv_obj_set_style_text_line_space(roller3, 50, 0);
    // lv_obj_set_style_text_line_space(roller3, 50, LV_PART_SELECTED);
    lv_roller_set_visible_row_count(roller3, 5);
    lv_obj_add_event_cb(roller3, scroll_event_cb, LV_EVENT_ALL, time_img);

    mode_change(0);

    lv_obj_t *btn1 = lv_custom_text_btn_create(obj, "取消");
    lv_obj_add_event_cb(btn1, roller_click_event_cb, LV_EVENT_CLICKED, (void *)1);
    lv_obj_align(btn1, LV_ALIGN_BOTTOM_MID, -130, -25);

    lv_obj_t *btn2 = lv_custom_text_btn_create(obj, "确认");
    lv_obj_add_event_cb(btn2, roller_click_event_cb, LV_EVENT_CLICKED, (void *)2);
    lv_obj_align(btn2, LV_ALIGN_BOTTOM_MID, 130, -25);
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
        multistage_cook_start(0);
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
        multistage_cook_start(orderTime);
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

static void page_update_cb(void *arg)
{
    memset(multistage_para, 0, sizeof(multistage_para));
    listLastIndex = 0;
    // multistage_update();
}
static void add_click_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    if (user_data == listLastIndex)
    {
        listResetIndex = -1;
        steam_left_roller_create(lv_layer_top());
    }
}
static void btn_click_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    int index = user_data / 2;
    LV_LOG_USER("%s,code:%d listLastIndex:%d index:%d\n", __func__, e->code, listLastIndex, index);
    if (user_data % 2 == 0)
    {
        if (index < 2)
            memcpy(&multistage_para[index], &multistage_para[index + 1], sizeof(multistage_para_t) * (2 - index));
        --listLastIndex;
        memset(&multistage_para[listLastIndex], 0, sizeof(multistage_para_t) * (3 - index));
        multistage_update();
    }
    else
    {
        listResetIndex = index;
        steam_left_roller_create(lv_layer_top());
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
        lv_100ask_page_manager_set_open_page(NULL, "page_cook_tab");
    }
}
void lv_page_multistage_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_update_cb = page_update_cb;

    lv_page_back_bar_init(page, "返回", NULL, NULL);
    bottom_bar = lv_page_bottom_bar_init(page, NULL, "开始烹饪", bottom_bar_event_cb);
    lv_obj_t *label_total_time = lv_label_create(bottom_bar);
    lv_obj_set_style_text_font(label_total_time, g_robam_font.FZLTHJW_32.font, 0);
    lv_obj_set_style_text_color(label_total_time, getThemesFontColor1(), 0);
    lv_label_set_text(label_total_time, "总时长");
    lv_obj_align(label_total_time, LV_ALIGN_LEFT_MID, 40, 0);
    label_total_time = lv_custom_time_create(bottom_bar, 332, g_robam_font.FZLTHC_48.font, 0, 5);
    lv_obj_align(label_total_time, LV_ALIGN_LEFT_MID, 150, 0);

    page_cont_row = lv_obj_create(page);
    lv_obj_set_size(page_cont_row, 750, 230);
    lv_obj_align(page_cont_row, LV_ALIGN_TOP_MID, 0, 120);
    lv_obj_set_flex_flow(page_cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(page_cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    //------------------------------
    char buf[10];
    for (int i = 0; i < 3; ++i)
    {
        if (i > 0)
        {
            lv_obj_t *bg_line = lv_img_create(page_cont_row);
            lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
            lv_obj_set_size(bg_line, 2, LV_PCT(100));
        }
        lv_obj_t *item_obj = lv_obj_create(page_cont_row);
        lv_obj_set_flex_flow(item_obj, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(item_obj, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        // lv_obj_set_style_bg_opa(item_obj, LV_OPA_100, 0);
        lv_obj_set_size(item_obj, 150, LV_PCT(100));

        lv_obj_t *label = lv_label_create(item_obj); // 0
        lv_obj_set_style_text_font(label, g_robam_font.FZLTHJW_48.font, 0);
        lv_obj_set_style_text_color(label, getThemesFontColor1(), 0);
        sprintf(buf, "第%d段", i + 1);
        lv_label_set_text(label, buf);

        lv_obj_t *img = lv_img_create(item_obj); // 1
        lv_img_set_src(img, getThemesPath("bg_multi_add.png"));
        //-------------------------------------------------------
        label = lv_label_create(item_obj); // 2
        lv_obj_set_style_text_font(label, g_robam_font.FZLTHJW_44.font, 0);
        lv_obj_set_style_text_color(label, getThemesFontColor1(), 0);
        lv_label_set_text(label, workModeName(4));

        label = lv_custom_vapour_create(item_obj, 2, g_robam_font.FZLTHC_44.font, 0, 5);  // 3
        label = lv_custom_temp_create(item_obj, 180, g_robam_font.FZLTHC_44.font, 0, -5); // 4
        label = lv_custom_temp_create(item_obj, 280, g_robam_font.FZLTHC_44.font, 0, -5); // 5

        label = lv_custom_time_create(item_obj, 332, g_robam_font.FZLTHC_44.font, 0, 5); // 6

        lv_obj_add_event_cb(item_obj, add_click_event_cb, LV_EVENT_CLICKED, (void *)i);
    }
}
