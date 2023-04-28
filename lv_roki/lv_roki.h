/**
 * @file lv_roki.h
 *
 */
#ifndef LV_ROKI_H
#define LV_ROKI_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/reboot.h>
#include <pthread.h>
#include <errno.h>

#include <sys/select.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include "cJSON.h"
#include "base64.h"
#include "curl/curl.h"

#include "cmd_run.h"
#include "commonFunc.h"
#include "networkFunc.h"
// #include "signalQuit.h"
#include "POSIXTimer.h"
// #include "UartCfg.h"
//---------------------------------------
#include "lvgl/lvgl.h"

#include "lv_100ask_pinyin_ime/lv_100ask_pinyin_ime.h"
#include "lv_100ask_page_manager/lv_100ask_page_manager.h"

#include "tcp/lv_backlight.h"
#include "tcp/lv_dev_state.h"
#include "tcp/uds_protocol.h"
#include "tcp/wifi_list.h"
#include "tcp/hv_http_client.h"

#include "custom_item/lv_custom_item.h"
#include "custom_item/lv_cycle_scroll.h"
#include "dialog/lv_auto_dialog.h"
#include "dialog/lv_manual_dialog.h"
    /*********************
     *      DEFINES
     *********************/
#define recipesImagesPath "S:/oem/marssenger/"
#define themesImagesPath "S:/assets/images/"
#define themesWindowBackgroundColor 0x1A1A1A
#define themesPopupWindowColor 0x333333
#define themesTextColor 0xE68855
#define themesTextColor2 0xA2A2A2

#define publicImagesPath "S:/oem/robam/images/"
    /**********************
     *      TYPEDEFS
     **********************/
    typedef struct
    {
        lv_ft_info_t FZLTHC_80;
        lv_ft_info_t FZLTHC_66;
        lv_ft_info_t FZLTHC_60;
        lv_ft_info_t FZLTHC_54;
        lv_ft_info_t FZLTHC_48;
        lv_ft_info_t FZLTHC_44;
        lv_ft_info_t FZLTHC_40;
        lv_ft_info_t FZLTHC_32;
        lv_ft_info_t FZLTHC_30;
        lv_ft_info_t FZLTHJW_48;
        lv_ft_info_t FZLTHJW_44;
        lv_ft_info_t FZLTHJW_40;
        lv_ft_info_t FZLTHJW_36;
        lv_ft_info_t FZLTHJW_32;
        lv_ft_info_t FZLTHJW_30;
        lv_ft_info_t FZLTHJW_16;
    } TYPE_FONT_T;
    typedef struct
    {
        unsigned int color1[2];
    } COLOR_FONT_T;
    extern TYPE_FONT_T g_robam_font;

    typedef struct
    {
        char ssid[40];
        char psk[40];
        int encryp;
    } wifi_info_t;
    extern wifi_info_t g_wifi_info;

    LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_20);
    LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_24);
    LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_26);
    LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_30);
    LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_34);
    LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_40);
    LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_50);
    extern lv_style_t roller_style_unselected, roller_style_selected;
    extern lv_style_t slider_style_main, slider_style_indicator, slider_style_knob;
    extern lv_style_t switch_style_indicator, switch_style_indicator_check, switch_style_knob;
    extern int g_wifi_state;
    extern pthread_mutex_t g_mutex;
    extern lv_obj_t *manual_scr, *main_scr;
    extern char scan_count;
    extern char g_versionCheckState;
    extern char g_wifiPageStatus;
    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    void register_page_property_change_cb(void (*cb)(const char *key, void *value));
    const char *getImagePath(const char *directory, const char *file);
    const char *getThemesPath(const char *file);
    lv_color_t getThemesFontColor1(void);
    void lv_theme_switch(const int theme_index);
    void lv_sleep_wakeup(void);

    void lv_roki_widgets(void);
    lv_obj_t *lv_page_back_bar_init(lv_obj_t *parent, const char *name, const char *title, lv_event_cb_t event_cb);
    lv_obj_t *lv_page_bottom_bar_init(lv_obj_t *parent, const char *left_btn_text, const char *right_btn_text, lv_event_cb_t event_cb);
    lv_obj_t *lv_custom_back_button_create(lv_obj_t *parent, const void *img_src, const char *text);
    lv_obj_t *lv_page_top_bar_init(lv_obj_t *parent);
    void lv_page_top_bar_reinit(void);
    void lv_page_top_hide(int index);
    void lv_page_top_update_wifi(int status);
    void lv_page_top_update_time(long timestamp);

    void lv_page_main_init(lv_obj_t *page);
    void lv_page_hood_init(lv_obj_t *page);
    void lv_page_smart_cook_init(lv_obj_t *page);
    void lv_page_close_heat_init(lv_obj_t *page);
    void lv_page_steamoven_init(lv_obj_t *page);
    void lv_page_smartrecipes_init(lv_obj_t *page);
    void lv_page_steaming_init(lv_obj_t *page);
    void lv_page_steam_mode_init(lv_obj_t *page);
    void lv_page_steam_right_init(lv_obj_t *page);
    void lv_page_steam_assist_init(lv_obj_t *page);
    void lv_page_multistage_init(lv_obj_t *page);
    void lv_page_set_init(lv_obj_t *page);
    void lv_page_cook_details_init(lv_obj_t *page);

    void lv_page_set_tabview_set(int index);
    void lv_page_local_set_create(lv_obj_t *page);
    void lv_page_local_set_visible(const int visible);
    void lv_page_wifi_create(lv_obj_t *page);
    void lv_page_wifi_visible(const int visible);
    void lv_wifi_property_change_cb(const char *key, void *value);
    void wifi_connecting_change(char state);
    lv_obj_t *lv_about_bind_dialog(const char *topText, const char *centerText);
    void lv_update_property_change_cb(const char *key, void *value);
    void lv_update_state(int index);
    void lv_page_about_create(lv_obj_t *page);
    void lv_page_about_visible(const int visible);
    void lv_page_update_create(lv_obj_t *page);
    void lv_page_update_visible(const int visible);
    void lv_page_reset_create(lv_obj_t *page);

    void switch_value_state(lv_obj_t *sw, const char state);
    void recipe_cook_start(recipe_t *recipe, const int reserve_time);
    steamoven_mode_t *get_steamoven_mode(unsigned char mode_index);
    void production_mode(const char state);

    void lv_page_production_main_init(lv_obj_t *page);
    void lv_page_screen_main_init(lv_obj_t *page);
    void lv_page_screen_line_init(lv_obj_t *page);
    void lv_page_screen_lcd_init(lv_obj_t *page);
    void lv_page_screen_touch_init(lv_obj_t *page);
    void lv_page_screen_click_init(lv_obj_t *page);
    void lv_page_screen_aging_init(lv_obj_t *page);
    void lv_page_smart_test_init(lv_obj_t *page);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
