

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"
/*********************
 *      DEFINES
 *********************/

wifi_info_t g_wifi_info = {0};
char g_wifiPageStatus = 0;

#define POSIXTimer
#ifdef POSIXTimer
static timer_t *wifi_timer;
#else
static lv_timer_t *wifi_timer;
#endif
static timer_t *wifi_connecting_timer;
char scan_count;
static lv_obj_t *wifi_list;
static lv_obj_t *label_connect;
static lv_obj_t *ta1, *sw;
static char scan_min_num = 2, wifi_input_enable = 0;
static char *cur_ssid, *bssid;
static char cur_flags;

static void lv_wifi_list_clean(void);
static lv_obj_t *lv_wifi_list_create(const char *ssid, const int rssi, const int flags);
/**********************
 *  STATIC VARIABLES
 **********************/
static int
wifi_list_item(void *arg)
{
    wifi_node_t *ptr = arg;
    // LV_LOG_USER("%s,ssid:%s,rssi:%d,flags:%d\n", __func__, ptr->ssid, ptr->rssi, ptr->flags);
    lv_obj_t *obj = lv_wifi_list_create(ptr->ssid, ptr->rssi, ptr->flags);
    if (bssid == NULL)
        LV_LOG_USER("%s,bssid,null\n", __func__);
    else
    {
        if (wifi_connecting > 0 && strcmp(ptr->ssid, cur_ssid) == 0)
        {
            lv_obj_t *img_indicator = lv_img_create(obj);
            lv_obj_align(img_indicator, LV_ALIGN_TOP_LEFT, 0, 30);
            lv_img_set_src(img_indicator, themesImagesPath "icon_loading_small.png");
            lv_obj_move_to_index(obj, 0);
            lv_obj_add_state(lv_obj_get_child(obj, 0), LV_STATE_CHECKED);
        }
        else if (strcmp(bssid, ptr->bssid) == 0 && wifi_connecting == 0)
        {
            lv_obj_t *img_indicator = lv_img_create(obj);
            lv_obj_align(img_indicator, LV_ALIGN_TOP_LEFT, 0, 30);
            lv_img_set_src(img_indicator, themesImagesPath "wifi/icon_selected.png");
            lv_obj_move_to_index(obj, 0);
            lv_obj_add_state(lv_obj_get_child(obj, 0), LV_STATE_CHECKED);
        }
    }
    return 0;
}
// static void *wifi_update_task(void *arg)
// {
//     pthread_mutex_lock(&g_mutex);
//     bssid = get_attr_value_string("bssid");
//     lv_wifi_list_clean();
//     wifi_list_each(wifi_list_item);
//     pthread_mutex_unlock(&g_mutex);
// }
static void wifi_update()
{
    // pthread_t tid;
    // pthread_create(&tid, NULL, wifi_update_task, NULL);
    // pthread_detach(tid);
    bssid = get_attr_value_string("bssid");
    lv_wifi_list_clean();
    wifi_list_each(wifi_list_item);
    if (lv_obj_get_child_cnt(wifi_list) <= 1)
    {
        lv_obj_t *obj = lv_obj_create(wifi_list);
        lv_obj_set_size(obj, LV_PCT(100), 70);
        lv_obj_t *img_indicator = lv_img_create(obj);
        lv_obj_align(img_indicator, LV_ALIGN_TOP_LEFT, 0, 30);
        lv_img_set_src(img_indicator, themesImagesPath "icon_loading_small.png");
    }
}
static void wifi_scan_timer_reset()
{
    LV_LOG_USER("%s...\n", __func__);
    get_toServer("WifiCurConnected");
    set_num_toServer("WifiScan", -1);
    scan_count = 0;
    wifi_input_enable = 0;
    POSIXTimerSet(wifi_timer, 3, 3);
}
void wifi_connecting_change(char state)
{
    if (g_wifiPageStatus == 0)
        return;
    if (state > 0)
    {
        POSIXTimerSet(wifi_connecting_timer, 0, 60);
    }
    else
    {
        if (POSIXTimerGet(wifi_connecting_timer) > 0)
            POSIXTimerSet(wifi_connecting_timer, 0, 0);
        else
        {
        }
        wifi_scan_timer_reset();
    }
}
static void wifi_fail_dialog_event_cb(lv_event_t *e)
{
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
        break;
    }
    clean_manual_layer();
}
static void lv_wifi_fail_dialog(const char *info)
{
    clean_manual_layer();
    lv_obj_t *layer = get_manual_layer();
    lv_dialog1(layer, info, NULL, "好的", themesImagesPath "icon_wifi_warn.png", NULL, wifi_fail_dialog_event_cb);
}
void lv_wifi_property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("%s,key:%s\n", __func__, key);
    if (strcmp("WifiScanR", key) == 0)
    {
        if (wifi_connecting == 0 && wifi_input_enable == 0)
        {
            wifi_update();
        }
    }
    else if (strcmp("WifiState", key) == 0)
    {
        if (g_wifi_state == RK_WIFI_State_CONNECTFAILED || g_wifi_state == RK_WIFI_State_DISCONNECTED)
        {
            lv_wifi_fail_dialog("网络连接失败，请重试");
        }
        else if (g_wifi_state == RK_WIFI_State_CONNECTED)
        {
            systemRun("(wpa_cli list_networks | tail -n +3 | grep -v 'CURRENT' | awk '{system(\"wpa_cli disable_network \" $1)}') &");
        }
        else if (g_wifi_state == RK_WIFI_State_LINK_CONNECTED)
        {
            lv_about_bind_dialog("连接成功", "下载火粉APP   绑定设备\n海量智慧菜谱  一键烹饪");
        }
    }
    else if (strcmp("ssid", key) == 0)
    {
        if (g_wifi_state >= RK_WIFI_State_CONNECTED && strlen(g_wifi_info.ssid) > 0)
        {
            char *data = get_value_string(value);
            if (strcmp(g_wifi_info.ssid, data) == 0)
            {
                lv_obj_t *layer = get_manual_layer();
                if ((long)layer->user_data == 5)
                {
                    clean_manual_layer();
                }
            }
        }
        if (strlen(g_wifi_info.ssid) > 0)
            memset(g_wifi_info.ssid, 0, sizeof(g_wifi_info.ssid));
    }
}
static void POSIXTimer_connecting_cb(union sigval val)
{
    wifi_connecting = 0;
    wifi_connecting_change(wifi_connecting);
}
#ifdef POSIXTimer
static void POSIXTimer_cb(union sigval val)
#else
static void lv_timer_cb(lv_timer_t *timer)
#endif
{
    LV_LOG_USER("%s,", __func__);

    ++scan_count;
    if (scan_count <= scan_min_num)
    {
        if (wifi_connecting == 0 && wifi_input_enable == 0)
            get_toServer("WifiScanR");
        if (scan_count == 1)
        {
            if (wifi_connecting == 0 && wifi_input_enable == 0)
            {
                get_toServer("WifiCurConnected");
                set_num_toServer("WifiScan", -1);
            }
        }
        else if (scan_count == scan_min_num)
        {
#ifdef POSIXTimer
            POSIXTimerSet(wifi_timer, 9, 9);
#else
            lv_timer_set_period(wifi_timer, 10000);
#endif
        }
    }
    else
    {
        if (wifi_connecting == 0 && wifi_input_enable == 0)
        {
            get_toServer("WifiCurConnected");
            if (scan_count % 2 == 0)
                set_num_toServer("WifiScan", -1);
            else
                get_toServer("WifiScanR");
        }
    }
}
void switch_value_state(lv_obj_t *sw_obj, const char state)
{
    lv_obj_t *label_sw = lv_obj_get_child(sw_obj, 0);
    if (state)
    {
        lv_obj_set_style_text_color(label_sw, lv_color_hex(0xffffff), 0);
        lv_label_set_text(label_sw, "开");
        lv_obj_align(label_sw, LV_ALIGN_CENTER, -20, 0);
        lv_obj_add_state(sw_obj, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_set_style_text_color(label_sw, lv_color_hex(themesTextColor2), 0);
        lv_label_set_text(label_sw, "关");
        lv_obj_align(label_sw, LV_ALIGN_CENTER, 20, 0);
        lv_obj_clear_state(sw_obj, LV_STATE_CHECKED);
    }
}
static void switch_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        // LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        // lv_obj_t *label_sw = lv_obj_get_child(obj, 0);
        if (lv_obj_has_state(obj, LV_STATE_CHECKED))
        {
            g_save_settings.wifiEnable = 1;
        }
        else
        {
            g_save_settings.wifiEnable = 0;
        }
        switch_value_state(sw, g_save_settings.wifiEnable);
        LV_LOG_USER("H_Kv_Set:%d", H_Kv_Set("wifiEnable", &g_save_settings.wifiEnable, 1, 0));
    }
}

