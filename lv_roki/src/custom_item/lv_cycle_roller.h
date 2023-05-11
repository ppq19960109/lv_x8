#ifndef LV_CYCLE_ROLLER_H
#define LV_CYCLE_ROLLER_H

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
    typedef void (*cycle_roller_child_cb)(lv_obj_t *child, char select, char select_end);
    typedef struct lv_cycle_roller
    {
        lv_obj_t obj;
        char cycle_flag;
        char mask_flag;
        char flex_flow;
        char selected_index;
        cycle_roller_child_cb cb;
    } lv_cycle_roller_t;

    /**********************
     * GLOBAL PROTOTYPES
     ***********************/
    lv_obj_t *lv_cycle_roller_create(lv_obj_t *parent);
    void lv_cycle_roller_set_state(lv_obj_t *cycle_roller, char cycle_flag, char mask_flag, char flex_flow, cycle_roller_child_cb cb);
    void cycle_roller_to_index(lv_obj_t *cycle_roller, const unsigned int cur_index, const int event_scroll);
    void cycle_roller_to_userdata(lv_obj_t *cycle_roller, const unsigned int value, const int event_scroll);
    int lv_cycle_roller_get_selected_index(lv_obj_t *cycle_roller);
    lv_obj_t *lv_cycle_roller_get_selected(lv_obj_t *cycle_roller);
    lv_obj_t *lv_cycle_roller_unit_create(lv_obj_t *parent, int width, int height, lv_flex_flow_t flow);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
