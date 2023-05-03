

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_roki/lv_roki.h"
/*********************
 *      DEFINES
 *********************/
static lv_obj_t *sleep_slider_label, *sw, *sleep_obj;
static lv_obj_t *cont_col, *screensaver;
static lv_obj_t *light_slider;
/**********************
 *  STATIC VARIABLES
 **********************/
void lv_page_local_set_visible(const int visible)
{
    LV_LOG_USER("%s,brightness:%d\n", __func__, g_save_settings.brightness);
    if (visible)
    {
        // switch_value_state(sw, g_save_settings.sleepSwitch);
        if (g_save_settings.sleepSwitch)
            lv_obj_clear_flag(sleep_obj, LV_OBJ_FLAG_HIDDEN);
        else
            lv_obj_add_flag(sleep_obj, LV_OBJ_FLAG_HIDDEN);

        lv_obj_scroll_to_y(cont_col, 0, LV_ANIM_OFF);
        lv_slider_set_value(light_slider, g_save_settings.brightness, LV_ANIM_OFF);
        lv_obj_clear_flag(cont_col, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(screensaver, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {

        lv_obj_add_flag(cont_col, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(screensaver, LV_OBJ_FLAG_HIDDEN);
    }
}

static void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    int slider_value = lv_slider_get_value(slider);
    LV_LOG_USER("%s,code:%d slider value:%d user_data:%ld\n", __func__, e->code, slider_value, user_data);
    if (user_data == 0)
    {
        g_save_settings.brightness = slider_value;
        backlightSet(g_save_settings.brightness);
        H_Kv_Set("brightness", &g_save_settings.brightness, 1, 0);
    }
    else if (user_data == 1)
    {
        char buf[16];
        lv_snprintf(buf, sizeof(buf), "%d分钟", slider_value);
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
        if (lv_obj_has_state(obj, LV_STATE_CHECKED))
        {
            g_save_settings.sleepSwitch = 1;
            lv_obj_clear_flag(user_data, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            g_save_settings.sleepSwitch = 0;
            lv_obj_add_flag(user_data, LV_OBJ_FLAG_HIDDEN);
        }
        // switch_value_state(sw, g_save_settings.sleepSwitch);
        H_Kv_Set("sleepSwitch", &g_save_settings.sleepSwitch, 1, 0);
    }
}
static void btn_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        break;
    case 1:
        lv_page_local_set_visible(0);
        break;
    case 2:
        lv_page_local_set_visible(1);
        break;
    }
}
void lv_page_local_set_create(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    char buf[96];

    cont_col = lv_obj_create(page);
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

    light_slider = lv_slider_create(light_obj);
    lv_obj_set_size(light_slider, 590, 15);
    lv_obj_align(light_slider, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(light_slider, 1, 255);
    lv_slider_set_value(light_slider, 200, LV_ANIM_OFF);

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

    sw = lv_switch_create(sleep_switch_obj);
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
    sleep_obj = lv_obj_create(cont_col);
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
    lv_obj_set_size(sleep_slider, 590, 15);
    lv_obj_align(sleep_slider, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(sleep_slider, 1, 5);
    lv_slider_set_value(sleep_slider, 3, LV_ANIM_OFF);

    lv_obj_add_style(sleep_slider, &slider_style_main, LV_PART_MAIN);
    lv_obj_add_style(sleep_slider, &slider_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(sleep_slider, &slider_style_knob, LV_PART_KNOB);

    lv_obj_add_event_cb(sleep_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, (void *)1);
    // lv_obj_add_flag(sleep_slider, LV_OBJ_FLAG_ADV_HITTEST);
    // lv_slider_set_mode(sleep_slider, LV_SLIDER_MODE_NORMAL);
    sleep_slider_label = lv_label_create(sleep_obj);
    lv_obj_align_to(sleep_slider_label, sleep_slider, LV_ALIGN_OUT_TOP_MID, 0, -15);
    lv_obj_set_style_text_font(sleep_slider_label, &lv_font_SiYuanHeiTi_Normal_24, 0);
    lv_obj_set_style_text_color(sleep_slider_label, lv_color_hex(0xffffff), 0);

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
    lv_obj_add_event_cb(time_obj, btn_event_cb, LV_EVENT_CLICKED, 0);

    lv_obj_t *label_time2 = lv_label_create(time_obj);
    lv_obj_set_style_text_font(label_time2, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_time2, "13:14");
    lv_obj_set_style_text_color(label_time2, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_time2, LV_ALIGN_RIGHT_MID, -100, 0);
    //----------------------------------
    lv_obj_t *screen_obj = lv_obj_create(cont_col);
    lv_obj_set_size(screen_obj, LV_PCT(100), 100);
    lv_obj_t *label_screen = lv_label_create(screen_obj);
    lv_obj_set_style_text_font(label_screen, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_screen, "屏保");
    lv_obj_set_style_text_color(label_screen, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_screen, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_add_event_cb(screen_obj, btn_event_cb, LV_EVENT_CLICKED, (void *)1);

    lv_obj_t *label_screen2 = lv_label_create(screen_obj);
    lv_obj_set_style_text_font(label_screen2, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_screen2, "时钟");
    lv_obj_set_style_text_color(label_screen2, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_screen2, LV_ALIGN_RIGHT_MID, -100, 0);
    //--------------------------------------------------------------------------------
    screensaver = lv_obj_create(page);
    lv_obj_add_flag(screensaver, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(screensaver, LV_PCT(100), LV_PCT(100));
    lv_obj_t *back_button = lv_custom_back_button_create(screensaver, themesImagesPath "back_button_background.png", "屏保");
    lv_obj_add_event_cb(back_button, btn_event_cb, LV_EVENT_CLICKED, (void *)2);
    static lv_coord_t col_dsc[] = {438, 438, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {116, 116, LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    lv_obj_t *cont = lv_obj_create(screensaver);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);
    // lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 438 * 2 + 25, 116 * 2 + 18);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 74);
    lv_obj_set_grid_align(cont, LV_GRID_ALIGN_SPACE_BETWEEN, LV_GRID_ALIGN_SPACE_BETWEEN);

    lv_obj_t *element_obj, *element;

    for (int i = 0; i < 4; i++)
    {
        uint8_t col = i % 2;
        uint8_t row = i / 2;

        element_obj = lv_obj_create(cont);
        // lv_obj_set_size(element_obj, 418 , 116);
        lv_obj_set_grid_cell(element_obj, LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);

        element = lv_img_create(element_obj);
        sprintf(buf, themesImagesPath "screen_saver%d_narrow.png", i);
        lv_img_set_src(element, buf);

        element = radiobutton_create(element_obj, "");
        lv_obj_align(element, LV_ALIGN_RIGHT_MID, 0, 0);
    }
}
