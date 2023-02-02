#ifndef LV_MANUAL_DIALOG_H
#define LV_MANUAL_DIALOG_H

#ifdef __cplusplus
extern "C"
{
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

    /**********************
     * GLOBAL PROTOTYPES
     ***********************/
    lv_obj_t *get_manual_layer();
    void clean_manual_layer();

    lv_obj_t *lv_manual_cook_dialog(const char *content, lv_event_cb_t event_cb);
    lv_obj_t *lv_manual_dialog1(const char *content, const char *cancel, const char *confirm, lv_event_cb_t event_cb);
    lv_obj_t *lv_manual_reserve_dialog(const char *content, lv_event_cb_t event_cb);
    int lv_get_reserve_dialog_time(lv_obj_t *reserve_dialog);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
