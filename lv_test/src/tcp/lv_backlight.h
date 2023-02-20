#ifndef LV_BACKLIGHT_H
#define LV_BACKLIGHT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "main.h"
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
int backlightEnable(void);
int backlightDisable(void);
int backlightSet(unsigned char value);
int backlightGet(void);
void setClockTimestamp(long timestamp);
void setClockTime(int hours, int minutes);
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
