

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("%s,key:%s\n", __func__, key);
}
static void page_update_cb(void)
{
    LV_LOG_USER("%s\n", __func__);
}
static void tabview_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *current_target = lv_event_get_current_target(e);
    unsigned short index = lv_tabview_get_tab_act(current_target);
    LV_LOG_USER("%s,index:%d\n", __func__, index);
}
void lv_page_cook_details_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;

    lv_obj_t *back_bar = lv_page_back_bar_init(page, "菜谱详情", NULL, NULL);

    lv_obj_t *left_content = lv_obj_create(page);
    lv_obj_set_size(left_content, 250, 345);
    lv_obj_align_to(left_content, back_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    lv_obj_t *tabview = lv_tabview_create(page, LV_DIR_TOP, 40);
    lv_obj_set_size(tabview, 730, 300);
    lv_obj_align_to(tabview, left_content, LV_ALIGN_OUT_RIGHT_TOP, 0, 12);
    lv_obj_add_event_cb(tabview, tabview_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *tab_btns = lv_tabview_get_tab_btns(tabview);
    lv_obj_set_width(tab_btns, 180);

    // lv_obj_set_style_pad_bottom(tab_btns, 10, 0);
    // lv_obj_set_style_border_width(tab_btns, 10, 0);
    // lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_BOTTOM, LV_PART_ITEMS);

    lv_obj_set_style_bg_opa(tab_btns, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(tab_btns, lv_color_hex(0x2C2C2C), 0);
    lv_obj_set_style_radius(tab_btns, 20, 0);

    lv_obj_set_style_bg_opa(tab_btns, LV_OPA_100, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(tab_btns, lv_color_hex(themesTextColor2), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_radius(tab_btns, 20, LV_PART_ITEMS | LV_STATE_CHECKED);

    lv_obj_set_style_text_color(tab_btns, lv_color_hex(0x000000), LV_STATE_CHECKED);
    lv_obj_set_style_text_color(tab_btns, lv_color_hex(themesTextColor2), 0);

    lv_obj_set_style_text_font(tab_btns, &lv_font_SiYuanHeiTi_Normal_26, 0);
    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "食材");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "步骤");

    lv_obj_set_style_bg_opa(tab1, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(tab2, LV_OPA_COVER, 0);

    /*Add content to the tabs*/
    lv_obj_t *label = lv_label_create(tab1);
    lv_label_set_text(label, "First tab1");

    // label = lv_label_create(tab2);
    // lv_label_set_text(label, "Third tab");
    lv_obj_t *tv = lv_tabview_create(tab2, LV_DIR_TOP, 0);
    lv_obj_set_size(tv, LV_PCT(100), LV_PCT(100));

    lv_obj_t *tv_btns = lv_tabview_get_tab_btns(tv);
    lv_obj_set_style_text_font(tv_btns, &lv_font_SiYuanHeiTi_Normal_26, 0);
    lv_obj_t *tv1 = lv_tabview_add_tab(tv, "食材1");
    lv_obj_t *tv2 = lv_tabview_add_tab(tv, "食材2");

     label = lv_label_create(tv1);
    lv_label_set_text(label, "First tv1");

     label = lv_label_create(tv2);
    lv_label_set_text(label, "First tv2");
}
