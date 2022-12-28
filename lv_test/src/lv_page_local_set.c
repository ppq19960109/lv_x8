

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
static void switch_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *user_data = lv_event_get_user_data(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        // LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        lv_obj_t *label1_sw = lv_obj_get_child(obj, 0);
        if (lv_obj_has_state(obj, LV_STATE_CHECKED))
        {
            lv_obj_set_style_text_color(label1_sw, lv_color_hex(0xffffff), 0);
            lv_label_set_text(label1_sw, "开");
            lv_obj_align(label1_sw, LV_ALIGN_CENTER, -20, 0);
            lv_obj_clear_flag(user_data, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_set_style_text_color(label1_sw, lv_color_hex(themesTextColor2), 0);
            lv_label_set_text(label1_sw, "关");
            lv_obj_align(label1_sw, LV_ALIGN_CENTER, 20, 0);
            lv_obj_add_flag(user_data, LV_OBJ_FLAG_HIDDEN);
        }
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

    lv_obj_t *label_light = lv_label_create(light_obj);
    lv_obj_set_style_text_font(label_light, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_light, "亮度");
    lv_obj_set_style_text_color(label_light, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_light, LV_ALIGN_LEFT_MID, 10, 0);

    lv_obj_t *light_img = lv_img_create(light_obj);
    lv_obj_align(light_img, LV_ALIGN_LEFT_MID, 100, 0);
    lv_img_set_src(light_img, themesImagesPath "icon_light_small.png");
    lv_obj_t *light_img2 = lv_img_create(light_obj);
    lv_obj_align(light_img2, LV_ALIGN_RIGHT_MID, -90, 0);
    lv_img_set_src(light_img2, themesImagesPath "icon_light_big.png");

    lv_obj_t *light_slider = lv_slider_create(light_obj);
    lv_obj_set_size(light_slider, 590, 10);
    lv_obj_align(light_slider, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(light_slider, 1, 255);
    lv_slider_set_value(light_slider, 200, LV_ANIM_ON);

    lv_obj_add_style(light_slider, &slider_style_main, LV_PART_MAIN);
    lv_obj_add_style(light_slider, &slider_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(light_slider, &slider_style_knob, LV_PART_KNOB);

    lv_obj_add_event_cb(light_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, 0);
    //-------------------------------------------------------------
    lv_obj_t *sleep_switch_obj = lv_obj_create(cont_col);
    lv_obj_set_size(sleep_switch_obj, LV_PCT(100), 100);
    lv_obj_t *label_sleep_switch = lv_label_create(sleep_switch_obj);
    lv_obj_set_style_text_font(label_sleep_switch, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_sleep_switch, "息屏");
    lv_obj_set_style_text_color(label_sleep_switch, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_sleep_switch, LV_ALIGN_LEFT_MID, 10, 0);

    lv_obj_t *sw = lv_switch_create(sleep_switch_obj);
    lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -80, 0);
    lv_obj_set_size(sw, 100, 46);

    lv_obj_t *label1_sw = lv_label_create(sw);
    lv_obj_set_style_text_font(label1_sw, &lv_font_SiYuanHeiTi_Normal_26, 0);
    lv_obj_set_style_text_color(label1_sw, lv_color_hex(themesTextColor2), 0);
    lv_label_set_text(label1_sw, "关");
    lv_obj_align(label1_sw, LV_ALIGN_CENTER, 20, 0);

    lv_obj_add_style(sw, &switch_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(sw, &switch_style_indicator_check, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_add_style(sw, &switch_style_knob, LV_PART_KNOB);

    //-------------------------------------------------------------
    lv_obj_t *sleep_obj = lv_obj_create(cont_col);
    lv_obj_set_size(sleep_obj, LV_PCT(100), 100);

    lv_obj_add_event_cb(sw, switch_event_handler, LV_EVENT_VALUE_CHANGED, sleep_obj);

    lv_obj_t *label_sleep = lv_label_create(sleep_obj);
    lv_obj_set_style_text_font(label_sleep, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_sleep, "1分钟");
    lv_obj_set_style_text_color(label_sleep, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_sleep, LV_ALIGN_LEFT_MID, 50, 0);

    lv_obj_t *label_sleep2 = lv_label_create(sleep_obj);
    lv_obj_set_style_text_font(label_sleep2, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_sleep2, "5分钟");
    lv_obj_set_style_text_color(label_sleep2, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_sleep2, LV_ALIGN_RIGHT_MID, -70, 0);

    lv_obj_t *sleep_slider = lv_slider_create(sleep_obj);
    lv_obj_set_size(sleep_slider, 590, 10);
    lv_obj_align(sleep_slider, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(sleep_slider, 1, 5);
    lv_slider_set_value(sleep_slider, 3, LV_ANIM_ON);

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
    char buf[16];
    lv_snprintf(buf, sizeof(buf), "%d分钟", (int)lv_slider_get_value(sleep_slider));
    lv_label_set_text(sleep_slider_label, buf);
    //----------------------------------
    lv_obj_t *time_obj = lv_obj_create(cont_col);
    lv_obj_set_size(time_obj, LV_PCT(100), 100);
    lv_obj_t *label_time = lv_label_create(time_obj);
    lv_obj_set_style_text_font(label_time, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_time, "时间");
    lv_obj_set_style_text_color(label_time, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_time, LV_ALIGN_LEFT_MID, 10, 0);
}
