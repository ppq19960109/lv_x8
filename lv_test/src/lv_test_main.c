

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"
#include "wifi_list.h"
/*********************
 *      DEFINES
 *********************/
int g_wifi_state = 0;
lv_style_t roller_style_unselected, roller_style_selected;
lv_style_t slider_style_main, slider_style_indicator, slider_style_knob;
lv_style_t switch_style_indicator, switch_style_indicator_check, switch_style_knob;
static lv_obj_t *icon_wifi, *icon_standby, *clock_text;
static timer_t clock_timer;
static lv_obj_t *home, *home_bar;
char LStOvState = 0, RStOvState = 0;
char LStoveStatus = 0, RStoveStatus = 0;
static lv_obj_t *gif;
static lv_timer_t *sleep_timer;
/**********************
 *  STATIC VARIABLES
 **********************/
void (*page_property_change_cb)(const char *key, void *value);
void register_page_property_change_cb(void (*cb)(const char *key, void *value))
{
    page_property_change_cb = cb;
}
void production_mode(const char state)
{
    if (state)
    {
        lv_obj_add_flag(home_bar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_width(home, 1280);
    }
    else
    {
        lv_obj_clear_flag(home_bar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_width(home, 1160);
    }
}
#if LV_100ASK_PAGE_MANAGER_COSTOM_ANIMARION
/*open page anim*/
static void open_page_anim(lv_obj_t *obj)
{
    /*Do something with LVGL*/
    lv_100ask_page_manager_page_t *page = (lv_100ask_page_manager_page_t *)obj;
    LV_LOG_USER("open page anim. name:%s", page->name);

    if (page->page_update_cb != NULL)
        page->page_update_cb(page);

    if (page->page_property_change_cb != NULL)
        page_property_change_cb = page->page_property_change_cb;
    else
        LV_LOG_USER("page name:%s,page_property_change_cb is null", page->name);

    lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);

    if (lv_page_get_page_depth() > 1)
    {
        lv_img_set_src(icon_standby, themesImagesPath "icon_home.png");
    }
    else
    {
        lv_img_set_src(icon_standby, themesImagesPath "icon_sleep.png");
    }
    // lv_obj_t *layer=lv_scr_act();
    // LV_LOG_USER("lv_scr_act child_cnt:%d user_data:%d",lv_obj_get_child_cnt(layer),layer->user_data);
}

/*close page anim*/
static void close_page_anim(lv_obj_t *obj)
{
    /*Do something with LVGL*/
    lv_100ask_page_manager_page_t *page = (lv_100ask_page_manager_page_t *)obj;
    LV_LOG_USER("close page anim. name:%s page_close_cb:%p", page->name, page->page_close_cb);
    if (page->page_close_cb != NULL)
        page->page_close_cb(page);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
}
#endif
//-------------------------------------------------
static void getCurrentTime()
{
    time_t t;
    time(&t);
    struct tm *local_tm = localtime(&t);
    LV_LOG_USER("year:%d mon:%d day:%d", local_tm->tm_year, local_tm->tm_mon, local_tm->tm_mday);
    LV_LOG_USER("hour:%d min:%d sec:%d", local_tm->tm_hour, local_tm->tm_min, local_tm->tm_sec);

    char buf[8];
    sprintf(buf, "%02d:%02d", local_tm->tm_hour, local_tm->tm_min);
    lv_label_set_text(clock_text, buf);
}
static void signalHandler(int signal)
{
    printf("signalHandler signal is %d\n", signal);
    if (signal == SIGRTMIN)
    {
        getCurrentTime();
    }
}
static void POSIXTimer_cb(union sigval val)
{
    LV_LOG_USER("%s sival_int:%d", __func__, val.sival_int);
    if (val.sival_int == 0)
    {
        // raise(SIGRTMIN);
        pthread_mutex_lock(&g_mutex);
        if (gif != NULL)
        {
            lv_obj_del(gif);
            gif = NULL;
        }
        getCurrentTime();
        pthread_mutex_unlock(&g_mutex);
    }
}
static void steamInterfaceChange(int state)
{
    if (state == 0)
    {
        if (lv_page_exist_page("page_steaming") > 0)
        {
            if (lv_page_exist_page("page_steamoven"))
                lv_page_back_page("page_steamoven");
            else
                lv_page_back_top_page();
        }
    }
    else
    {
        if (lv_page_exist_page("page_steaming") == 0)
        {
            if (lv_page_current_exist_page("page_steam_left") || lv_page_current_exist_page("page_multistage") || lv_page_current_exist_page("page_steam_right") || lv_page_current_exist_page("page_steam_assist"))
                lv_100ask_page_manager_set_open_page(NULL, "page_steaming");
            else if (lv_page_current_exist_page("page_smartrecipes") || lv_page_current_exist_page("page_cook_details"))
            {
                lv_100ask_page_manager_set_open_page(NULL, "page_steaming");
            }
        }
    }
}

static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("key:%s value:%p\n", key, value);

    if (strcmp("LStOvState", key) == 0)
    {
        LStOvState = get_value_int(value);
        RStOvState = get_attr_value_int("RStOvState");
        if (LStOvState == WORK_STATE_STOP)
        {
            if (RStOvState == WORK_STATE_STOP)
            {
                steamInterfaceChange(0);
            }
        }
        else
        {
            steamInterfaceChange(1);
        }
    }
    else if (strcmp("RStOvState", key) == 0)
    {
        LStOvState = get_attr_value_int("LStOvState");
        RStOvState = get_value_int(value);
        if (RStOvState == WORK_STATE_STOP)
        {
            if (LStOvState == WORK_STATE_STOP)
            {
                steamInterfaceChange(0);
            }
        }
        else
        {
            steamInterfaceChange(1);
        }
    }
    else if (strcmp("LStoveStatus", key) == 0)
    {
        LStoveStatus = get_value_int(value);
    }
    else if (strcmp("RStoveStatus", key) == 0)
    {
        RStoveStatus = get_value_int(value);
    }
    else if (strcmp("WifiScanR", key) == 0)
    {
        cJSON *root = cJSON_Parse(get_value_string(value));
        char *json = cJSON_PrintUnformatted(root);
        LV_LOG_USER("%s,json:%s\n", __func__, json);
        free(json);
        wifi_list_clear();

        int arraySize = cJSON_GetArraySize(root);
        cJSON *arraySub, *ssid, *rssi, *bssid, *flags;
        for (int i = 0; i < arraySize; i++)
        {
            arraySub = cJSON_GetArrayItem(root, i);
            if (arraySub == NULL)
                continue;
            ssid = cJSON_GetObjectItem(arraySub, "ssid");
            if (strcmp(ssid->valuestring, "") == 0)
                continue;
            rssi = cJSON_GetObjectItem(arraySub, "rssi");
            bssid = cJSON_GetObjectItem(arraySub, "bssid");
            flags = cJSON_GetObjectItem(arraySub, "flags");
            wifi_node_t *cur = malloc(sizeof(wifi_node_t));
            memset(cur, 0, sizeof(wifi_node_t));
            strncpy(cur->ssid, ssid->valuestring, sizeof(cur->ssid));
            strncpy(cur->bssid, bssid->valuestring, sizeof(cur->bssid));
            cur->flags = encrypType(flags->valuestring);
            cur->rssi = rssi->valueint;

            wifi_list_add(cur);
        }
        // wifi_list_each(NULL);
        cJSON_Delete(root);
    }
    else if (strcmp("WifiState", key) == 0)
    {
        g_wifi_state = get_value_int(value);
        if (g_wifi_state == RK_WIFI_State_CONNECTING)
        {
        }
        else
        {
            wifi_connecting = 0;
            wifi_connecting_change(wifi_connecting);
            if (g_wifi_state == RK_WIFI_State_CONNECTED)
            {
                lv_img_set_src(icon_wifi, themesImagesPath "icon_wifi_half_connect.png");
            }
            else if (g_wifi_state == RK_WIFI_State_LINK_CONNECTED)
            {
                lv_img_set_src(icon_wifi, themesImagesPath "icon_wifi_connected.png");
            }
            else
            {
                lv_img_set_src(icon_wifi, themesImagesPath "icon_wifi_disconnect.png");
            }
            if (g_wifiPageStatus == 0)
            {
                get_toServer("WifiCurConnected");
            }
        }
    }
    else if (strcmp("ssid", key) == 0)
    {
        if (g_wifi_state >= RK_WIFI_State_CONNECTED && strlen(g_wifi_info.ssid) > 0)
        {
            char *data = get_value_string(value);
            if (strcmp(g_wifi_info.ssid, data) == 0)
            {
                char buf[200];
                sprintf(buf, "(wpa_cli list_networks | tail -n +3 | grep \'%s\' | grep -v 'CURRENT' | awk '{system(\"wpa_cli remove_network \" $1)}' && wpa_cli save_config) &", data);
                LV_LOG_USER("ssid:%s buf:%s\n", data, buf);
                systemRun(buf);
            }
        }
        if (g_wifiPageStatus == 0)
        {
            if (strlen(g_wifi_info.ssid) > 0)
                memset(g_wifi_info.ssid, 0, sizeof(g_wifi_info.ssid));
        }
    }
    else if (strcmp("NtpTimestamp", key) == 0)
    {
        long t = get_value_int(value);
        setClockTimestamp(t);
        getCurrentTime();
    }
    else if (strcmp("HoodOffLeftTime", key) == 0)
    {
        int data = get_value_int(value);
        if (data)
        {
            char buf[120], buf1[36];
            sprintf(buf, "烟机自动延时#E68855 %d分钟#\n后关闭,清除余烟", data);
            sprintf(buf1, "立即关闭#E68855 %d分钟#", data);
            lv_auto_dialog3_update(buf, buf1);
        }
        else
        {
            lv_auto_dialog3_close();
        }
    }
    else if (strcmp("HoodOffRemind", key) == 0)
    {
        int data = get_value_int(value);
        switch (data)
        {
        case 1:
            lv_auto_dialog3();
            break;
        case 2:
            // if (LStOvState == WORK_STATE_PREHEAT || LStOvState == WORK_STATE_RUN || LStOvState == WORK_STATE_PAUSE || RStOvState == WORK_STATE_PREHEAT || RStOvState == WORK_STATE_RUN || RStOvState == WORK_STATE_PAUSE)
            lv_auto_dialog1("蒸烤箱工作中,需散热,烟机无法关闭", NULL, "好的", NULL);
            break;
        case 3:
            // if (LStOvState == WORK_STATE_PREHEAT || LStOvState == WORK_STATE_RUN || LStOvState == WORK_STATE_PAUSE)
            lv_auto_dialog1("烤箱工作中,需散热,烟机不得低于2档", NULL, "好的", NULL);
            break;
        case 4:
            if (LStoveStatus > 0 || RStoveStatus > 0)
                lv_auto_dialog1("灶具工作中，烟机无法关闭", NULL, "好的", NULL);
            break;
        case 5:
            break;
        }
    }
    else if (strcmp("OTAState", key) == 0)
    {
        int data = get_value_int(value);
        if (data == OTA_STATE_NEW_FIRMWARE)
        {
            if (g_versionCheckState > 0)
            {
                char *ver = get_attr_value_string("OTANewVersion");
                lv_auto_upgrade_confirm_dialog6(ver);
            }
        }
        else if (data == OTA_STATE_DOWNLOAD_START)
        {
            lv_auto_upgrade_dialog5("通讯板");
        }
        else if (data == OTA_STATE_DOWNLOAD_FAIL)
        {
            // loaderUpdateResultShow("通讯板升级失败");
        }
        else if (data == OTA_STATE_INSTALL_SUCCESS)
        {
        }

        if (data == OTA_STATE_NO_FIRMWARE)
        {
            if (g_versionCheckState > 0)
            {
                --g_versionCheckState;
                if (g_versionCheckState == 0)
                {
                    lv_update_state(2);
                }
            }
        }
        else
        {
            g_versionCheckState = 0;
        }
    }
    else if (strcmp("OTAPowerState", key) == 0)
    {
        int data = get_value_int(value);
        if (data == OTA_STATE_NEW_FIRMWARE)
        {
            if (g_versionCheckState > 0)
            {
                char *ver = get_attr_value_string("OTAPowerNewVersion");
                lv_auto_upgrade_confirm_dialog6(ver);
            }
        }
        else if (data == OTA_STATE_DOWNLOAD_START)
        {
            lv_auto_upgrade_dialog5("电源板");
        }
        else if (data == OTA_STATE_DOWNLOAD_FAIL)
        {
            // loaderUpdateResultShow("通讯板升级失败");
        }
        else if (data == OTA_STATE_INSTALL_SUCCESS)
        {
        }

        if (data == OTA_STATE_NO_FIRMWARE)
        {
            if (g_versionCheckState > 0)
            {
                --g_versionCheckState;
                if (g_versionCheckState == 0)
                {
                    lv_update_state(2);
                }
            }
        }
        else
        {
            g_versionCheckState = 0;
        }
    }
    else if (strcmp("OTAProgress", key) == 0)
    {
        int data = get_value_int(value);
        lv_auto_upgrade_dialog5_progress(data, "通讯板");
    }
    else if (strcmp("OTAPowerProgress", key) == 0)
    {
        int data = get_value_int(value);
        lv_auto_upgrade_dialog5_progress(data, "电源板");
    }
    if (page_property_change_cb != NULL)
        page_property_change_cb(key, value);
}

