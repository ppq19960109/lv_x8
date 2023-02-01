/**
 * @file lv_test.h
 *
 */

#ifndef LV_TEST_H
#define LV_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <pthread.h>
#include "../lvgl/lvgl.h"

#include "src/lv_test_main.h"
#include "lv_100ask_page_manager_test/simple_test.h"
#include "lv_100ask_pinyin_ime_test/simple_test.h"
#include "../lv_100ask_pinyin_ime/lv_100ask_pinyin_ime.h"
#include "../lv_100ask_page_manager/lv_100ask_page_manager.h"
/*********************
 *      DEFINES
 *********************/
#define recipesImagesPath "S:/oem/marssenger/"
#define themesImagesPath "S:/oem/marssenger/themes/X8GCZ/"
#define themesWindowBackgroundColor 0x1A1A1A
#define themesPopupWindowColor 0x333333
#define themesTextColor 0xE68855
#define themesTextColor2 0xA2A2A2
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
