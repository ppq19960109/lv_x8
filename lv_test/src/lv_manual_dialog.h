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
    enum MANUAL_INDEX_ENUM
    {
        MANUAL_INDEX_GENERAL = 0x01,
        MANUAL_INDEX_RESERVE,
    };
    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     ***********************/
    lv_obj_t *get_manual_layer(void);
    void clean_manual_layer(void);

    lv_obj_t *lv_manual_cook_dialog(const char *content, lv_event_cb_t event_cb);
    lv_obj_t *lv_manual_dialog1(const char *content, const char *cancel, const char *confirm, lv_event_cb_t event_cb);
    lv_obj_t *lv_manual_reserve_dialog(const char *text_before, const char *text_after, const char *text_confirm, const int hour, lv_event_cb_t event_cb);
    int lv_get_reserve_dialog_time(void);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
