

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"
/*********************
 *      DEFINES
 *********************/
static timer_t wifi_timer;
static lv_obj_t *wifi_list;
/**********************
 *  STATIC VARIABLES
 **********************/
static void POSIXTimer_cb(union sigval val)
{
    LV_LOG_USER("%s sival_int:%d", __func__, val.sival_int);
    if (val.sival_int == 0)
    {
        set_num_toServer("WifiScan", -1);
        get_toServer("WifiScanR");
    }
}
static void switch_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        // LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        lv_obj_t *label1_sw = lv_obj_get_child(obj, 0);
        if (lv_obj_has_state(obj, LV_STATE_CHECKED))
        {
            lv_obj_set_style_text_color(label1_sw, lv_color_hex(0xffffff), 0);
            lv_label_set_text(label1_sw, "开");
            lv_obj_align(label1_sw, LV_ALIGN_CENTER, -20, 0);
        }
        else
        {
            lv_obj_set_style_text_color(label1_sw, lv_color_hex(themesTextColor2), 0);
            lv_label_set_text(label1_sw, "关");
            lv_obj_align(label1_sw, LV_ALIGN_CENTER, 20, 0);
        }
    }
}
static lv_obj_t *lv_divider_create(lv_obj_t *parent)
{
    lv_obj_t *divider = lv_obj_create(parent);
    lv_obj_set_size(divider, LV_PCT(100), 2);
    lv_obj_set_style_bg_opa(divider, 15, 0);
    lv_obj_set_style_bg_color(divider, lv_color_hex(0xE7E7E7), 0);
    return divider;
}
void lv_wifi_list_clean(void)
{
    lv_obj_clean(wifi_list);
}
lv_obj_t *lv_wifi_list_create(const char *ssid, const int rssi, const char *flags)
{
    lv_obj_t *obj = lv_obj_create(wifi_list);
    lv_obj_set_size(obj, LV_PCT(100), 70);

    lv_obj_t *divider = lv_divider_create(obj);
    lv_obj_align(divider, LV_ALIGN_BOTTOM_MID, 0, 80);

    lv_obj_t *label = lv_label_create(obj);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, ssid);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 30, 35);

    int signal = signalLevel(rssi);
    lv_obj_t *img_signal = lv_img_create(obj);
    lv_obj_align(img_signal, LV_ALIGN_TOP_RIGHT, -40, 35);
    lv_img_set_src(img_signal, themesImagesPath "wifi/wifi1.png");
    switch (signal)
    {
    case 0:

    case 1:
        break;
    case 2:

        break;
    }
    return obj;
}
void lv_page_wifi_visible(const int visible)
{
    if (visible)
    {
        POSIXTimerSet(wifi_timer, 0, 5);
    }
    else
    {
        POSIXTimerSet(wifi_timer, 0, 0);
    }
}
void lv_page_wifi_create(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    wifi_timer = POSIXTimerCreate(0, POSIXTimer_cb);

    lv_obj_t *head = lv_obj_create(page);
    lv_obj_set_size(head, LV_PCT(100), 140);
    lv_obj_align(head, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_t *content = lv_list_create(page);
    lv_obj_set_size(content, LV_PCT(100), 260);
    lv_obj_align(content, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *divider = lv_divider_create(head);
    lv_obj_align(divider, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_t *label = lv_label_create(head);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, "WIFI");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 30, 30);

    lv_obj_t *sw = lv_switch_create(head);
    lv_obj_align(sw, LV_ALIGN_TOP_RIGHT, -114, 25);
    lv_obj_set_size(sw, 100, 46);

    lv_obj_t *label1_sw = lv_label_create(sw);
    lv_obj_set_style_text_font(label1_sw, &lv_font_SiYuanHeiTi_Normal_26, 0);
    lv_obj_set_style_text_color(label1_sw, lv_color_hex(themesTextColor2), 0);
    lv_label_set_text(label1_sw, "关");
    lv_obj_align(label1_sw, LV_ALIGN_CENTER, 20, 0);

    lv_obj_set_style_bg_opa(sw, LV_OPA_100, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(sw, lv_color_hex(0x333333), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(sw, lv_color_hex(themesTextColor), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_radius(sw, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);

    lv_obj_set_style_bg_opa(sw, LV_OPA_COVER, LV_PART_KNOB);
    lv_obj_set_style_bg_color(sw, lv_color_hex(0xfffff), LV_PART_KNOB);
    lv_obj_set_style_radius(sw, LV_RADIUS_CIRCLE, LV_PART_KNOB);
    lv_obj_set_style_pad_all(sw, -3, LV_PART_KNOB);

    lv_obj_add_event_cb(sw, switch_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    wifi_list = lv_list_create(content);

    lv_obj_set_size(wifi_list, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_align(wifi_list, LV_ALIGN_TOP_MID, 0, 0);
}
