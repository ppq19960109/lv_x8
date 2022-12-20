

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
int g_wifi_state = 0;
lv_style_t roller_style_unselected, roller_style_selected;
static lv_obj_t *icon_wifi;
/**********************
 *  STATIC VARIABLES
 **********************/
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
    if (page->page_update_cb != NULL)
        page->page_update_cb();

    if (page->page_property_change_cb != NULL)
        page_property_change_cb = page->page_property_change_cb;
    else
        LV_LOG_USER("page name:%s,page_property_change_cb is null", page->name);
    LV_LOG_USER("open page anim. name:%s", page->name);

    lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
}

/*close page anim*/
static void close_page_anim(lv_obj_t *obj)
{
    /*Do something with LVGL*/
    LV_LOG_USER("close page anim.");
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
}
#endif
//-------------------------------------------------
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
            if (lv_page_current_exist_page("page_steam_left") || lv_page_current_exist_page("PageMultistage"))
                lv_100ask_page_manager_set_open_page(NULL, "page_steaming");
            else if (lv_page_current_exist_page("PageSmartRecipes") || lv_page_current_exist_page("PageCookDetails"))
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
        int LStOvState = get_attr_value_int("LStOvState");
        int RStOvState = get_attr_value_int("RStOvState");
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
        wifi_list_each(NULL);
        cJSON_Delete(root);
    }
    else if (strcmp("WifiState", key) == 0)
    {
        g_wifi_state = get_value_int(value);
        
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
}
//-------------------------------------------------
static void init_style()
{
    lv_style_init(&roller_style_unselected);
    lv_style_set_text_font(&roller_style_unselected, &lv_font_SiYuanHeiTi_Normal_30);
    lv_style_set_text_color(&roller_style_unselected, lv_color_hex(0xffffff));
    lv_style_set_text_align(&roller_style_unselected, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_line_space(&roller_style_unselected, 16);

    lv_style_init(&roller_style_selected);
    lv_style_set_text_font(&roller_style_selected, &lv_font_SiYuanHeiTi_Normal_34);
    lv_style_set_text_color(&roller_style_selected, lv_color_hex(themesTextColor));
    lv_style_set_text_align(&roller_style_selected, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_line_space(&roller_style_selected, 16);
}
void lv_test_widgets(void)
{
    LV_LOG_USER("lv_test_widgets...");
    // lv_100ask_pinyin_ime_simple_test();
    // lv_100ask_page_manager_simple_test();
    // lv_100ask_demo_layer();
    // return 0;
    lv_dev_init();
    init_style();

    lv_obj_t *win_bg = lv_img_create(lv_scr_act());
    lv_img_set_src(win_bg, themesImagesPath "window-background.png");

    lv_obj_t *home_bar = lv_obj_create(lv_scr_act());
    lv_obj_set_size(home_bar, 120, LV_PCT(100));
    lv_obj_set_align(home_bar, LV_ALIGN_RIGHT_MID);

    lv_obj_t *newline2 = lv_img_create(home_bar);
    lv_img_set_src(newline2, themesImagesPath "icon_newline2.png");
    lv_obj_set_size(newline2, 2, LV_SIZE_CONTENT);
    lv_obj_set_align(newline2, LV_ALIGN_LEFT_MID);

    lv_obj_t *clock_text = lv_label_create(home_bar);
    lv_obj_align(clock_text, LV_ALIGN_TOP_MID, 0, 17);
    lv_obj_set_style_text_color(clock_text, lv_color_hex(themesTextColor2), 0);
    lv_label_set_text(clock_text, "12:45");

    lv_obj_t *icon_newline1 = lv_img_create(home_bar);
    lv_img_set_src(icon_newline1, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline1, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline1, LV_ALIGN_TOP_MID, 0, 54);
    icon_wifi = lv_img_create(home_bar);
    lv_img_set_src(icon_wifi, themesImagesPath "icon_wifi_disconnect.png");
    lv_obj_align(icon_wifi, LV_ALIGN_TOP_MID, 0, 80);

    lv_obj_t *icon_newline2 = lv_img_create(home_bar);
    lv_img_set_src(icon_newline2, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline2, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline2, LV_ALIGN_TOP_MID, 0, 133);
    lv_obj_t *icon_set = lv_img_create(home_bar);
    lv_img_set_src(icon_set, themesImagesPath "icon_set.png");
    lv_obj_align(icon_set, LV_ALIGN_TOP_MID, 0, 163);

    lv_obj_t *icon_newline3 = lv_img_create(home_bar);
    lv_img_set_src(icon_newline3, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline3, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline3, LV_ALIGN_TOP_MID, 0, 223);
    lv_obj_t *icon_alarm = lv_img_create(home_bar);
    lv_img_set_src(icon_alarm, themesImagesPath "icon_alarm.png");
    lv_obj_align(icon_alarm, LV_ALIGN_TOP_MID, 0, 248);

    lv_obj_t *icon_newline4 = lv_img_create(home_bar);
    lv_img_set_src(icon_newline4, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline4, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline4, LV_ALIGN_TOP_MID, 0, 314);
    lv_obj_t *icon_standby = lv_img_create(home_bar);
    lv_img_set_src(icon_standby, themesImagesPath "icon_standby.png");
    lv_obj_align(icon_standby, LV_ALIGN_TOP_MID, 0, 339);
    //****************************************************
    lv_obj_t *home = lv_obj_create(lv_scr_act());
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

    lv_100ask_page_manager_set_page_init(main_page, init_main_page);
    lv_100ask_page_manager_set_page_init(page_hood, lv_page_hood_init);
    lv_100ask_page_manager_set_page_init(page_steamoven, lv_page_steamoven_init);
    lv_100ask_page_manager_set_page_init(page_smartrecipes, lv_page_smartrecipes_init);
    lv_100ask_page_manager_set_page_init(page_steaming, lv_page_steaming_init);
    lv_100ask_page_manager_set_page_init(page_steam_left, lv_page_steam_left_init);
    lv_100ask_page_manager_set_page_init(page_set, lv_page_set_init);
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
#endif
    lv_100ask_page_manager_set_main_page(page_manager, main_page);
    lv_100ask_page_manager_set_open_page(NULL, "main_page");

    lv_100ask_page_manager_set_load_page_event(icon_set, NULL, "page_set");
}
