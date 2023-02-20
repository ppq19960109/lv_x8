#ifndef LV_AUTO_DIALOG_H
#define LV_AUTO_DIALOG_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*********************
     *      INCLUDES
     *********************/

    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     ***********************/
    lv_obj_t *get_auto_layer(void);
    void clean_auto_layer(void);

    lv_obj_t *lv_dialog1(lv_obj_t *layer, const char *content, const char *cancel, const char *confirm, const char *top_img, lv_event_cb_t event_cb);
    lv_obj_t *lv_auto_dialog1(const char *content, const char *cancel, const char *confirm, lv_event_cb_t event_cb);
    lv_obj_t *lv_auto_dialog3(void);
    void lv_auto_dialog3_update(const char *content, const char *confirm);
    void lv_auto_dialog3_close(void);
    void lv_auto_screen_dialog4(const char index);
    void lv_auto_screen_dialog4_close(void);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
