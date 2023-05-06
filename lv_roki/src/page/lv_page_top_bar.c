

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *bar, *label_time, *img_wifi;
/**********************
 *  STATIC VARIABLES
 **********************/

static void event_cb(lv_event_t *e)
{
    static int long_press_repeat_count = 0;
    int user_data = (int)lv_event_get_user_data(e);
    // LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    // lv_obj_t * target      = lv_event_get_target(e);
    if (user_data == 0)
    {
        if (e->code == LV_EVENT_LONG_PRESSED_REPEAT)
        {
            if (++long_press_repeat_count == 20)
            {
                LV_LOG_USER("%s,long_press_repeat_count 2s\n", __func__);
            }
        }
        else if (e->code == LV_EVENT_RELEASED)
        {
            long_press_repeat_count = 0;
        }
    }
    else
    {
    }
}
void lv_page_top_update_time(long timestamp)
{
    time_t t;
    time(&t);
    struct tm *local_tm = localtime(&t);
    LV_LOG_USER("year:%d mon:%d day:%d", local_tm->tm_year, local_tm->tm_mon, local_tm->tm_mday);
    LV_LOG_USER("hour:%d min:%d sec:%d", local_tm->tm_hour, local_tm->tm_min, local_tm->tm_sec);

    char buf[6];
    sprintf(buf, "%02d:%02d", local_tm->tm_hour, local_tm->tm_min);
    lv_label_set_text(label_time, buf);
}
void lv_page_top_update_wifi(int status)
{
    switch (status)
    {
    case 0:
        lv_obj_add_flag(img_wifi, LV_OBJ_FLAG_HIDDEN);
        break;
    case 1:
        lv_img_set_src(img_wifi, getThemesPath("bg_wifi_max.png"));
        lv_obj_clear_flag(img_wifi, LV_OBJ_FLAG_HIDDEN);
        break;
    case 2:
        break;
    case 3:
        break;
    }
}
void lv_page_top_hide(int index)
{
    if (index < 0)
    {
        for (int i = 0; i < lv_obj_get_child_cnt(bar); ++i)
            lv_obj_clear_flag(lv_obj_get_child(bar, i), LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(lv_obj_get_child(bar, index), LV_OBJ_FLAG_HIDDEN);
    }
}
static void lv_page_top_bar_create(lv_obj_t *parent)
{
    lv_obj_t *time_wifi = lv_obj_create(parent);
    lv_obj_set_size(time_wifi, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(time_wifi, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_ext_click_area(time_wifi, 5);
    label_time = lv_label_create(time_wifi);
    lv_obj_set_style_text_font(label_time, g_robam_font.FZLTHC_30.font, 0);
    lv_obj_set_style_text_color(label_time, getThemesFontColor1(), 0);
    lv_label_set_text(label_time, "12:22");
    img_wifi = lv_img_create(time_wifi);
    lv_img_set_src(img_wifi, getThemesPath("bg_wifi_max.png"));
    lv_obj_align_to(img_wifi, label_time, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_align(time_wifi, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_add_event_cb(time_wifi, event_cb, LV_EVENT_ALL, (void *)0);

    lv_obj_t *water_tank = lv_obj_create(parent);
    lv_obj_set_size(water_tank, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_t *img_water = lv_img_create(water_tank);
    lv_img_set_src(img_water, getThemesPath("bg_water_box.png"));
    lv_obj_t *label_water = lv_label_create(water_tank);
    lv_obj_set_style_text_font(label_water, g_robam_font.FZLTHC_30.font, 0);
    lv_obj_set_style_text_color(label_water, getThemesFontColor1(), 0);
    lv_label_set_text(label_water, "水箱");
    lv_obj_align_to(label_water, img_water, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_align(water_tank, LV_ALIGN_RIGHT_MID, -24, 0);

    lv_obj_add_event_cb(water_tank, event_cb, LV_EVENT_CLICKED, (void *)1);

    lv_page_top_update_time(0);
}
lv_obj_t *lv_page_top_bar_init(lv_obj_t *parent)
{
    LV_LOG_USER("%s...", __func__);
    bar = lv_obj_create(parent);
    lv_obj_set_size(bar, LV_PCT(55), 80);
    lv_obj_align(bar, LV_ALIGN_TOP_RIGHT, 0, 0);

    lv_page_top_bar_create(bar);
    return bar;
}
void lv_page_top_bar_reinit(void)
{
    if (bar == NULL)
        return;
    lv_obj_clean(bar);
    lv_page_top_bar_create(bar);
    lv_page_top_update_time(0);
    lv_page_top_update_wifi(1);
}
