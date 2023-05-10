

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
            LV_LOG_USER("%s,mode_roller select_end\n", __func__);
            lv_custom_mode_change(&steamoven_roller, (int)child->user_data);
        }
        else if (steamoven_roller.steamoven_mode != NULL && steamoven_roller.steamoven_mode->maxlowertemp > 0)
        {
            int temp = 0, lowertemp = 0;
            if (lv_obj_get_parent(child) == steamoven_roller.temp_roller)
            {
                temp = (int)lv_cycle_scroll_get_selected(steamoven_roller.temp_roller)->user_data;
                lowertemp = (int)lv_cycle_scroll_get_selected(steamoven_roller.lower_temp_roller)->user_data;

        lv_obj_clean(steamoven_roller.lower_temp_roller);
        for (int i = 100; i <= 200; ++i)
        {
            child = mode_roller_scroll_child_create(steamoven_roller.lower_temp_roller, NULL, i);
            child->user_data = (void *)i;
        }
                if (temp > lowertemp + 20)
                {
                    cycle_scroll_to_userdata(steamoven_roller.lower_temp_roller, temp - 20);
                }
                else if (lowertemp > temp + 20)
                {
                    cycle_scroll_to_userdata(steamoven_roller.lower_temp_roller, temp + 20);
                }
                LV_LOG_USER("%s,temp_roller select_end:%d,%d\n", __func__, temp, lowertemp);
            }
            else if (lv_obj_get_parent(child) == steamoven_roller.lower_temp_roller)
            {
                temp = (int)lv_cycle_scroll_get_selected(steamoven_roller.temp_roller)->user_data;
                lowertemp = (int)lv_cycle_scroll_get_selected(steamoven_roller.lower_temp_roller)->user_data;
                if (lowertemp > temp + 20)
                {
                    // cycle_scroll_to_userdata(steamoven_roller.temp_roller, lowertemp - 20);
                }
                else if (temp > lowertemp + 20)
                {
                    // cycle_scroll_to_userdata(steamoven_roller.temp_roller, lowertemp + 20);
                }
                LV_LOG_USER("%s,lower_temp_roller select_end:%d,%d\n", __func__, temp, lowertemp);
            }
        }
    }
}
static void bottom_bar_event_cb(lv_event_t *e)
{
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    if (user_data == 0)
    {
        cycle_scroll_change(steamoven_roller.temp_roller,0);
    }
    else
    {
        cook_start(0);
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

    steamoven_roller.cooktype = COOK_TYPE_BAKE;
    lv_obj_t *cont_row = lv_custom_mode_roller_create(page, &steamoven_roller, &lv_cycle_scroll);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 106);
}
