/**
 * @file lv_100ask_page_manager.h
 *
 */

#ifndef LV_100ASK_PAGE_MANAGER_H
#define LV_100ASK_PAGE_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl/lvgl.h"

#if LV_USE_100ASK_PAGE_MANAGER != 0

/*********************
 *      DEFINES
 *********************/
#define LV_ANIM_EXEC(attr)      (lv_anim_exec_xcb_t)lv_obj_set_##attr

#define ANIM_DEF(start_time, obj, attr, start, end) \
        {start_time, obj, LV_ANIM_EXEC(attr), start, end, 400, lv_anim_path_overshoot, true}

#define LV_ANIM_TIMELINE_WRAPPER_END {0, NULL}


/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;
    lv_obj_t * main_page;
    lv_ll_t history_ll;
    uint8_t cur_depth;
    uint8_t prev_depth;
} lv_100ask_page_manager_t;

typedef struct {
    lv_obj_t obj;
    lv_obj_t *back_btn;
    char * name;
    lv_anim_timeline_t * anim_timeline;
    void (*init)(lv_obj_t * obj);
    void (*open_page)(lv_obj_t * obj);
    void (*close_page)(lv_obj_t * obj);
    void (*page_property_change_cb)(const char *key, void *value);
    void (*page_update_cb)();
} lv_100ask_page_manager_page_t;

typedef struct {
    lv_obj_t * page;
} lv_100ask_page_manager_history_t;

/*Data of anim_timeline*/
typedef struct {
    uint32_t start_time;
    lv_obj_t *obj;
    lv_anim_exec_xcb_t exec_cb;
    int32_t start;
    int32_t end;
    uint16_t duration;
    lv_anim_path_cb_t path_cb;
    bool early_apply;
} lv_anim_timeline_wrapper_t;

/***********************
 * GLOBAL VARIABLES
 ***********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_obj_t * lv_100ask_page_manager_create(lv_obj_t * parent);

/*=====================
 * Setter functions
 *====================*/

void lv_100ask_page_manager_set_main_page(lv_obj_t * obj, lv_obj_t * page);

void lv_100ask_page_manager_set_page_init(lv_obj_t * obj, void (*init)(lv_obj_t  *page));

void lv_100ask_page_manager_set_open_page(lv_obj_t * obj, char *name);

void lv_100ask_page_manager_set_close_page(lv_obj_t * obj, char *name);

void lv_100ask_page_manager_set_load_page_event(lv_obj_t * obj, lv_obj_t * page, char *name);

void lv_100ask_page_manager_set_open_page_anim(lv_obj_t * obj, void (*open_anim)(lv_obj_t  * obj));

void lv_100ask_page_manager_set_close_page_anim(lv_obj_t * obj, void (*close_anim)(lv_obj_t  * obj));
void lv_page_back_previous_page(void);
void lv_page_back_top_page(void);
void lv_page_back_replace_page(lv_obj_t * obj, char * name);
int lv_page_exist_page(const char* name);
/*=====================
 * Getter functions
 *====================*/

/*=====================
 * Other functions
 *====================*/

lv_obj_t * lv_100ask_page_manager_page_create(lv_obj_t * parent, char * name);

/**********************
 *      MACROS
 **********************/

#endif  /*LV_USE_100ASK_PAGE_MANAGER*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_100ASK_PAGE_MANAGER_H*/
