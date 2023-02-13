

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *hood_speed_obj[3];
static lv_obj_t *stir_fried_rotate;

static lv_obj_t *smart_smoke_rotate;
static lv_obj_t *sw_obj[4];
/**********************
 *  STATIC VARIABLES
 **********************/
static void switch_update(int index, int value)
{
    if (value)
    {
        lv_obj_add_state(sw_obj[index], LV_STATE_CHECKED);
        lv_label_set_text(lv_obj_get_child(sw_obj[index], 0), "开");
    }
    else
    {
        lv_obj_clear_state(sw_obj[index], LV_STATE_CHECKED);
        lv_label_set_text(lv_obj_get_child(sw_obj[index], 0), "关");
    }
}
static void page_update_RAuxiliarySwitch(void *ptr)
{
    int value;
    if (ptr == NULL)
        value = get_attr_value_int("RAuxiliarySwitch");
    else
        value = get_value_int(ptr);
    switch_update(0, value);
}
static void page_update_CookingCurveSwitch(void *ptr)
{
    int value;
    if (ptr == NULL)
        value = get_attr_value_int("CookingCurveSwitch");
    else
        value = get_value_int(ptr);
    switch_update(1, value);
}
static void page_update_OilTempSwitch(void *ptr)
{
    int value;
    if (ptr == NULL)
        value = get_attr_value_int("OilTempSwitch");
    else
        value = get_value_int(ptr);
    switch_update(2, value);
}
static void page_update_RMovePotLowHeatSwitch(void *ptr)
{
    int value;
    if (ptr == NULL)
        value = get_attr_value_int("RMovePotLowHeatSwitch");
    else
        value = get_value_int(ptr);
    switch_update(3, value);
}
static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("key:%s\n", key);
    if (strcmp("RAuxiliarySwitch", key) == 0)
    {
        page_update_RAuxiliarySwitch(value);
    }
    else if (strcmp("CookingCurveSwitch", key) == 0)
    {
        page_update_CookingCurveSwitch(value);
    }
    else if (strcmp("OilTempSwitch", key) == 0)
    {
        page_update_OilTempSwitch(value);
    }
    else if (strcmp("RMovePotLowHeatSwitch", key) == 0)
    {
        page_update_RMovePotLowHeatSwitch(value);
    }
}
static void page_update_cb(void)
{
    page_update_RAuxiliarySwitch(NULL);
    page_update_CookingCurveSwitch(NULL);
    page_update_OilTempSwitch(NULL);
    page_update_RMovePotLowHeatSwitch(NULL);
}
static void switch_event_handler(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    bool state = lv_obj_has_state(target, LV_STATE_CHECKED);
    switch (user_data)
    {
    case 0:
    {
        set_num_toServer("RAuxiliarySwitch", state);
    }
    break;
    case 1:
        break;
    case 2:
        set_num_toServer("OilTempSwitch", state);
        break;
    }
}
lv_obj_t *lv_custom_switch_create(lv_obj_t *parent, int index)
{
    lv_obj_t *sw = lv_switch_create(parent);
    lv_obj_set_size(sw, 140, 50);

    lv_obj_t *label1_sw = lv_label_create(sw);
    lv_obj_set_style_text_font(label1_sw, &lv_font_SiYuanHeiTi_Normal_26, 0);
    lv_obj_set_style_text_color(label1_sw, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label1_sw, "关");
    lv_obj_align(label1_sw, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_style(sw, &switch_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(sw, &switch_style_indicator_check, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_add_style(sw, &switch_style_knob, LV_PART_KNOB);

    lv_obj_add_event_cb(sw, switch_event_handler, LV_EVENT_VALUE_CHANGED, index);
    return sw;
}

void lv_page_smart_cook_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;

    lv_obj_t *back_bar = lv_page_back_bar_init(page, "智慧烹饪", NULL, NULL);

    lv_obj_t *cont_row = lv_obj_create(page);
    lv_obj_set_size(cont_row, 314 + 204 * 3 + 40 * 2, 282);
    lv_obj_align_to(cont_row, back_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *auxiliary_temp_control = lv_img_create(cont_row);
    lv_img_set_src(auxiliary_temp_control, themesImagesPath "auxiliary_temp_control.png");
    lv_obj_set_size(auxiliary_temp_control, 314, LV_PCT(100));
    lv_obj_t *label_title = lv_label_create(auxiliary_temp_control);
    lv_obj_set_style_text_font(label_title, &lv_font_SiYuanHeiTi_Normal_34, 0);
    lv_obj_set_style_text_color(label_title, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_title, "右灶 | 辅助控火");
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_t *sw = lv_custom_switch_create(auxiliary_temp_control, 0);
    lv_obj_align(sw, LV_ALIGN_BOTTOM_MID, 0, -30);
    sw_obj[0] = sw;

    lv_obj_t *cooking_curve = lv_img_create(cont_row);
    lv_img_set_src(cooking_curve, themesImagesPath "cooking_curve.png");
    lv_obj_set_size(cooking_curve, 204, LV_PCT(100));
    label_title = lv_label_create(cooking_curve);
    lv_obj_set_style_text_font(label_title, &lv_font_SiYuanHeiTi_Normal_34, 0);
    lv_obj_set_style_text_color(label_title, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_title, "烹饪曲线");
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 30);
    sw = lv_custom_switch_create(cooking_curve, 1);
    lv_obj_align(sw, LV_ALIGN_BOTTOM_MID, 0, -30);
    sw_obj[1] = sw;

    lv_obj_t *oilTemp = lv_img_create(cont_row);
    lv_img_set_src(oilTemp, themesImagesPath "oil_temp.png");
    lv_obj_set_size(oilTemp, 204, LV_PCT(100));
    label_title = lv_label_create(oilTemp);
    lv_obj_set_style_text_font(label_title, &lv_font_SiYuanHeiTi_Normal_34, 0);
    lv_obj_set_style_text_color(label_title, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_title, "油温显示");
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 30);
    sw = lv_custom_switch_create(oilTemp, 2);
    lv_obj_align(sw, LV_ALIGN_BOTTOM_MID, 0, -30);
    sw_obj[2] = sw;

    lv_obj_t *lowHeat = lv_img_create(cont_row);
    lv_img_set_src(lowHeat, themesImagesPath "movePot_lowHeat.png");
    lv_obj_set_size(lowHeat, 204, LV_PCT(100));
    label_title = lv_label_create(lowHeat);
    lv_obj_set_style_text_font(label_title, &lv_font_SiYuanHeiTi_Normal_34, 0);
    lv_obj_set_style_text_color(label_title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_align(label_title, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_title, "右灶\n移锅小火");
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 30);
    sw = lv_custom_switch_create(lowHeat, 3);
    lv_obj_align(sw, LV_ALIGN_BOTTOM_MID, 0, -30);
    sw_obj[3] = sw;
}
