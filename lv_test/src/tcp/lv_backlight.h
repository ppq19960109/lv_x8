/**
 * @file lv_test_main.h
 *
 */

#ifndef LV_BACKLIGHT_H
#define LV_BACKLIGHT_H

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

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/
int backlightEnable();
int backlightDisable();
int backlightSet(unsigned char value);
int backlightGet();
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
