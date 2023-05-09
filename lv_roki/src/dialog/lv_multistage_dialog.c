

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/
static lv_cycle_scroll_t lv_cycle_scroll;
static steamoven_roller_t steamoven_roller;
/**********************
 *  STATIC VARIABLES
 **********************/

static void cook_start(const int reserve_time)
{
    steamoven_t steamoven = {0};
    lv_custom_get_roller_attr(&steamoven_roller, &steamoven);
    steamoven.orderTime = reserve_time;
    LV_LOG_USER("%s,mode:%d,temp:%d,lowertemp:%d,time:%d,vapour:%d\n", __func__, steamoven.attr[0].mode, steamoven.attr[0].temp, steamoven.attr[0].lowertemp, steamoven.attr[0].time, steamoven.attr[0].vapour);
}

static void scroll_child_select_cb(lv_obj_t *child, char select, char select_end)
{
    lv_obj_t *child2 = lv_obj_get_child(child, 0);
    if (select)
    {
        lv_obj_add_state(child, LV_STATE_CHECKED);
        lv_obj_add_state(child2, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(child, LV_STATE_CHECKED);
        lv_obj_clear_state(child2, LV_STATE_CHECKED);
    }
    lv_obj_center(child2);
    if (select_end)
    {
        if (lv_obj_get_parent(child) == steamoven_roller.mode_roller)
        {
            lv_custom_mode_change(&steamoven_roller, (int)child->user_data);
        }
    }
}
static void bottom_bar_event_cb(lv_event_t *e)
{
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    if (user_data == 0)
    {
    }
    else
    {
        cook_start(0);
    }
}
void lv_multistage_dialog(multistage_para_t *multistage_para)
{
    lv_obj_t *layer = get_manual_layer();
    layer->user_data = (void *)MANUAL_INDEX_MULTISTAGE;

    lv_page_back_bar_init(layer, "返回", NULL, NULL);
    lv_page_bottom_bar_init(layer, "", "确认", bottom_bar_event_cb);

    lv_cycle_scroll.cb = scroll_child_select_cb;
    lv_cycle_scroll.cycle_flag = 1;
    lv_cycle_scroll.mask_flag = 1;

    steamoven_roller.cooktype = COOK_TYPE_MULTISTAGE;
    lv_obj_t *cont_row = lv_custom_mode_roller_create(layer, &steamoven_roller, &lv_cycle_scroll);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 106);
}
