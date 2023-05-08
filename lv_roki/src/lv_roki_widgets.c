
/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"
/*********************
 *      DEFINES
 *********************/
#define FONT_PATH_FZLTHC_GBK "/assets/fonts/FZLTHC_GBK.TTF"
#define FONT_PATH_FZLTHJW "/assets/fonts/FZLTHJW.TTF"

int g_wifi_state = 0;
save_settings_t g_save_settings;
lv_style_t roller_style_unselected, roller_style_selected;
lv_style_t slider_style_main, slider_style_indicator, slider_style_knob;
lv_style_t switch_style_indicator, switch_style_indicator_check, switch_style_knob;

char LStOvState = 0;
char LStoveStatus = 0, RStoveStatus = 0;
TYPE_FONT_T g_robam_font;
COLOR_FONT_T g_robam_font_color = {
    color1 : {0xE1C9BE, 0xFFFFFF}
};
/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *win_bg;
static lv_timer_t *sleep_timer, *clock_timer;

static char *themes_path[] = {"S:/oem/robam/images/theme1/", "S:/oem/robam/images/theme2/"};

void (*page_property_change_cb)(const char *key, void *value);
void register_page_property_change_cb(void (*cb)(const char *key, void *value))
{
    page_property_change_cb = cb;
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
static void robam_font_init(void)
{
    g_robam_font.FZLTHC_80.name = FONT_PATH_FZLTHC_GBK;
    g_robam_font.FZLTHC_80.weight = 80;
    lv_ft_font_init(&g_robam_font.FZLTHC_80);

    g_robam_font.FZLTHC_66.name = FONT_PATH_FZLTHC_GBK;
    g_robam_font.FZLTHC_66.weight = 66;
    lv_ft_font_init(&g_robam_font.FZLTHC_66);

    g_robam_font.FZLTHC_60.name = FONT_PATH_FZLTHC_GBK;
    g_robam_font.FZLTHC_60.weight = 60;
    lv_ft_font_init(&g_robam_font.FZLTHC_60);

    g_robam_font.FZLTHC_54.name = FONT_PATH_FZLTHC_GBK;
    g_robam_font.FZLTHC_54.weight = 54;
    lv_ft_font_init(&g_robam_font.FZLTHC_54);

    g_robam_font.FZLTHC_48.name = FONT_PATH_FZLTHC_GBK;
    g_robam_font.FZLTHC_48.weight = 48;
    lv_ft_font_init(&g_robam_font.FZLTHC_48);

    g_robam_font.FZLTHC_44.name = FONT_PATH_FZLTHC_GBK;
    g_robam_font.FZLTHC_44.weight = 44;
    lv_ft_font_init(&g_robam_font.FZLTHC_44);

    g_robam_font.FZLTHC_40.name = FONT_PATH_FZLTHC_GBK;
    g_robam_font.FZLTHC_40.weight = 40;
    lv_ft_font_init(&g_robam_font.FZLTHC_40);

    g_robam_font.FZLTHC_32.name = FONT_PATH_FZLTHC_GBK;
    g_robam_font.FZLTHC_32.weight = 32;
    lv_ft_font_init(&g_robam_font.FZLTHC_32);

    g_robam_font.FZLTHC_30.name = FONT_PATH_FZLTHC_GBK;
    g_robam_font.FZLTHC_30.weight = 30;
    lv_ft_font_init(&g_robam_font.FZLTHC_30);

    g_robam_font.FZLTHJW_48.name = FONT_PATH_FZLTHJW;
    g_robam_font.FZLTHJW_48.weight = 48;
    lv_ft_font_init(&g_robam_font.FZLTHJW_48);

    g_robam_font.FZLTHJW_44.name = FONT_PATH_FZLTHJW;
    g_robam_font.FZLTHJW_44.weight = 44;
    lv_ft_font_init(&g_robam_font.FZLTHJW_44);

    g_robam_font.FZLTHJW_40.name = FONT_PATH_FZLTHJW;
    g_robam_font.FZLTHJW_40.weight = 40;
    lv_ft_font_init(&g_robam_font.FZLTHJW_40);

    g_robam_font.FZLTHJW_36.name = FONT_PATH_FZLTHJW;
    g_robam_font.FZLTHJW_36.weight = 36;
    lv_ft_font_init(&g_robam_font.FZLTHJW_36);

    g_robam_font.FZLTHJW_32.name = FONT_PATH_FZLTHJW;
    g_robam_font.FZLTHJW_32.weight = 32;
    lv_ft_font_init(&g_robam_font.FZLTHJW_32);

    g_robam_font.FZLTHJW_30.name = FONT_PATH_FZLTHJW;
    g_robam_font.FZLTHJW_30.weight = 30;
    lv_ft_font_init(&g_robam_font.FZLTHJW_30);

    g_robam_font.FZLTHJW_16.name = FONT_PATH_FZLTHJW;
    g_robam_font.FZLTHJW_16.weight = 16;
    lv_ft_font_init(&g_robam_font.FZLTHJW_16);
}

const char *getImagePath(const char *directory, const char *file)
{
    static char path[80];
    sprintf(path, "%s%s", directory, file);
    return path;
}
const char *getThemesPath(const char *file)
{
    return getImagePath(themes_path[g_save_settings.themesIndex], file);
}
lv_color_t getThemesFontColor1(void)
{
    return lv_color_hex(g_robam_font_color.color1[g_save_settings.themesIndex]);
}
void lv_theme_switch(const int theme_index)
{
    g_save_settings.themesIndex = theme_index;
    lv_img_set_src(win_bg, getThemesPath("bg_work_bg.png"));
    lv_page_reset_page();
    lv_page_top_bar_reinit();
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
            if (lv_page_current_exist_page("page_steam_left") || lv_page_current_exist_page("page_multistage") ||
                lv_page_current_exist_page("page_steam_right") || lv_page_current_exist_page("page_steam_assist"))
                lv_100ask_page_manager_set_open_page(NULL, "page_steaming");
            else if (lv_page_current_exist_page("page_smartrecipes") ||
                     lv_page_current_exist_page("page_cook_details"))
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
        if (LStOvState == WORK_STATE_STOP)
        {
            steamInterfaceChange(0);
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
            // if (g_wifi_state == RK_WIFI_State_CONNECTED)
            // {
            //     lv_img_set_src(icon_wifi, themesImagesPath "icon_wifi_half_connect.png");
            // }
            // else if (g_wifi_state == RK_WIFI_State_LINK_CONNECTED)
            // {
            //     lv_img_set_src(icon_wifi, themesImagesPath "icon_wifi_connected.png");
            // }
            // else
            // {
            //     lv_img_set_src(icon_wifi, themesImagesPath "icon_wifi_disconnect.png");
            // }
        }
    }
    else if (strcmp("NtpTimestamp", key) == 0)
    {
        long t = get_value_int(value);
        setClockTimestamp(t);
        // getCurrentTime();
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
            // if (LStOvState == WORK_STATE_PREHEAT || LStOvState == WORK_STATE_RUN || LStOvState ==
            // WORK_STATE_PAUSE || RStOvState == WORK_STATE_PREHEAT || RStOvState == WORK_STATE_RUN || RStOvState ==
            // WORK_STATE_PAUSE)
            lv_auto_dialog1("蒸烤箱工作中,需散热,烟机无法关闭", NULL, "好的", NULL);
            break;
        case 3:
            // if (LStOvState == WORK_STATE_PREHEAT || LStOvState == WORK_STATE_RUN || LStOvState ==
            // WORK_STATE_PAUSE)
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
    else if (strcmp("OTAProgress", key) == 0)
    {
        int data = get_value_int(value);
        lv_auto_upgrade_dialog5_progress(data, "通讯板");
    }
    if (page_property_change_cb != NULL)
        page_property_change_cb(key, value);
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
static void lv_clock_timer_cb(lv_timer_t *timer)
{
    uint32_t tick = lv_tick_get();
    LV_LOG_USER("%s,tick:%u", __func__, tick);
    lv_page_top_update_time(0);
}
static void lv_sleep_timer_cb(lv_timer_t *timer)
{
    uint32_t tick = lv_tick_get();
    LV_LOG_USER("%s,sleep...tick:%u", __func__, tick);

    lv_timer_pause(sleep_timer);
    // lv_auto_screen_dialog4(0);
}
void lv_sleep_wakeup(void)
{
    // LV_LOG_USER("%s,wakeup...", __func__);
    lv_timer_reset(sleep_timer);
    lv_timer_resume(sleep_timer);
    // lv_auto_screen_dialog4_close();
}

/**********************************************************************
**函数名称: animation_play_over
**函数说明: 开机动画播放结束事件回调
**输入参数: e: lv_event_t
**输出参数: none
**返 回 值: none
************************************************************************/
static void animation_play_over(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_del(obj);
    LV_LOG_USER("%s,code:%d child_cnt:%d\n", __func__, e->code, lv_obj_get_child_cnt(lv_layer_top()));
}
static void boot_video(void)
{
    if (lv_ffmpeg_get_frame_num("/assets/images/boot_9068a.mp4") <= 0)
        return;
    lv_obj_t *player = lv_ffmpeg_player_create(lv_layer_top());
    lv_ffmpeg_player_set_src(player, "/assets/images/boot_9068a.mp4");
    lv_ffmpeg_player_set_auto_restart(player, false);
    lv_ffmpeg_player_set_cmd(player, LV_FFMPEG_PLAYER_CMD_START);

    lv_obj_add_event_cb(player, animation_play_over, _LV_EVENT_LAST, NULL);
}

static void save_settings_init(void)
{
    int len = 1;
    H_Kv_Get("firstStartup", &g_save_settings.firstStartup, &len);
    H_Kv_Get("sleepSwitch", &g_save_settings.sleepSwitch, &len);
    H_Kv_Get("sleepTime", &g_save_settings.sleepTime, &len);
    H_Kv_Get("screenSaverIndex", &g_save_settings.screenSaverIndex, &len);
    H_Kv_Get("brightness", &g_save_settings.brightness, &len);
    H_Kv_Get("wifiEnable", &g_save_settings.wifiEnable, &len);
    H_Kv_Get("themesIndex", &g_save_settings.themesIndex, &len);

    if (g_save_settings.brightness == 0)
    {
        g_save_settings.brightness = 6;
        H_Kv_Set("brightness", &g_save_settings.brightness, 1, 0);
    }
    backlightSet(g_save_settings.brightness);
}

lv_obj_t *manual_scr = NULL, *main_scr = NULL;
void lv_roki_widgets(void)
{
    main_scr = lv_scr_act();
    // manual_scr = lv_scr_act();
    LV_LOG_USER("%s,manual_scr:%p main_scr:%p", __func__, manual_scr, main_scr);
    // lv_100ask_pinyin_ime_simple_test();
    // lv_100ask_page_manager_simple_test();
    // lv_obj_t *bg = lv_img_create(lv_scr_act());
    // lv_img_set_src(bg, "S:/assets/images/bg_home_bg.png");
    // cycle_scroll_test(main_scr);
    // return;
    backlightEnable();
    save_settings_init();

    register_property_change_cb(property_change_cb);
    robam_font_init();
    init_style();

    clock_timer = lv_timer_create(lv_clock_timer_cb, 59000, NULL);
    sleep_timer = lv_timer_create(lv_sleep_timer_cb, 60000, NULL);

    // boot_video();
    win_bg = lv_img_create(main_scr);
    lv_img_set_src(win_bg, getThemesPath("bg_work_bg.png"));
    //****************************************************
    lv_obj_t *page_manager = lv_100ask_page_manager_create(main_scr);

    lv_page_top_bar_init(main_scr);

    lv_obj_t *main_page = lv_100ask_page_manager_page_create(page_manager, "main_page");
    lv_obj_t *page_hood = lv_100ask_page_manager_page_create(page_manager, "page_hood");
    lv_obj_t *page_steamoven = lv_100ask_page_manager_page_create(page_manager, "page_steamoven");
    lv_obj_t *page_smartrecipes = lv_100ask_page_manager_page_create(page_manager, "page_smartrecipes");
    lv_obj_t *page_steam_mode = lv_100ask_page_manager_page_create(page_manager, "page_steam_mode");
    lv_obj_t *page_cook_tab = lv_100ask_page_manager_page_create(page_manager, "page_cook_tab");
    lv_obj_t *page_cook_details = lv_100ask_page_manager_page_create(page_manager, "page_cook_details");
    lv_obj_t *page_multistage = lv_100ask_page_manager_page_create(page_manager, "page_multistage");
    lv_obj_t *page_smart_cook = lv_100ask_page_manager_page_create(page_manager, "page_smart_cook");
    lv_obj_t *page_steam_right = lv_100ask_page_manager_page_create(page_manager, "page_steam_right");
    lv_obj_t *page_cook_bake = lv_100ask_page_manager_page_create(page_manager, "page_cook_bake");
    lv_obj_t *page_production_main = lv_100ask_page_manager_page_create(page_manager, "page_production_main");
    lv_obj_t *page_screen_main = lv_100ask_page_manager_page_create(page_manager, "page_screen_main");
    lv_obj_t *page_screen_line = lv_100ask_page_manager_page_create(page_manager, "page_screen_line");
    lv_obj_t *page_screen_lcd = lv_100ask_page_manager_page_create(page_manager, "page_screen_lcd");
    lv_obj_t *page_screen_touch = lv_100ask_page_manager_page_create(page_manager, "page_screen_touch");
    lv_obj_t *page_screen_click = lv_100ask_page_manager_page_create(page_manager, "page_screen_click");
    lv_obj_t *page_screen_aging = lv_100ask_page_manager_page_create(page_manager, "page_screen_aging");
    lv_obj_t *page_smart_test = lv_100ask_page_manager_page_create(page_manager, "page_smart_test");

    lv_100ask_page_manager_set_page_init(main_page, lv_page_main_init);
    lv_100ask_page_manager_set_page_init(page_hood, lv_page_hood_init);
    lv_100ask_page_manager_set_page_init(page_steamoven, lv_page_steamoven_init);
    lv_100ask_page_manager_set_page_init(page_smartrecipes, lv_page_smartrecipes_init);
    lv_100ask_page_manager_set_page_init(page_steam_mode, lv_page_steam_mode_init);
    lv_100ask_page_manager_set_page_init(page_cook_tab, lv_page_cook_tab_init);
    lv_100ask_page_manager_set_page_init(page_cook_details, lv_page_cook_details_init);
    lv_100ask_page_manager_set_page_init(page_multistage, lv_page_multistage_init);
    lv_100ask_page_manager_set_page_init(page_smart_cook, lv_page_smart_cook_init);
    lv_100ask_page_manager_set_page_init(page_steam_right, lv_page_steam_right_init);
    lv_100ask_page_manager_set_page_init(page_cook_bake, lv_page_cook_bake_init);
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
    lv_100ask_page_manager_set_open_page_anim(page_steam_mode, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_steam_mode, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_cook_tab, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_cook_tab, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_cook_details, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_cook_details, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_multistage, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_multistage, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_smart_cook, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_smart_cook, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_steam_right, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_steam_right, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_cook_bake, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_cook_bake, close_page_anim);
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
