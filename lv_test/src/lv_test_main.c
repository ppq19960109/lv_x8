

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_30);
/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static void obj_event_cb(lv_event_t * e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t * target = lv_event_get_current_target(e); // lv_event_get_target(e);
    lv_obj_set_style_bg_color(target, lv_color_hex(0xff0000), LV_STATE_PRESSED);

    lv_obj_t * label = lv_event_get_user_data(e);
    lv_event_send(label, LV_EVENT_CLICKED, 0);
}

void lv_test_widgets(void)
{
    LV_LOG_USER("lv_test_widgets...");
    // lv_100ask_pinyin_ime_simple_test();
    lv_100ask_page_manager_simple_test();
}