static void wifi_input_event_handler(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        wifi_input_enable = 0;
        clean_manual_layer();
        break;
    case 1:
    {
        const char *ta_text = lv_textarea_get_text(ta1);
        int ta_text_len = strlen(ta_text);
        if (ta_text_len >= 8)
        {
            lv_label_set_text(target, "连接中");
            // lv_obj_set_style_text_color(target, lv_color_hex(themesTextColor2), 0);
            connectWiFi(cur_ssid, ta_text, cur_flags);
            wifi_update();
        }
    }
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
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        int ta_text_len = strlen(lv_textarea_get_text(ta));
        LV_LOG_USER("%s,code:%d text len:%d\n", __func__, code, ta_text_len);
        if (ta_text_len >= 8)
        {
            lv_obj_set_style_text_color(label_connect, lv_color_hex(themesTextColor), 0);
        }
        else
        {
            lv_obj_set_style_text_color(label_connect, lv_color_hex(themesTextColor2), 0);
        }
    }
}
LV_FONT_DECLARE(lv_font_source_han_sans_normal_16);
static lv_obj_t *lv_wifi_input_dialog5(char *ssid, const int flags)
{
    cur_ssid = ssid;
    cur_flags = flags;
    wifi_input_enable = 1;
    lv_obj_t *layer = get_manual_layer();
    layer->user_data = (void *)5;
    lv_obj_t *obj = lv_obj_create(layer);
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

    label_connect = lv_label_create(back_bar);
    lv_obj_set_style_text_font(label_connect, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_label_set_text(label_connect, "连接");
    lv_obj_set_style_text_color(label_connect, lv_color_hex(themesTextColor2), 0);
    lv_obj_align(label_connect, LV_ALIGN_RIGHT_MID, -40, 0);
    lv_obj_add_flag(label_connect, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(label_connect, wifi_input_event_handler, LV_EVENT_CLICKED, (void *)1);

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
    ta1 = lv_textarea_create(bottom_bar);
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
    char *user_data = lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d current_target:%p target:%p\n", __func__, e->code, current_target, target);
    lv_wifi_input_dialog5(user_data, 1);
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
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 50, 30);

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

    lv_obj_add_event_cb(obj, wifi_event_handler, LV_EVENT_CLICKED, (void *)ssid);
    return obj;
}
void lv_page_wifi_visible(const int visible)
{
    LV_LOG_USER("%s...visible:%d", __func__, visible);
    g_wifiPageStatus = visible;
    lv_obj_scroll_to_view(lv_obj_get_child(wifi_list, 0), LV_ANIM_OFF);
    if (visible)
    {
        // scan_count = 0;
        wifi_input_enable = 0;
        // set_num_toServer("WifiScan", -1);

        switch_value_state(sw, g_save_settings.wifiEnable);
        if (wifi_connecting == 0)
        {
            systemRun("(wpa_cli list_networks | tail -n +3 | grep -v 'CURRENT' | awk '{system(\"wpa_cli disable_network \" $1)}') &");
            get_toServer("WifiScanR");
        }
#ifdef POSIXTimer
        if (scan_count == 0)
            POSIXTimerSet(wifi_timer, 3, 3);
        else
            POSIXTimerSet(wifi_timer, 9, 9);
#else
        lv_timer_set_period(wifi_timer, 3000);
        lv_timer_resume(wifi_timer);
#endif
    }
    else
    {
#ifdef POSIXTimer
        POSIXTimerSet(wifi_timer, 0, 0);
#else
        lv_timer_pause(wifi_timer);
#endif
        // lv_obj_scroll_to_x(wifi_list, 10, LV_ANIM_OFF);
        // lv_obj_scroll_to_y(wifi_list, 10, LV_ANIM_OFF);
        // lv_obj_scroll_by(wifi_list, 0, 50, LV_ANIM_OFF);
        if (wifi_connecting == 0)
        {
            systemRun("(wpa_cli enable_network all) &");
        }
    }
}
void lv_page_wifi_create(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    if (wifi_timer == NULL)
    {
#ifdef POSIXTimer
        wifi_timer = POSIXTimerCreate(0, POSIXTimer_cb);
#else
        wifi_timer = lv_timer_create_basic();
        lv_timer_set_cb(wifi_timer, lv_timer_cb);
        lv_timer_pause(wifi_timer);
#endif
    }
    if (wifi_connecting_timer == NULL)
        wifi_connecting_timer = POSIXTimerCreate(0, POSIXTimer_connecting_cb);

    lv_obj_t *head = lv_obj_create(page);
    lv_obj_set_size(head, LV_PCT(100), 80);
    lv_obj_align(head, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_t *content = lv_list_create(page);
    lv_obj_set_size(content, LV_PCT(100), 260);
    lv_obj_align(content, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *divider = lv_divider_create(head);
    lv_obj_align(divider, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_t *label = lv_label_create(head);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, "WIFI");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 30, 30);

    sw = lv_switch_create(head);
    lv_obj_align(sw, LV_ALIGN_TOP_RIGHT, -114, 25);
    lv_obj_set_size(sw, 100, 46);

    lv_obj_t *label_sw = lv_label_create(sw);
    lv_obj_set_style_text_font(label_sw, &lv_font_SiYuanHeiTi_Normal_26, 0);
    lv_obj_set_style_text_color(label_sw, lv_color_hex(themesTextColor2), 0);
    lv_label_set_text(label_sw, "关");
    lv_obj_align(label_sw, LV_ALIGN_CENTER, 20, 0);

    lv_obj_add_style(sw, &switch_style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(sw, &switch_style_indicator_check, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_add_style(sw, &switch_style_knob, LV_PART_KNOB);

    lv_obj_add_event_cb(sw, switch_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    wifi_list = lv_list_create(content);
    lv_obj_clear_flag(content, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_set_size(wifi_list, LV_PCT(100), LV_PCT(100));
    lv_obj_align(wifi_list, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_set_scroll_dir(wifi_list, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(wifi_list, LV_SCROLL_SNAP_START);
    // lv_wifi_list_create("abc", -66, 2);
    // lv_wifi_list_create("QWER", -46, 1);
    wifi_update();
}
