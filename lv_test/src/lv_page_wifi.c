

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"
/*********************
 *      DEFINES
 *********************/
typedef struct
{
    char ssid[33];
    int flags;
} wifi_info_t;
static timer_t wifi_timer;
static lv_obj_t *wifi_list;

static char *bssid;
static char scan_count, scan_min_num = 2;
static lv_obj_t *lv_wifi_list_create(const char *ssid, const int rssi, const int flags);
static void lv_wifi_list_clean(void);
/**********************
 *  STATIC VARIABLES
 **********************/
static int wifi_list_item(void *arg)
{
    wifi_node_t *ptr = arg;
    // LV_LOG_USER("%s,ssid:%s,rssi:%d,flags:%d\n", __func__, ptr->ssid, ptr->rssi, ptr->flags);
    lv_obj_t *obj = lv_wifi_list_create(ptr->ssid, ptr->rssi, ptr->flags);
    if (bssid == NULL)
        LV_LOG_USER("%s,bssid,null\n", __func__);
    else
    {
        if (strcmp(bssid, ptr->bssid) == 0)
        {
            lv_obj_move_to_index(obj, 0);
            lv_obj_add_state(lv_obj_get_child(obj, 0), LV_STATE_CHECKED);
        }
    }
    return 0;
}
void lv_wifi_property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("%s,key:%s\n", __func__, key);
    if (strcmp("WifiScanR", key) == 0)
    {
        bssid = get_attr_value_string("bssid");
        lv_wifi_list_clean();
        wifi_list_each(wifi_list_item);
        // lv_wifi_list_create("ASDF", -46, "WPA-PSK-CCMP+TKIP");
    }
}

static void POSIXTimer_cb(union sigval val)
{
    LV_LOG_USER("%s sival_int:%d", __func__, val.sival_int);
    if (val.sival_int == 0)
    {
        ++scan_count;
        if (scan_count <= scan_min_num)
        {
            if (scan_count == 1)
            {
                get_toServer("WifiCurConnected");
                set_num_toServer("WifiScan", -1);
            }
            else if (scan_count == scan_min_num)
            {
                POSIXTimerSet(wifi_timer, 9, 9);
            }
            get_toServer("WifiScanR");
        }
        else
        {
            get_toServer("WifiCurConnected");
            if (scan_count % 2 == 0)
                set_num_toServer("WifiScan", -1);
            else
                get_toServer("WifiScanR");
        }
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
static void wifi_input_event_handler(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    int user_data = lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        lv_obj_clean(lv_layer_top());
        break;
    case 1:
        break;
    case 2:
        break;
    }
}
static void ta_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    lv_obj_t *kb = lv_event_get_user_data(e);

    if (code == LV_EVENT_FOCUSED)
    {
        if (lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD)
        {
            LV_LOG_USER("%s,code:%d\n", __func__, e->code);
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)
    {
        LV_LOG_USER("%s,code:%d\n", __func__, code);
        if (code == LV_EVENT_CANCEL)
            lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, ta); /*To forget the last clicked object to make it focusable again*/
    }
}
LV_FONT_DECLARE(lv_font_source_han_sans_normal_16);

