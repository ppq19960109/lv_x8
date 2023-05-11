#ifndef LV_CYCLE_SCROLL_H
#define LV_CYCLE_SCROLL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/
    typedef void (*cycle_scroll_child_cb)(lv_obj_t *child, char select, char select_end);
    typedef struct lv_cycle_scroll
    {
        char cycle_flag;
        char mask_flag;
        char selected_index;
        cycle_scroll_child_cb cb;
    } lv_cycle_scroll_t;

    /**********************
     * GLOBAL PROTOTYPES
     ***********************/
    lv_obj_t *lv_cycle_scroll_create(lv_obj_t *parent, int width, int height, lv_flex_flow_t flow, lv_cycle_scroll_t *lv_cycle_scroll);
    lv_obj_t *lv_cycle_scroll_unit_create(lv_obj_t *parent, int width, int height, lv_flex_flow_t flow, lv_cycle_scroll_t *lv_cycle_scroll);
    void cycle_scroll_change(lv_obj_t *cycle_scroll, const unsigned int cur_index, const int event_scroll);
    void cycle_scroll_to_userdata(lv_obj_t *cycle_scroll, const unsigned int value, const int event_scroll);
    lv_obj_t *lv_cycle_scroll_get_selected(lv_obj_t *cycle_scroll);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
