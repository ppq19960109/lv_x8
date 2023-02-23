

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static lv_timer_t *lv_timer;
static int bg_index = 0;
/**********************
 *  STATIC VARIABLES
 **********************/
static void event_handler(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);

    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    long user_data = (long)lv_event_get_user_data(e);
    if (user_data == 0)
        lv_page_back_previous_page();
}
static void page_update_cb(void *arg)
{
    bg_index = 0;
    lv_timer_resume(lv_timer);
}
static void page_close_cb(void *arg)
{
    lv_timer_pause(lv_timer);
}
static void lv_timer_cb(lv_timer_t *timer)
{
    lv_obj_t *obj_background = (lv_obj_t *)timer->user_data;
    ++bg_index;
    
    if (bg_index % 3 == 0)
    {
        lv_obj_set_style_bg_color(obj_background, lv_color_hex(0xff0000), 0);
    }
    else if (bg_index % 3 == 1)
    {
        lv_obj_set_style_bg_color(obj_background, lv_color_hex(0x00ff00), 0);
    }
    else if (bg_index % 3 == 2)
    {
        lv_obj_set_style_bg_color(obj_background, lv_color_hex(0x0000ff), 0);
    }
}
void lv_page_screen_aging_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_update_cb = page_update_cb;
    manager_page->page_close_cb = page_close_cb;

    lv_obj_t *obj_background = lv_obj_create(page);
    lv_obj_set_size(obj_background, 1000, 300);
    lv_obj_set_style_bg_color(obj_background, lv_color_hex(0xff0000), 0);
    lv_obj_set_style_bg_opa(obj_background, LV_OPA_100, 0);
    lv_obj_align(obj_background, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *btn = lv_btn_create(page);
    lv_obj_set_size(btn, 140, 50);
    lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn, 25, 0);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -10, -5);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void *)0);
    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
    lv_label_set_text(label, "退出");
    lv_obj_center(label);

    lv_timer = lv_timer_create(lv_timer_cb, 2000, obj_background);
}
