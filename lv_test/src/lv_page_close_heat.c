

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *left_heat, *right_heat;
static lv_obj_t *btn_array[2];
static char timing_state[2];
/**********************
 *  STATIC VARIABLES
 **********************/
static void btn_array_update(const int index, char *text[], const int count)
{
    lv_obj_t *child;
    int i;
    for (i = 0; i < count; ++i)
    {
        child = lv_obj_get_child(btn_array[index], i);
        lv_obj_clear_flag(child, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(lv_obj_get_child(child, 0), text[i]);
    }
}
static void page_update_StoveTimingState(const int index, void *ptr)
{
    int value;
    lv_obj_t *child;
    if (index == 0)
    {
        if (ptr == NULL)
            value = get_attr_value_int("LStoveTimingState");
        child = left_heat;
    }
    else
    {
        if (ptr == NULL)
            value = get_attr_value_int("RStoveTimingState");
        child = right_heat;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);
    timing_state[index] = value;
}

static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("key:%s\n", key);
    if (strcmp("LStoveTimingState", key) == 0)
    {
        page_update_StoveTimingState(0, value);
    }
    else if (strcmp("RStoveTimingState", key) == 0)
    {
        page_update_StoveTimingState(1, value);
    }
}
static void page_update_cb(void* arg)
{
    page_update_StoveTimingState(0, NULL);
    page_update_StoveTimingState(1, NULL);
}

static lv_obj_t *lv_custom_heat_create(lv_obj_t *parent, int index)
{
    lv_obj_t *obj = lv_img_create(parent);
    lv_img_set_src(obj, themesImagesPath "icon_close_heat_background.png");
    lv_obj_set_size(obj, 309, 309);

    lv_obj_t *label = lv_label_create(obj); // 0
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    if (index == 0)
        lv_label_set_text(label, "左灶");
    else
        lv_label_set_text(label, "右灶");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 105);

    label = lv_label_create(obj); // 1
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, "定时关火");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 157);

    label = lv_label_create(obj); // 2
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_50, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(themesTextColor), 0);
    lv_label_set_text(label, "12:34");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 135);

    lv_obj_t *arc = lv_arc_create(obj); // 3
    lv_obj_set_size(arc, 240, 240);
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(arc);
    lv_obj_set_style_arc_color(arc, lv_color_hex(themesTextColor), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(arc, 20, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(arc, true, LV_PART_INDICATOR);
    lv_arc_set_value(arc, 50);
    return obj;
}
static void left_dialog1_event_cb(lv_event_t *e)
{
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
        set_num_toServer("LStoveTimingOpera", WORK_OPERATION_CANCEL);
        break;
    }
    clean_manual_layer();
}
static void right_dialog1_event_cb(lv_event_t *e)
{
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
        set_num_toServer("RStoveTimingOpera", WORK_OPERATION_CANCEL);
        break;
    }
    clean_manual_layer();
}
static void left_btn_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        if (timing_state[0] == TIMING_STATE_RUN)
        {
        }

        break;
    case 1:
        if (timing_state[0] == TIMING_STATE_RUN)
        {
            lv_manual_dialog1("是否取消左灶定时关火？", "否", "是", left_dialog1_event_cb);
        }
        break;
    }
}
static void right_btn_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        if (timing_state[1] == TIMING_STATE_RUN)
        {
        }
        break;
    case 1:
        if (timing_state[1] == TIMING_STATE_RUN)
        {
            lv_manual_dialog1("是否取消右灶定时关火？", "否", "是", right_dialog1_event_cb);
        }
        break;
    }
}
void lv_page_close_heat_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;

    lv_obj_t *back_bar = lv_page_back_bar_init(page, "定时关火", NULL, NULL);

    lv_obj_t *cont_row = lv_obj_create(page);
    lv_obj_set_size(cont_row, 309 * 2 + 124, 309);
    lv_obj_align_to(cont_row, back_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    left_heat = lv_custom_heat_create(cont_row, 0);
    right_heat = lv_custom_heat_create(cont_row, 1);

    btn_array[0] = lv_btn_array_create(page, 2, left_btn_event_cb);
    lv_obj_align(btn_array[0], LV_ALIGN_LEFT_MID, 52, 25);
    btn_array[1] = lv_btn_array_create(page, 2, right_btn_event_cb);
    lv_obj_align(btn_array[1], LV_ALIGN_RIGHT_MID, -52, 25);
    char *btn_text[2];
    btn_text[0] = "重置";
    btn_text[1] = "取消";
    btn_array_update(0, btn_text, 2);
    btn_array_update(1, btn_text, 2);
}
