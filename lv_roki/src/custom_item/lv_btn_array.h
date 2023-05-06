#ifndef LV_BTN_ARRAY_H
#define LV_BTN_ARRAY_H

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
    lv_obj_t *lv_btn_array_create(lv_obj_t *parent, const char count, lv_event_cb_t event_cb, lv_flex_flow_t flex_flow, int width, int height, int space);
    lv_obj_t *lv_btn_img_array_create(lv_obj_t *parent, const char count, lv_event_cb_t event_cb, lv_flex_flow_t flex_flow, int width, int height, int space);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
