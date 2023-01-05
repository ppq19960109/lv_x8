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
#include "lv_dev_state.h"
#include "lv_custom_item.h"
#include "wifi_list.h"
    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/
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
    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    void register_page_property_change_cb(void (*cb)(const char *key, void *value));
    void lv_test_widgets(void);
    lv_obj_t *lv_custom_image_button_create(lv_obj_t *parent, const void *img_src, lv_coord_t width, lv_coord_t height,
                                            lv_coord_t x, lv_coord_t y);
    lv_obj_t *lv_page_back_bar_init(lv_obj_t *parent, const char *name, const char *title, lv_event_cb_t event_cb);
    lv_obj_t *lv_page_top_bar_init(lv_obj_t *parent, const int index);
    lv_obj_t *lv_rotate_anim(lv_obj_t *obj, const int run);
    lv_obj_t *lv_custom_text_btn_create(lv_obj_t *parent, const char *text);

    void lv_page_hood_init(lv_obj_t *page);
    void lv_page_steamoven_init(lv_obj_t *page);
    void lv_page_smartrecipes_init(lv_obj_t *page);
    void lv_page_steaming_init(lv_obj_t *page);
    void lv_page_steam_left_init(lv_obj_t *page);
    void lv_page_multistage_init(lv_obj_t *page);
    void lv_page_set_init(lv_obj_t *page);
    void lv_page_cook_details_init(lv_obj_t *page);

    void lv_page_local_set_create(lv_obj_t *page);
    void lv_page_wifi_create(lv_obj_t *page);
    void lv_page_wifi_visible(const int visible);
    void lv_wifi_property_change_cb(const char *key, void *value);
    steamoven_mode_t *get_steamoven_mode(unsigned char mode_index);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
