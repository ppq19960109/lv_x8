

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static void event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:

        break;
    case 3:

        break;
    case 4:

        break;
    case 5:
        lv_page_back_previous_page();
        break;
    }
}
static void page_update_cb(void *arg)
{
}

void lv_page_screen_click_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_update_cb = page_update_cb;

    static lv_style_t btn_style;
    lv_style_init(&btn_style);
    lv_style_set_bg_color(&btn_style, lv_color_hex(themesTextColor));
    lv_style_set_bg_opa(&btn_style, LV_OPA_100);
    lv_style_set_radius(&btn_style, LV_RADIUS_CIRCLE);

    lv_obj_t *btn = lv_btn_create(page);
    lv_obj_set_size(btn, 100, 100);
    lv_obj_add_style(btn, &btn_style, 0);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, (void *)0);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 20, 20);

    btn = lv_btn_create(page);
    lv_obj_set_size(btn, 100, 100);
    lv_obj_add_style(btn, &btn_style, 0);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, (void *)1);
    lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, -20, 20);

    btn = lv_btn_create(page);
    lv_obj_set_size(btn, 100, 100);
    lv_obj_add_style(btn, &btn_style, 0);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, (void *)2);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    btn = lv_btn_create(page);
    lv_obj_set_size(btn, 100, 100);
    lv_obj_add_style(btn, &btn_style, 0);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, (void *)3);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 20, -20);

    btn = lv_btn_create(page);
    lv_obj_set_size(btn, 100, 100);
    lv_obj_add_style(btn, &btn_style, 0);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, (void *)4);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -20, -20);

    btn = lv_btn_create(page);
    lv_obj_set_size(btn, 140, 50);
    lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn, 25, 0);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, (void *)5);
    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
    lv_label_set_text(label, "退出");
    lv_obj_center(label);
}
