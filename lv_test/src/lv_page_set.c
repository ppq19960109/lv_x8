

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *tabview;
/**********************
 *  STATIC VARIABLES
 **********************/
void lv_page_set_tabview_set(int index)
{
    lv_tabview_set_act(tabview, index, LV_ANIM_ON);
    if (index == 1)
        lv_page_wifi_visible(1);
}
static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("lv_page_set,key:%s\n", key);
    lv_wifi_property_change_cb(key, value);
}
static void page_update_cb(void)
{
    unsigned short index = lv_tabview_get_tab_act(tabview);
    LV_LOG_USER("lv_page_set,%s index:%d\n", __func__, index);
    // lv_tabview_set_act(tabview, 2, LV_ANIM_ON);
    scan_count = 0;
    if (index == 0)
        lv_page_local_set_visible(1);
    else if (index == 1)
        lv_page_wifi_visible(1);
}
static void tabview_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *current_target = lv_event_get_current_target(e);
    unsigned short index = lv_tabview_get_tab_act(current_target);
    LV_LOG_USER("%s,index:%d\n", __func__, index);
    if (index == 0)
        lv_page_local_set_visible(1);
    else if (index == 2)
        lv_page_about_visible(1);
    else if (index == 3)
        lv_page_update_visible(1);

    if (index == 1)
        lv_page_wifi_visible(1);
    else
        lv_page_wifi_visible(0);
}
static void back_bar_event_cb(lv_event_t *e)
{
    lv_page_wifi_visible(0);
    lv_page_back_previous_page();
}

void lv_page_set_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;

    lv_obj_t *back_bar = lv_page_back_bar_init(page, "设置", NULL, back_bar_event_cb);

    tabview = lv_tabview_create(page, LV_DIR_LEFT, 200);
    lv_obj_set_size(tabview, LV_PCT(100), 340);
    lv_obj_align_to(tabview, back_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_event_cb(tabview, tabview_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *tab_btns = lv_tabview_get_tab_btns(tabview);
    lv_obj_set_style_bg_img_src(tab_btns, themesImagesPath "menulist_item_background.png", LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(tab_btns, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_border_color(tab_btns, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_border_width(tab_btns, 2, 0);

    lv_obj_set_style_text_font(tab_btns, &lv_font_SiYuanHeiTi_Normal_24, 0);
    lv_obj_set_style_text_font(tab_btns, &lv_font_SiYuanHeiTi_Normal_30, LV_STATE_CHECKED);
    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "本机设置");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "网络连接");
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "关于本机");
    lv_obj_t *tab4 = lv_tabview_add_tab(tabview, "系统更新");
    lv_obj_t *tab5 = lv_tabview_add_tab(tabview, "恢复出厂设置");

    // lv_obj_set_style_bg_opa(tab1, LV_OPA_COVER, 0);
    // lv_obj_set_style_bg_opa(tab2, LV_OPA_COVER, 0);
    // lv_obj_set_style_bg_opa(tab3, LV_OPA_COVER, 0);
    // lv_obj_set_style_bg_opa(tab4, LV_OPA_COVER, 0);
    // lv_obj_set_style_bg_opa(tab5, LV_OPA_COVER, 0);

    /*Add content to the tabs*/
    lv_page_local_set_create(tab1);
    lv_page_wifi_create(tab2);
    lv_page_about_create(tab3);
    lv_page_update_create(tab4);
    lv_page_reset_create(tab5);

    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
}
