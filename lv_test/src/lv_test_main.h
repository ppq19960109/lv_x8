/**
 * @file lv_test_main.h
 *
 */

#ifndef LV_TEST_MAIN_H
#define LV_TEST_MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lv_test.h"
#include "main.h"
#include "lv_backlight.h"
#include "lv_dev_state.h"
#include "uds_protocol.h"

#include "lv_custom_item.h"
#include "lv_auto_dialog.h"
#include "lv_manual_dialog.h"
#include "hv_http_client.h"
    /*********************
     *      DEFINES
     *********************/
#define recipesImagesPath "S:/oem/marssenger/"
#define themesImagesPath "S:/oem/marssenger/themes/X8GCZ/"
#define themesWindowBackgroundColor 0x1A1A1A
#define themesPopupWindowColor 0x333333
#define themesTextColor 0xE68855
#define themesTextColor2 0xA2A2A2
    /**********************
     *      TYPEDEFS
     **********************/
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
    void custom_tick_update(struct timeval *tv);
    void register_page_property_change_cb(void (*cb)(const char *key, void *value));
    void lv_test_widgets(void);
    void lv_sleep_wakeup(void);

    lv_obj_t *lv_page_back_bar_init(lv_obj_t *parent, const char *name, const char *title, lv_event_cb_t event_cb);
    lv_obj_t *lv_custom_back_button_create(lv_obj_t *parent, const void *img_src, const char *text);
    lv_obj_t *lv_page_top_bar_init(lv_obj_t *parent, const int index);

    void lv_page_hood_init(lv_obj_t *page);
    void lv_page_smart_cook_init(lv_obj_t *page);
    void lv_page_close_heat_init(lv_obj_t *page);
    void lv_page_steamoven_init(lv_obj_t *page);
    void lv_page_smartrecipes_init(lv_obj_t *page);
    void lv_page_steaming_init(lv_obj_t *page);
    void lv_page_steam_left_init(lv_obj_t *page);
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

#endif /*LV_DEMO_WIDGETS_H*/
