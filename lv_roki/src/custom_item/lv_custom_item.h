#ifndef LV_CUSTOM_ITEM_H
#define LV_CUSTOM_ITEM_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*********************
     *      INCLUDES
     *********************/
#include "lv_roki/lv_roki.h"
    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    lv_obj_t *lv_custom_image_button_create(lv_obj_t *parent, const void *img_src, lv_coord_t x, lv_coord_t y);
    lv_obj_t *lv_rotate_anim(lv_obj_t *obj, const int run);
    lv_obj_t *lv_custom_text_btn_create(lv_obj_t *parent, const char *text);
    lv_obj_t *lv_custom_btn_array_create(lv_obj_t *parent, const char *text[], const char count, lv_event_cb_t event_cb);

    lv_obj_t *radiobutton_create(lv_obj_t *parent, const char *txt);
    lv_obj_t *lv_divider_create(lv_obj_t *parent);

    lv_obj_t *lv_custom_vapour_create(lv_obj_t *parent, int vapour, const lv_font_t *font, lv_coord_t x_ofs, lv_coord_t y_ofs);
    lv_obj_t *lv_custom_temp_create(lv_obj_t *parent, int temp, const lv_font_t *font, lv_coord_t x_ofs, lv_coord_t y_ofs);
    lv_obj_t *lv_custom_temp_fix_width_create(lv_obj_t *parent, int temp, const lv_font_t *font, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_coord_t w, const char *unit_before);
    lv_obj_t *lv_custom_time_create(lv_obj_t *parent, int time, const lv_font_t *font, lv_coord_t x_ofs, lv_coord_t y_ofs);

    lv_obj_t *lv_page_indicator_create(lv_obj_t *parent, int count);
    void lv_page_indicator_check(lv_obj_t *page_indicator, int index);

    char *time_roller_options(int max_time);
    char *temp_roller_options(int min_temp, int max_temp);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