static lv_obj_t *lv_wifi_input_dialog(const char *ssid, const int flags)
{
    lv_obj_t *obj = lv_obj_create(lv_layer_top());
    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(themesPopupWindowColor), 0);

    lv_obj_t *back_bar = lv_obj_create(obj);
    lv_obj_set_size(back_bar, LV_PCT(100), 55);
    lv_obj_set_align(back_bar, LV_ALIGN_TOP_MID);

    lv_obj_t *back_line = lv_obj_create(back_bar);
    lv_obj_set_size(back_line, LV_PCT(100), 1);
    lv_obj_set_align(back_line, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_bg_color(back_line, lv_color_hex(0x3A3A3A), 0);
    lv_obj_set_style_bg_opa(back_line, LV_OPA_100, 0);

    lv_obj_t *label_cancel = lv_label_create(back_bar);
    lv_obj_set_style_text_font(label_cancel, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_cancel, "取消");
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(themesTextColor2), 0);
    lv_obj_align(label_cancel, LV_ALIGN_LEFT_MID, 40, 0);
    lv_obj_add_flag(label_cancel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(label_cancel, wifi_input_event_handler, LV_EVENT_CLICKED, (void *)0);

    lv_obj_t *label_title = lv_label_create(back_bar);
    lv_obj_set_style_text_font(label_title, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_title, ssid);
    lv_obj_set_style_text_color(label_title, lv_color_hex(themesTextColor2), 0);
    lv_obj_align(label_title, LV_ALIGN_CENTER, 0, 0);
    //--------------------------------------------
    lv_obj_t *bottom_bar = lv_obj_create(obj);
    lv_obj_set_size(bottom_bar, LV_PCT(100), 345);
    lv_obj_set_align(bottom_bar, LV_ALIGN_BOTTOM_MID);

    lv_obj_t *pinyin = lv_obj_create(bottom_bar);
    lv_obj_set_size(pinyin, 900, LV_PCT(100));
    lv_obj_set_align(pinyin, LV_ALIGN_BOTTOM_RIGHT);

    lv_obj_t *pinyin_ime = lv_100ask_pinyin_ime_create(pinyin);
    lv_obj_set_style_bg_opa(pinyin_ime, LV_OPA_COVER, 0);
    lv_obj_set_style_text_font(pinyin_ime, &lv_font_source_han_sans_normal_16, 0);
   
    lv_obj_t *kb = lv_100ask_pinyin_ime_get_kb(pinyin_ime);

    //-------------------------------------
    lv_obj_t *ta1 = lv_textarea_create(bottom_bar);
    lv_obj_set_size(ta1, 350, 160);
    lv_obj_set_style_bg_opa(ta1, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(ta1, lv_color_hex(themesWindowBackgroundColor), 0);
    lv_obj_set_style_text_font(ta1, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(ta1, lv_color_hex(0xECF4FC), 0);
    lv_obj_set_style_text_color(ta1, lv_color_hex(0xff0000), LV_PART_SELECTED);
    lv_obj_align(ta1, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_textarea_set_cursor_click_pos(ta1, true);
    lv_textarea_set_text_selection(ta1, true);
    lv_textarea_set_placeholder_text(ta1, "请输入密码");
    lv_obj_set_style_pad_all(ta1, 10, 0);
    lv_obj_set_style_pad_gap(ta1, 10, 0);

    static lv_style_t ta_cursor;
    lv_style_init(&ta_cursor);
    lv_style_set_border_color(&ta_cursor, lv_color_hex(0xECF4FC));
    lv_style_set_border_width(&ta_cursor, 2);
    lv_style_set_pad_left(&ta_cursor, -2);
    lv_style_set_border_side(&ta_cursor, LV_BORDER_SIDE_LEFT);
    lv_style_set_anim_time(&ta_cursor, 500);
    lv_obj_add_style(ta1, &ta_cursor, LV_PART_CURSOR | LV_STATE_FOCUSED);

    lv_obj_t *label1 = lv_textarea_get_label(ta1);
    lv_obj_set_style_text_color(label1, lv_color_hex(0xff0000), LV_PART_SELECTED);
    lv_label_set_text_sel_start(label1, 0);
    lv_label_set_text_sel_end(label1, 2);

    lv_obj_add_event_cb(ta1, ta_event_cb, LV_EVENT_ALL, kb);
    lv_keyboard_set_textarea(kb, ta1);
    return obj;
}

static void wifi_event_handler(lv_event_t *e)
{
    lv_obj_t *current_target = lv_event_get_current_target(e);
    lv_obj_t *target = lv_event_get_target(e);
    const char *user_data = lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d current_target:%p target:%p\n", __func__, e->code, current_target, target);
    lv_wifi_input_dialog(user_data, 1);
}
void lv_wifi_list_clean(void)
{
    lv_obj_clean(wifi_list);
}
lv_obj_t *lv_wifi_list_create(const char *ssid, const int rssi, const int flags)
{
    lv_obj_t *obj = lv_obj_create(wifi_list);
    lv_obj_set_size(obj, LV_PCT(100), 70);

    lv_obj_t *label = lv_label_create(obj);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_color(label, lv_color_hex(themesTextColor), LV_STATE_CHECKED);
    lv_label_set_text(label, ssid);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 30, 30);

    lv_obj_t *divider = lv_divider_create(obj);
    lv_obj_align(divider, LV_ALIGN_BOTTOM_MID, 0, 0);

    int signal = signalLevel(rssi);
    lv_obj_t *img_signal = lv_img_create(obj);
    lv_obj_align(img_signal, LV_ALIGN_TOP_RIGHT, -40, 30);
    switch (signal)
    {
    case 0:
        lv_img_set_src(img_signal, themesImagesPath "wifi/wifi1.png");
    case 1:
        lv_img_set_src(img_signal, themesImagesPath "wifi/wifi2.png");
        break;
    case 2:
        lv_img_set_src(img_signal, themesImagesPath "wifi/wifi3.png");
        break;
    default:
        lv_img_set_src(img_signal, themesImagesPath "wifi/wifi4.png");
        break;
    }
    if (flags > 0)
    {
        lv_obj_t *img_encryp = lv_img_create(obj);
        lv_obj_align(img_encryp, LV_ALIGN_TOP_RIGHT, -40, 30);
        lv_img_set_src(img_encryp, themesImagesPath "wifi/wifi_lock.png");
    }

    lv_obj_add_event_cb(obj, wifi_event_handler, LV_EVENT_CLICKED, ssid);
    return obj;
}
void lv_page_wifi_visible(const int visible)
{
    if (visible)
    {
        scan_count = 0;
        // POSIXTimerSet(wifi_timer, 2, 2);
        // get_toServer("WifiScanR");
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
    lv_obj_clear_flag(content, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_set_size(wifi_list, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_align(wifi_list, LV_ALIGN_TOP_MID, 0, 0);

    lv_wifi_list_create("abc", -66, 2);
    lv_wifi_list_create("QWER", -46, 1);
}
