#ifndef LV_CUSTOM_MODE_ROLLER_H
#define LV_CUSTOM_MODE_ROLLER_H

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
    enum COOK_TYPE_ENUM
    {
        COOK_TYPE_STEAMED = 0,
        COOK_TYPE_BAKE,
        COOK_TYPE_FRIED,
        COOK_TYPE_VAPOUR_BAKE,
        COOK_TYPE_MULTISTAGE,
    };
    /**********************
     *      TYPEDEFS
     **********************/
    typedef struct
    {
        unsigned char cooktype;
        lv_obj_t *mode_roller;
        lv_obj_t *temp_roller;
        lv_obj_t *lower_temp_roller;
        lv_obj_t *vapour_roller;
        lv_obj_t *time_roller;
    } steamoven_roller_t;

    typedef struct
    {
        unsigned char cooktype;
        unsigned char mode_index;
        unsigned char mode;
        unsigned char vapour;
        unsigned short temp;
        unsigned short lowertemp;
        unsigned short mintemp;
        unsigned short maxtemp;
        unsigned short time;
        unsigned short maxtime;
        char **vapourmodel;
        unsigned short minlowertemp;
        unsigned short maxlowertemp;
        char name[18];
    } steamoven_mode_t;
    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    steamoven_mode_t *get_steamoven_mode(unsigned char mode_index);
    lv_obj_t *mode_roller_scroll_child_create(lv_obj_t *parent, const char *text, const int num);
    lv_obj_t *lv_custom_mode_roller_create(lv_obj_t *parent, steamoven_roller_t *steamoven_roller, lv_cycle_scroll_t *lv_cycle_scroll);
    void lv_custom_mode_change(steamoven_roller_t *steamoven_roller, unsigned char mode_index);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
