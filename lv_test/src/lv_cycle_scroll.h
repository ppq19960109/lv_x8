#ifndef LV_CYCLE_SCROLL_H
#define LV_CYCLE_SCROLL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lv_test.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef void (*cycle_scroll_child_cb)(lv_obj_t * child, int select);
/**********************
 * GLOBAL PROTOTYPES
 ***********************/
lv_obj_t * lv_cycle_scroll_create(lv_obj_t * parent, int width, int height, lv_flex_flow_t flow,
                                  cycle_scroll_child_cb cb);
void cycle_scroll_change(lv_obj_t * cycle_scroll, const int cur_index);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
