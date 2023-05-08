

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
    // int mode_index = (int)lv_cycle_scroll_get_selected(roller_scroll[0])->user_data;
    // int temp = (int)lv_cycle_scroll_get_selected(roller_scroll[1])->user_data;
    // int time = (int)lv_cycle_scroll_get_selected(roller_scroll[2])->user_data;
    // int vapour;
    // if (lv_obj_has_flag(roller_scroll[3], LV_OBJ_FLAG_HIDDEN))
    //     vapour = 0;
    // else
    //     vapour = (int)lv_cycle_scroll_get_selected(roller_scroll[2])->user_data + 1;

    // LV_LOG_USER("%s,roller index:%d,%d,%d,%d\n", __func__, mode_index, temp, time, vapour);
    // steamoven_mode_t *steamoven_mode = get_steamoven_mode(mode_index);
    // steamoven_t steamoven = {0};
    // steamoven.attr[0].mode = steamoven_mode->mode;
    // steamoven.attr[0].temp = temp_index + steamoven_mode->mintemp;
    // steamoven.attr[0].time = time_index + 1;
    // steamoven.orderTime = reserve_time;
    // set_cook_toServer(&steamoven);
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
        if (lv_obj_get_parent(child)->user_data == NULL)
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
        lv_100ask_page_manager_set_open_page(NULL, "page_cook_tab");
    }
}
void lv_page_cook_bake_init(lv_obj_t *page)
{
    lv_page_back_bar_init(page, "返回", NULL, NULL);
    lv_page_bottom_bar_init(page, "预约", "开始烹饪", bottom_bar_event_cb);

    lv_cycle_scroll.cb = scroll_child_select_cb;
    lv_cycle_scroll.cycle_flag = 1;
    lv_cycle_scroll.mask_flag = 1;

    steamoven_roller.cooktype = COOK_TYPE_STEAMED;
    lv_obj_t *cont_row = lv_custom_mode_roller_create(page, &steamoven_roller, &lv_cycle_scroll);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 106);
}
