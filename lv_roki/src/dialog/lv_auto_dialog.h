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
    enum AUTO_INDEX_ENUM
    {
        AUTO_INDEX_GENERAL = 0x01,
        AUTO_INDEX_HOODOFF = 0x03,
        AUTO_INDEX_SCREEN,
        AUTO_INDEX_UPGRADE,
        AUTO_INDEX_UPGRADE_CONFIRM,
    };
    enum CLICK_EVENT_ENUM
    {
        CLICK_EVENT_CLOSE = 0x00,
        CLICK_EVENT_CANCEL,
        CLICK_EVENT_CONFIRM,
    };
    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     ***********************/
    lv_obj_t *get_auto_layer(void);
    void clean_auto_layer(void);

    lv_obj_t *lv_dialog1(lv_obj_t *layer, const char *content, const char *cancel, const char *confirm, const char *top_img, const char *top_text, lv_event_cb_t event_cb);
    lv_obj_t *lv_auto_dialog1(const char *content, const char *cancel, const char *confirm, lv_event_cb_t event_cb);
    lv_obj_t *lv_auto_dialog3(void);
    void lv_auto_dialog3_update(const char *content, const char *confirm);
    void lv_auto_dialog3_close(void);
    void lv_auto_screen_dialog4(const char index);
    void lv_auto_screen_dialog4_close(void);
    void lv_auto_upgrade_dialog5(const char *title);
    void lv_auto_upgrade_dialog5_progress(const int progress, const char *title);
    void lv_auto_upgrade_dialog5_close(void);
    void lv_auto_upgrade_confirm_dialog6(const char *version);
    void lv_auto_upgrade_confirm_dialog6_close(void);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