static void init_main_page(lv_obj_t *page)
{
    lv_obj_t *cont_row = lv_obj_create(page);

    lv_obj_set_size(cont_row, 292 * 3 + 100, 360);
    lv_obj_align(cont_row, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *home_hood = lv_img_create(cont_row);
    lv_img_set_src(home_hood, themesImagesPath "home_hood_background.png");
    lv_obj_t *label_title1 = lv_label_create(home_hood);
    lv_obj_set_style_text_font(label_title1, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_label_set_text(label_title1, "烟机灶具");
    lv_obj_set_style_text_color(label_title1, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_title1, LV_ALIGN_TOP_MID, 0, 45);
    lv_100ask_page_manager_set_load_page_event(home_hood, NULL, "page_hood");

    lv_obj_t *home_steamoven = lv_img_create(cont_row);
    lv_img_set_src(home_steamoven, themesImagesPath "home_steamoven_background.png");
    lv_obj_t *label_title2 = lv_label_create(home_steamoven);
    lv_obj_set_style_text_font(label_title2, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_label_set_text(label_title2, "蒸烤箱");
    lv_obj_set_style_text_color(label_title2, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_title2, LV_ALIGN_TOP_MID, 0, 45);
    lv_100ask_page_manager_set_load_page_event(home_steamoven, NULL, "page_steamoven");

    lv_obj_t *home_smartrecipes = lv_img_create(cont_row);
    lv_img_set_src(home_smartrecipes, themesImagesPath "home_smartrecipes_background.png");
    lv_obj_t *label_title3 = lv_label_create(home_smartrecipes);
    lv_obj_set_style_text_font(label_title3, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_label_set_text(label_title3, "智慧菜谱");
    lv_obj_set_style_text_color(label_title3, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_title3, LV_ALIGN_TOP_MID, 0, 45);
    lv_100ask_page_manager_set_load_page_event(home_smartrecipes, NULL, "page_smartrecipes");

    register_property_change_cb(property_change_cb);

    // gif = lv_gif_create(lv_scr_act());
    // lv_gif_set_src(gif, "S:/oem/boot.gif");
    // lv_gif_restart(gif);
}
//-------------------------------------------------
static void init_style()
{
    lv_style_init(&roller_style_unselected);
    lv_style_set_text_font(&roller_style_unselected, &lv_font_SiYuanHeiTi_Normal_30);
    lv_style_set_text_color(&roller_style_unselected, lv_color_hex(0xffffff));
    lv_style_set_text_align(&roller_style_unselected, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_line_space(&roller_style_unselected, 18);

    lv_style_init(&roller_style_selected);
    lv_style_set_text_font(&roller_style_selected, &lv_font_SiYuanHeiTi_Normal_34);
    lv_style_set_text_color(&roller_style_selected, lv_color_hex(themesTextColor));
    lv_style_set_text_align(&roller_style_selected, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_line_space(&roller_style_selected, 18);

    lv_style_init(&slider_style_main);
    lv_style_set_bg_opa(&slider_style_main, LV_OPA_COVER);
    lv_style_set_bg_color(&slider_style_main, lv_color_hex(themesTextColor2));
    lv_style_set_radius(&slider_style_main, LV_RADIUS_CIRCLE);

    lv_style_init(&slider_style_indicator);
    lv_style_set_bg_opa(&slider_style_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&slider_style_indicator, lv_color_hex(themesTextColor));
    lv_style_set_radius(&slider_style_indicator, LV_RADIUS_CIRCLE);

    lv_style_init(&slider_style_knob);
    lv_style_set_pad_all(&slider_style_knob, 30);
    lv_style_set_bg_opa(&slider_style_knob, LV_OPA_0);
    lv_style_set_bg_img_src(&slider_style_knob, themesImagesPath "icon_slide.png");

    lv_style_init(&switch_style_indicator);
    lv_style_set_bg_opa(&switch_style_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&switch_style_indicator, lv_color_hex(0x333333));
    lv_style_set_radius(&switch_style_indicator, LV_RADIUS_CIRCLE);

    lv_style_init(&switch_style_indicator_check);
    lv_style_set_bg_color(&switch_style_indicator_check, lv_color_hex(themesTextColor));

    lv_style_init(&switch_style_knob);
    lv_style_set_bg_opa(&switch_style_knob, LV_OPA_COVER);
    lv_style_set_bg_color(&switch_style_knob, lv_color_hex(0xfffff));
    lv_style_set_radius(&switch_style_knob, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&switch_style_knob, -3);
}
static void home_bar_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%ld\n", __func__, e->code, user_data);
    switch (user_data)
    {
    case 0:
        lv_100ask_page_manager_set_open_page(NULL, "page_set");
        lv_page_set_tabview_set(1);
        break;
    case 1:
        lv_100ask_page_manager_set_open_page(NULL, "page_set");
        break;
    case 2:
    {
    }
    break;
    case 3:
    {
        if (lv_page_get_page_depth() > 1)
        {
            lv_page_back_top_page();
        }
        else
        {
            lv_auto_screen_dialog4(0);
        }
    }
    break;
    }
}
static void lv_sleep_timer_cb(lv_timer_t *timer)
{
    uint32_t tick = lv_tick_get();
    LV_LOG_USER("%s,sleep...tick:%u", __func__, tick);

    lv_timer_pause(sleep_timer);
    lv_auto_screen_dialog4(0);
}
void lv_sleep_wakeup(void)
{
    // LV_LOG_USER("%s,wakeup...", __func__);
    lv_timer_reset(sleep_timer);
    lv_timer_resume(sleep_timer);
    lv_auto_screen_dialog4_close();
}
int http_weather_get_cb(int code, const char *body)
{
    LV_LOG_USER("code:%d body:%s", code, body);
    return 0;
}
lv_obj_t *manual_scr = NULL, *main_scr = NULL;
void lv_test_widgets(void)
{
    // manual_scr = lv_obj_create(NULL);
    main_scr = lv_scr_act();
    LV_LOG_USER("lv_test_widgets...manual_scr:%p main_scr:%p", manual_scr, main_scr);
    // lv_100ask_pinyin_ime_simple_test();
    // lv_100ask_page_manager_simple_test();
    // lv_100ask_demo_layer();
    // return 0;
    init_style();

    http_async_client_get("http://mcook.marssenger.com/application/weather/day", http_weather_get_cb);
    clock_timer = POSIXTimerCreate(0, POSIXTimer_cb);
    POSIXTimerSet(clock_timer, 60, 10);
    sleep_timer = lv_timer_create(lv_sleep_timer_cb, 60000, NULL);

    signal(SIGRTMIN, signalHandler);

    lv_obj_t *win_bg = lv_img_create(lv_scr_act());
    lv_img_set_src(win_bg, themesImagesPath "window-background.png");

    home_bar = lv_obj_create(lv_scr_act());
    lv_obj_set_size(home_bar, 120, LV_PCT(100));
    lv_obj_set_align(home_bar, LV_ALIGN_RIGHT_MID);

    lv_obj_t *icon_newline2 = lv_img_create(home_bar);
    lv_img_set_src(icon_newline2, themesImagesPath "icon_newline2.png");
    lv_obj_set_size(icon_newline2, 2, LV_SIZE_CONTENT);
    lv_obj_set_align(icon_newline2, LV_ALIGN_LEFT_MID);

    clock_text = lv_label_create(home_bar);
    lv_obj_align(clock_text, LV_ALIGN_TOP_MID, 0, 17);
    lv_obj_set_style_text_color(clock_text, lv_color_hex(themesTextColor2), 0);
    lv_label_set_text(clock_text, "12:45");

    lv_obj_t *wifi_obj = lv_obj_create(home_bar);
    lv_obj_set_size(wifi_obj, LV_PCT(100), 86);
    lv_obj_align(wifi_obj, LV_ALIGN_TOP_MID, 0, 54);
    lv_obj_t *icon_newline1 = lv_img_create(wifi_obj);
    lv_img_set_src(icon_newline1, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline1, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline1, LV_ALIGN_TOP_MID, 0, 0);
    icon_wifi = lv_img_create(wifi_obj);
    lv_img_set_src(icon_wifi, themesImagesPath "icon_wifi_disconnect.png");
    lv_obj_align(icon_wifi, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(wifi_obj, home_bar_event_cb, LV_EVENT_CLICKED, 0);

    lv_obj_t *set_obj = lv_obj_create(home_bar);
    lv_obj_set_size(set_obj, LV_PCT(100), 86);
    lv_obj_align(set_obj, LV_ALIGN_TOP_MID, 0, 133);
    icon_newline1 = lv_img_create(set_obj);
    lv_img_set_src(icon_newline1, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline1, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline1, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_t *icon_set = lv_img_create(set_obj);
    lv_img_set_src(icon_set, themesImagesPath "icon_set.png");
    lv_obj_align(icon_set, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(set_obj, home_bar_event_cb, LV_EVENT_CLICKED, (void *)1);

    lv_obj_t *alarm_obj = lv_obj_create(home_bar);
    lv_obj_set_size(alarm_obj, LV_PCT(100), 86);
    lv_obj_align(alarm_obj, LV_ALIGN_TOP_MID, 0, 223);
    icon_newline1 = lv_img_create(alarm_obj);
    lv_img_set_src(icon_newline1, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline1, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline1, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_t *icon_alarm = lv_img_create(alarm_obj);
    lv_img_set_src(icon_alarm, themesImagesPath "icon_alarm.png");
    lv_obj_align(icon_alarm, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_t *icon_alarm_time = lv_label_create(alarm_obj);
    // lv_obj_set_style_text_font(icon_alarm_time, &lv_font_montserrat_24, 0);
    // lv_obj_set_style_text_color(icon_alarm_time, lv_color_hex(themesTextColor), 0);
    // lv_label_set_text(icon_alarm_time, "01:02:03");
    // lv_obj_align(icon_alarm_time, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(alarm_obj, home_bar_event_cb, LV_EVENT_CLICKED, (void *)2);

    lv_obj_t *standby_obj = lv_obj_create(home_bar);
    lv_obj_set_size(standby_obj, LV_PCT(100), 86);
    lv_obj_align(standby_obj, LV_ALIGN_TOP_MID, 0, 314);
    icon_newline1 = lv_img_create(standby_obj);
    lv_img_set_src(icon_newline1, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline1, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline1, LV_ALIGN_TOP_MID, 0, 0);
    icon_standby = lv_img_create(standby_obj);
    lv_img_set_src(icon_standby, themesImagesPath "icon_standby.png");
    lv_obj_align(icon_standby, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(standby_obj, home_bar_event_cb, LV_EVENT_CLICKED, (void *)3);

    //****************************************************
    home = lv_obj_create(lv_scr_act());
    // lv_obj_refresh_style(home, LV_PART_ANY, LV_STYLE_PROP_ANY);
    // lv_obj_invalidate(home);
    // lv_obj_update_layout(home);
    // static lv_style_t style_clear;
    // lv_style_init(&style_clear);
    // lv_style_set_bg_opa(&style_clear, LV_OPA_0);
    // lv_style_set_border_opa(&style_clear, LV_OPA_0);
    // lv_obj_add_style(home, &style_clear, 0);

    lv_obj_set_size(home, 1160, LV_PCT(100));
    lv_obj_set_align(home, LV_ALIGN_LEFT_MID);

    //****************************************************
    lv_obj_t *page_manager = lv_100ask_page_manager_create(home);

    lv_obj_t *main_page = lv_100ask_page_manager_page_create(page_manager, "main_page");
    lv_obj_t *page_hood = lv_100ask_page_manager_page_create(page_manager, "page_hood");
    lv_obj_t *page_steamoven = lv_100ask_page_manager_page_create(page_manager, "page_steamoven");
    lv_obj_t *page_smartrecipes = lv_100ask_page_manager_page_create(page_manager, "page_smartrecipes");
    lv_obj_t *page_steaming = lv_100ask_page_manager_page_create(page_manager, "page_steaming");
    lv_obj_t *page_steam_left = lv_100ask_page_manager_page_create(page_manager, "page_steam_left");
    lv_obj_t *page_set = lv_100ask_page_manager_page_create(page_manager, "page_set");
    lv_obj_t *page_cook_details = lv_100ask_page_manager_page_create(page_manager, "page_cook_details");
    lv_obj_t *page_multistage = lv_100ask_page_manager_page_create(page_manager, "page_multistage");
    lv_obj_t *page_smart_cook = lv_100ask_page_manager_page_create(page_manager, "page_smart_cook");
    lv_obj_t *page_close_heat = lv_100ask_page_manager_page_create(page_manager, "page_close_heat");
    lv_obj_t *page_steam_right = lv_100ask_page_manager_page_create(page_manager, "page_steam_right");
    lv_obj_t *page_steam_assist = lv_100ask_page_manager_page_create(page_manager, "page_steam_assist");
    lv_obj_t *page_production_main = lv_100ask_page_manager_page_create(page_manager, "page_production_main");
    lv_obj_t *page_screen_main = lv_100ask_page_manager_page_create(page_manager, "page_screen_main");
    lv_obj_t *page_screen_line = lv_100ask_page_manager_page_create(page_manager, "page_screen_line");
    lv_obj_t *page_screen_lcd = lv_100ask_page_manager_page_create(page_manager, "page_screen_lcd");
    lv_obj_t *page_screen_touch = lv_100ask_page_manager_page_create(page_manager, "page_screen_touch");
    lv_obj_t *page_screen_click = lv_100ask_page_manager_page_create(page_manager, "page_screen_click");
    lv_obj_t *page_screen_aging = lv_100ask_page_manager_page_create(page_manager, "page_screen_aging");
    lv_obj_t *page_smart_test = lv_100ask_page_manager_page_create(page_manager, "page_smart_test");

    lv_100ask_page_manager_set_page_init(main_page, init_main_page);
    lv_100ask_page_manager_set_page_init(page_hood, lv_page_hood_init);
    lv_100ask_page_manager_set_page_init(page_steamoven, lv_page_steamoven_init);
    lv_100ask_page_manager_set_page_init(page_smartrecipes, lv_page_smartrecipes_init);
    lv_100ask_page_manager_set_page_init(page_steaming, lv_page_steaming_init);
    lv_100ask_page_manager_set_page_init(page_steam_left, lv_page_steam_left_init);
    lv_100ask_page_manager_set_page_init(page_set, lv_page_set_init);
    lv_100ask_page_manager_set_page_init(page_cook_details, lv_page_cook_details_init);
    lv_100ask_page_manager_set_page_init(page_multistage, lv_page_multistage_init);
    lv_100ask_page_manager_set_page_init(page_smart_cook, lv_page_smart_cook_init);
    lv_100ask_page_manager_set_page_init(page_close_heat, lv_page_close_heat_init);
    lv_100ask_page_manager_set_page_init(page_steam_right, lv_page_steam_right_init);
    lv_100ask_page_manager_set_page_init(page_steam_assist, lv_page_steam_assist_init);
    lv_100ask_page_manager_set_page_init(page_production_main, lv_page_production_main_init);
    lv_100ask_page_manager_set_page_init(page_screen_main, lv_page_screen_main_init);
    lv_100ask_page_manager_set_page_init(page_screen_line, lv_page_screen_line_init);
    lv_100ask_page_manager_set_page_init(page_screen_lcd, lv_page_screen_lcd_init);
    lv_100ask_page_manager_set_page_init(page_screen_touch, lv_page_screen_touch_init);
    lv_100ask_page_manager_set_page_init(page_screen_click, lv_page_screen_click_init);
    lv_100ask_page_manager_set_page_init(page_screen_aging, lv_page_screen_aging_init);
    lv_100ask_page_manager_set_page_init(page_smart_test, lv_page_smart_test_init);
#if LV_100ASK_PAGE_MANAGER_COSTOM_ANIMARION
    lv_100ask_page_manager_set_open_page_anim(main_page, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(main_page, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_hood, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_hood, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_steamoven, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_steamoven, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_smartrecipes, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_smartrecipes, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_steam_left, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_steam_left, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_steaming, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_steaming, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_set, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_set, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_cook_details, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_cook_details, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_multistage, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_multistage, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_smart_cook, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_smart_cook, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_close_heat, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_close_heat, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_steam_right, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_steam_right, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_steam_assist, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_steam_assist, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_screen_line, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_screen_line, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_production_main, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_production_main, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_screen_main, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_screen_main, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_screen_lcd, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_screen_lcd, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_screen_touch, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_screen_touch, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_screen_click, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_screen_click, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_screen_aging, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_screen_aging, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_smart_test, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_smart_test, close_page_anim);
#endif
    lv_100ask_page_manager_set_main_page(page_manager, main_page);
    lv_100ask_page_manager_set_open_page(NULL, "main_page");
}
