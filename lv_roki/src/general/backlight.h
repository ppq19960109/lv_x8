#ifndef _ROKI_BACKLIGHT_H
#define _ROKI_BACKLIGHT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
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
