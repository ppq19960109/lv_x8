

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"
/*********************
 *      DEFINES
 *********************/
static lv_obj_t *sleep_slider_label;
/**********************
 *  STATIC VARIABLES
 **********************/

static void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int user_data = lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d slider value:%d user_data:%d\n", __func__, e->code, lv_slider_get_value(slider), user_data);
    if (user_data == 1)
    {
        char buf[16];
        lv_snprintf(buf, sizeof(buf), "%d分钟", (int)lv_slider_get_value(slider));
        lv_label_set_text(sleep_slider_label, buf);
    }
}

void lv_page_local_set_create(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t *cont_col = lv_obj_create(page);
    lv_obj_set_size(cont_col, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont_col, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *light_obj = lv_obj_create(cont_col);
    lv_obj_set_size(light_obj, LV_PCT(100), 100);
    lv_obj_t *light_slider = lv_slider_create(light_obj);
    lv_obj_set_size(light_slider, 590, 10);
    lv_obj_align(light_slider, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(light_slider, 1, 255);

    lv_obj_add_style(light_slider, &slider_style_main, LV_PART_MAIN);
    lv_obj_add_style(light_slider, &slider_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(light_slider, &slider_style_knob, LV_PART_KNOB);

    lv_obj_add_event_cb(light_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, 0);

    lv_obj_t *sleep_obj = lv_obj_create(cont_col);
    lv_obj_set_size(sleep_obj, LV_PCT(100), 100);
    lv_obj_t *sleep_slider = lv_slider_create(sleep_obj);
    lv_obj_set_size(sleep_slider, 590, 10);
    lv_obj_align(sleep_slider, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(sleep_slider, 1, 5);

    lv_obj_add_style(sleep_slider, &slider_style_main, LV_PART_MAIN);
    lv_obj_add_style(sleep_slider, &slider_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(sleep_slider, &slider_style_knob, LV_PART_KNOB);

    lv_obj_add_event_cb(sleep_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, 1);
    // lv_obj_add_flag(sleep_slider, LV_OBJ_FLAG_ADV_HITTEST);
    // lv_slider_set_mode(sleep_slider, LV_SLIDER_MODE_NORMAL);
    sleep_slider_label = lv_label_create(sleep_obj);
    lv_obj_align_to(sleep_slider_label, sleep_slider, LV_ALIGN_OUT_TOP_MID, 0, -15);
    lv_obj_set_style_text_font(sleep_slider_label, &lv_font_SiYuanHeiTi_Normal_24, 0);
    lv_obj_set_style_text_color(sleep_slider_label, lv_color_hex(0xffffff), 0);
}
