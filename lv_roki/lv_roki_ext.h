#ifndef LV_ROKI_EXT_H
#define LV_ROKI_EXT_H

#include "lv_100ask_page_manager/lv_100ask_page_manager.h"

#include "general/KV_linux.h"
#include "roki_dev_state.h"

extern pthread_mutex_t g_lvgl_mutex;

int lvgl_roki_main(void);
#endif
