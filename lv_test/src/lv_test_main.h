/**
 * @file lv_test_main.h
 *
 */

#ifndef LV_TEST_MAIN_H
#define LV_TEST_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lv_test.h"
#include "cmd_run.h"
#include "commonFunc.h"
#include "networkFunc.h"
#include "signalQuit.h"
#include "POSIXTimer.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_20);
LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_24);
LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_30);
LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_34);
LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_40);
LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_50);

typedef struct
{
    char imgUrl[128];
    char ingredients[960];
    char dishName[80];
    char cookSteps[180];
    char ** details;
    unsigned char details_count;
    int recipeid;
    int recipeType;
    int cookPos;
} recipe_t;
extern recipe_t g_recipes[40];
/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_test_widgets(void);
lv_obj_t * lv_custom_image_button_create(lv_obj_t * parent, const void * img_src, lv_coord_t width, lv_coord_t height,
                                         lv_coord_t x, lv_coord_t y);
lv_obj_t * lv_page_back_bar_init(lv_obj_t * parent, const char * name, const char * title);
lv_obj_t * lv_page_top_bar_init(lv_obj_t * parent, const int index);
lv_obj_t * lv_rotate_anim(lv_obj_t * obj, const int run);
void lv_page_hood_init(lv_obj_t * page);
void lv_page_steamoven_init(lv_obj_t * page);
void lv_page_smartrecipes_init(lv_obj_t * page);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
