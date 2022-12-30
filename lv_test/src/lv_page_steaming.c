

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *left_steaming, *right_steaming;
static lv_obj_t *btn_array[2];
static lv_obj_t *left_child[11], *right_child[11];
static char work_state[2];
/**********************
 *  STATIC VARIABLES
 **********************/
static void btn_array_update(const int index, const char *text[], const int count)
{
    lv_obj_t *child;
    int i;
    for (i = 0; i < count; ++i)
    {
        child = lv_obj_get_child(btn_array[index], i);
        lv_obj_clear_flag(child, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(lv_obj_get_child(child, 0), text[i]);
    }
    for (; i < 3; ++i)
    {
        child = lv_obj_get_child(btn_array[index], i);
        lv_obj_add_flag(child, LV_OBJ_FLAG_HIDDEN);
    }
}
static void page_update_StOvState(const int index, void *ptr)
{
    int value;
    lv_obj_t **child;
    if (index == 0)
    {
        if (ptr == NULL)
            value = get_attr_value_int("LStOvState");
        child = left_child;
    }
    else
    {
        if (ptr == NULL)
            value = get_attr_value_int("RStOvState");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);
    work_state[index] = value;
    if (value == WORK_STATE_STOP)
    {
        lv_obj_clear_flag(child[0], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[1], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[2], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[3], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[4], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[5], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[6], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[7], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[8], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[9], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[10], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn_array[index], LV_OBJ_FLAG_HIDDEN);
    }
    else if (value == WORK_STATE_FINISH)
    {
        lv_obj_add_flag(child[0], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[1], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[2], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[3], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[4], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[5], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[6], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[7], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[8], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[9], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[10], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(btn_array[index], LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(child[0], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[1], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[2], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[3], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[4], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[5], LV_OBJ_FLAG_HIDDEN);
        if (value == WORK_STATE_PREHEAT)
        {
            lv_obj_add_flag(child[6], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(child[7], LV_OBJ_FLAG_HIDDEN);
            lv_rotate_anim(child[7], 1);
        }
        else
        {
            lv_obj_clear_flag(child[6], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[7], LV_OBJ_FLAG_HIDDEN);
            lv_rotate_anim(child[7], 0);
        }
        lv_obj_add_flag(child[8], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[9], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[10], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn_array[index], LV_OBJ_FLAG_HIDDEN);

        lv_label_set_text(child[2], workStateChineseEnum[value]);
    }
    char *btn_text[3];
    int btn_count = 0;
    if (value == WORK_STATE_STOP)
    {
        if (index == 0)
        {
            btn_text[btn_count++] = "左腔蒸烤";
            btn_text[btn_count++] = "多段烹饪";
            btn_text[btn_count++] = "智慧菜谱";
        }
        else
        {
            btn_text[btn_count++] = "右腔速蒸";
            btn_text[btn_count++] = "辅助烹饪";
        }
    }
    else if (value == WORK_STATE_PAUSE || value == WORK_STATE_PREHEAT_RESERVE)
    {
        btn_text[btn_count++] = "继续";
        btn_text[btn_count++] = "取消";
    }
    else if (value == WORK_STATE_PAUSE_RESERVE)
    {
        btn_text[btn_count++] = "继续";
        btn_text[btn_count++] = "取消";
        btn_text[btn_count++] = "立即烹饪";
    }
    else if (value == WORK_STATE_RESERVE)
    {
        btn_text[btn_count++] = "暂停";
        btn_text[btn_count++] = "取消";
        btn_text[btn_count++] = "立即烹饪";
    }
    else if (value == WORK_STATE_FINISH)
    {
        return;
    }
    else
    {
        btn_text[btn_count++] = "暂停";
        btn_text[btn_count++] = "取消";
    }
    btn_array_update(index, btn_text, btn_count);
}

static void page_update_StOvRealTemp(const int index, void *ptr)
{
    if (work_state[index] != WORK_STATE_PREHEAT && work_state[index] != WORK_STATE_PREHEAT_RESERVE)
        return;
    int value;
    lv_obj_t **child;
    if (index == 0)
    {
        if (ptr == NULL)
            value = get_attr_value_int("LStOvRealTemp");
        child = left_child;
    }
    else
    {
        if (ptr == NULL)
            value = get_attr_value_int("RStOvRealTemp");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);
    char buf[16];
    sprintf(buf, "%dC", value);
    lv_label_set_text(child[3], buf);
}
static void page_update_StOvSetTimerLeft(const int index, void *ptr)
{
    int value;
    float setTime_value;
    lv_obj_t **child;
    if (index == 0)
    {
        setTime_value = get_attr_value_int("LStOvSetTimer");
        if (ptr == NULL)
            value = get_attr_value_int("LStOvSetTimerLeft");
        child = left_child;
    }
    else
    {
        setTime_value = get_attr_value_int("RStOvSetTimer");
        if (ptr == NULL)
            value = get_attr_value_int("RStOvSetTimerLeft");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);

    if (work_state[index] == WORK_STATE_RUN || work_state[index] == WORK_STATE_PAUSE)
    {
        char buf[16];
        sprintf(buf, "%d分钟", value);
        lv_label_set_text(child[3], buf);
    }

    lv_arc_set_value(child[6], (value / setTime_value) * 100);
}
static void page_update_StOvOrderTimerLeft(const int index, void *ptr)
{
    if (work_state[index] != WORK_STATE_RESERVE && work_state[index] != WORK_STATE_PAUSE_RESERVE)
        return;
    int value;
    float setTime_value;
    lv_obj_t **child;
    if (index == 0)
    {
        setTime_value = get_attr_value_int("LStOvOrderTimer");
        if (ptr == NULL)
            value = get_attr_value_int("LStOvOrderTimerLeft");
        child = left_child;
    }
    else
    {
        setTime_value = get_attr_value_int("RStOvOrderTimer");
        if (ptr == NULL)
            value = get_attr_value_int("RStOvOrderTimerLeft");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);
    char buf[16];
    sprintf(buf, "%d分钟", value);
    lv_label_set_text(child[3], buf);

    lv_arc_set_value(child[6], (value / setTime_value) * 100);
}
static void page_update_StOvMode(const int index, void *ptr)
{
    int value;
    lv_obj_t **child;
    if (index == 0)
    {
        if (ptr == NULL)
            value = get_attr_value_int("LStOvMode");
        child = left_child;
    }
    else
    {
        if (ptr == NULL)
            value = get_attr_value_int("RStOvMode");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);
    lv_label_set_text(child[4], workModeName(value));
}
static void page_update_StOvSetTemp_StOvSetTimer(const int index, void *ptr)
{
    int temp_value, time_value;
    lv_obj_t **child;
    if (index == 0)
    {
        temp_value = get_attr_value_int("LStOvSetTemp");
        time_value = get_attr_value_int("LStOvSetTimer");
        child = left_child;
    }
    else
    {
        temp_value = get_attr_value_int("RStOvSetTemp");
        time_value = get_attr_value_int("RStOvSetTimer");
        child = right_child;
    }
    char buf[28];
    sprintf(buf, "%dC  %d分钟", temp_value, time_value);
    lv_label_set_text(child[5], buf);
}
static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("key:%s\n", key);
    if (strcmp("LStOvState", key) == 0)
    {
        page_update_StOvState(0, value);
    }
    else if (strcmp("RStOvState", key) == 0)
    {
        page_update_StOvState(1, value);
    }
    else if (strcmp("LStOvRealTemp", key) == 0)
    {
        page_update_StOvRealTemp(0, value);
    }
    else if (strcmp("RStOvRealTemp", key) == 0)
    {
        page_update_StOvRealTemp(1, value);
    }
    else if (strcmp("LStOvSetTimerLeft", key) == 0)
    {
        page_update_StOvSetTimerLeft(0, value);
    }
    else if (strcmp("RStOvSetTimerLeft", key) == 0)
    {
        page_update_StOvSetTimerLeft(1, value);
    }
    else if (strcmp("LStOvOrderTimerLeft", key) == 0)
    {
        page_update_StOvOrderTimerLeft(0, value);
    }
    else if (strcmp("RStOvOrderTimerLeft", key) == 0)
    {
        page_update_StOvOrderTimerLeft(1, value);
    }
    else if (strcmp("LStOvMode", key) == 0)
    {
        page_update_StOvMode(0, value);
    }
    else if (strcmp("RStOvMode", key) == 0)
    {
        page_update_StOvMode(1, value);
    }
    else if (strcmp("LStOvSetTemp", key) == 0 || strcmp("LStOvSetTimer", key) == 0)
    {
        page_update_StOvSetTemp_StOvSetTimer(0, NULL);
    }
    else if (strcmp("RStOvSetTemp", key) == 0 || strcmp("RStOvSetTimer", key) == 0)
    {
        page_update_StOvSetTemp_StOvSetTimer(1, NULL);
    }
}
static void page_update_cb(void)
{
    LV_LOG_USER("%s\n", __func__);
    page_update_StOvState(0, NULL);
    page_update_StOvState(1, NULL);
    page_update_StOvRealTemp(0, NULL);
    page_update_StOvRealTemp(1, NULL);
    page_update_StOvSetTimerLeft(0, NULL);
    page_update_StOvSetTimerLeft(1, NULL);
    page_update_StOvOrderTimerLeft(0, NULL);
    page_update_StOvOrderTimerLeft(1, NULL);
    page_update_StOvMode(0, NULL);
    page_update_StOvMode(1, NULL);
    page_update_StOvSetTemp_StOvSetTimer(0, NULL);
    page_update_StOvSetTemp_StOvSetTimer(1, NULL);
}

static lv_obj_t *lv_text_btn_create(lv_obj_t *parent, const char *text, const void *user_data)
{
    lv_obj_t *btn1 = lv_btn_create(parent);
    lv_obj_set_size(btn1, 90, 45);
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0x191919), 0);
    lv_obj_set_style_bg_color(btn1, lv_color_hex(themesTextColor), LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(btn1, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn1, 45, 0);
    // lv_obj_add_event_cb(btn1, hood_speed_event_cb, LV_EVENT_CLICKED, user_data);

    lv_obj_t *label = lv_label_create(btn1);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x7C7C7C), 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_CHECKED);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return btn1;
}
static void finish_back_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    if (user_data == 0)
    {
        set_num_toServer("LStOvOperation", WORK_OPERATION_CONFIRM);
    }
    else
    {
        set_num_toServer("RStOvOperation", WORK_OPERATION_CONFIRM);
    }
}
static lv_obj_t *lv_steam_item_create(lv_obj_t *parent, const char index)
{
    // lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_t *obj = lv_img_create(parent);
    lv_img_set_src(obj, themesImagesPath "icon_steam_runing_background.png");
    lv_obj_set_size(obj, 290, 290);
    //------------------------------
    lv_obj_t *add_img = lv_img_create(obj); // 0
    lv_img_set_src(add_img, themesImagesPath "icon_cook_add.png");
    lv_obj_align(add_img, LV_ALIGN_TOP_MID, 0, 86);

    lv_obj_t *label = lv_label_create(obj); // 1
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(themesTextColor2), 0);
    if (index == 0)
        lv_label_set_text(label, "左腔");
    else
        lv_label_set_text(label, "右腔");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 183);
    //------------------------------
    lv_obj_t *label_state = lv_label_create(obj); // 2
    lv_obj_set_style_text_font(label_state, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_state, lv_color_hex(themesTextColor), 0);
    lv_obj_align(label_state, LV_ALIGN_TOP_MID, 0, 48);

    lv_obj_t *label_temp_time = lv_label_create(obj); // 3
    lv_obj_set_style_text_font(label_temp_time, &lv_font_SiYuanHeiTi_Normal_50, 0);
    lv_obj_set_style_text_color(label_temp_time, lv_color_hex(themesTextColor), 0);
    lv_obj_align(label_temp_time, LV_ALIGN_TOP_MID, 0, 98);

    lv_obj_t *label_mode = lv_label_create(obj); // 4
    lv_obj_set_style_text_font(label_mode, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_mode, lv_color_hex(themesTextColor2), 0);
    lv_obj_align(label_mode, LV_ALIGN_TOP_MID, 0, 161);

    lv_obj_t *label_set_temp_time = lv_label_create(obj); // 5
    lv_obj_set_style_text_font(label_set_temp_time, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_set_temp_time, lv_color_hex(themesTextColor2), 0);
    lv_obj_align(label_set_temp_time, LV_ALIGN_TOP_MID, 0, 200);

    lv_obj_t *arc = lv_arc_create(obj); // 6
    lv_obj_set_size(arc, LV_PCT(100), LV_PCT(100));
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(arc);
    lv_obj_set_style_arc_color(arc, lv_color_hex(themesTextColor), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(arc, 20, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(arc, true, LV_PART_INDICATOR);
    lv_arc_set_value(arc, 50);

    lv_obj_t *run_img = lv_img_create(obj); // 7
    lv_img_set_src(run_img, themesImagesPath "icon_steam_runing.png");
    lv_obj_align(run_img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_update_layout(run_img);
    lv_img_set_pivot(run_img, lv_obj_get_width(run_img) / 2, lv_obj_get_height(run_img) / 2);
    //------------------------------
    lv_obj_t *label_finish_mode = lv_label_create(obj); // 8
    lv_obj_set_style_text_font(label_finish_mode, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_finish_mode, lv_color_hex(themesTextColor2), 0);
    lv_obj_align(label_finish_mode, LV_ALIGN_TOP_MID, 0, 75);

    lv_obj_t *label_finish = lv_label_create(obj); // 9
    lv_obj_set_style_text_font(label_finish, &lv_font_SiYuanHeiTi_Normal_50, 0);
    lv_obj_set_style_text_color(label_finish, lv_color_hex(themesTextColor), 0);
    lv_obj_align(label_finish, LV_ALIGN_TOP_MID, 0, 123);
    lv_label_set_text(label_finish, "烹饪已完成");

    lv_obj_t *label_finish_btn = lv_btn_create(obj); // 10
    lv_obj_set_size(label_finish_btn, 140, 50);
    lv_obj_set_style_bg_color(label_finish_btn, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(label_finish_btn, LV_OPA_100, 0);
    lv_obj_set_style_radius(label_finish_btn, 25, 0);
    lv_obj_align(label_finish_btn, LV_ALIGN_TOP_MID, 0, 186);
    lv_obj_add_event_cb(label_finish_btn, finish_back_event_cb, LV_EVENT_CLICKED, (void *)index);

    lv_obj_t *label_finish_btn_text = lv_label_create(label_finish_btn);
    lv_obj_set_style_text_font(label_finish_btn_text, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_finish_btn_text, lv_color_hex(0x000000), 0);
    lv_label_set_text(label_finish_btn_text, "返回");
    lv_obj_center(label_finish_btn_text);
    return obj;
}
static void left_dialog2_event_cb(lv_event_t *e)
{
    int user_data = (int)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
        set_num_toServer("LStOvOperation", WORK_OPERATION_CANCEL);
        break;
    }
    lv_obj_clean(lv_layer_top());
}
static void left_btn_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        if (work_state[0] == WORK_STATE_STOP)
        {
        }
        else if (work_state[0] == WORK_STATE_PAUSE || work_state[0] == WORK_STATE_PAUSE_RESERVE)
        {
            set_num_toServer("LStOvOperation", WORK_OPERATION_START);
        }
        else
        {
            set_num_toServer("LStOvOperation", WORK_OPERATION_PAUSE);
        }
        break;
    case 1:
        if (work_state[0] == WORK_STATE_STOP)
        {
        }
        else
        {
            lv_custom_dialog2("是否取消左腔烹饪？", "否", "是", left_dialog2_event_cb);
        }
        break;
    case 2:
        if (work_state[0] == WORK_STATE_STOP)
        {
        }
        else
        {
            set_num_toServer("LStOvOperation", WORK_OPERATION_RUN_NOW);
        }
        break;
    }
}
static void right_btn_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        if (work_state[1] == WORK_STATE_STOP)
        {
        }
        else if (work_state[1] == WORK_STATE_PAUSE || work_state[1] == WORK_STATE_PAUSE_RESERVE)
        {
            set_num_toServer("RStOvOperation", WORK_OPERATION_START);
        }
        else
        {
            set_num_toServer("RStOvOperation", WORK_OPERATION_PAUSE);
        }
        break;
    case 1:
        if (work_state[1] == WORK_STATE_STOP)
        {
        }
        else
        {
            set_num_toServer("RStOvOperation", WORK_OPERATION_CANCEL);
        }
        break;
    case 2:
        if (work_state[1] == WORK_STATE_STOP)
        {
        }
        else
        {
            set_num_toServer("RStOvOperation", WORK_OPERATION_RUN_NOW);
        }
        break;
    }
}
static lv_obj_t *lv_btn_array_create(lv_obj_t *parent, const char count, lv_event_cb_t event_cb)
{
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 140, 55 * count + 40);
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
        lv_obj_center(label);
    }
    return cont;
}
static void back_bar_event_cb(lv_event_t *e)
{
    if (lv_page_exist_page("page_steamoven"))
        lv_page_back_page("page_steamoven");
    else
        lv_page_back_top_page();
}
void lv_page_steaming_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;

    lv_obj_t *back_bar = lv_page_back_bar_init(page, "蒸烤箱", NULL, back_bar_event_cb);

    lv_obj_t *cont_row = lv_obj_create(page);
    lv_obj_set_size(cont_row, 290 * 2 + 112, 290);
    lv_obj_align_to(cont_row, back_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    left_steaming = lv_steam_item_create(cont_row, 0);
    for (int i = 0; i < 11; ++i)
        left_child[i] = lv_obj_get_child(left_steaming, i);

    right_steaming = lv_steam_item_create(cont_row, 1);
    for (int i = 0; i < 11; ++i)
        right_child[i] = lv_obj_get_child(right_steaming, i);

    btn_array[0] = lv_btn_array_create(page, 3, left_btn_event_cb);
    lv_obj_align(btn_array[0], LV_ALIGN_LEFT_MID, 52, 25);
    btn_array[1] = lv_btn_array_create(page, 3, right_btn_event_cb);
    lv_obj_align(btn_array[1], LV_ALIGN_RIGHT_MID, -52, 25);
}
