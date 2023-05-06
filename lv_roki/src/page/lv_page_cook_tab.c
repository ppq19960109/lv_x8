

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"

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
}
static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("lv_page_set,key:%s\n", key);
    // lv_wifi_property_change_cb(key, value);
    lv_update_property_change_cb(key, value);
}
static void page_update_cb(void *arg)
{
    unsigned short index = lv_tabview_get_tab_act(tabview);
    LV_LOG_USER("lv_page_set,%s index:%d\n", __func__, index);
    lv_page_chart_completed_cb(arg);
}
static void page_close_cb(void *arg)
{
    lv_page_chart_destroyed_cb(arg);
}
static void tabview_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *current_target = lv_event_get_current_target(e);
    unsigned short index = lv_tabview_get_tab_act(current_target);
    LV_LOG_USER("%s,index:%d\n", __func__, index);
}
static void back_event_cb(lv_event_t *e)
{
    // LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_page_back_previous_page();
}
void lv_page_cook_tab_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;
    manager_page->page_close_cb = page_close_cb;

    tabview = lv_tabview_create(page, LV_DIR_TOP, 0);
    lv_obj_set_size(tabview, LV_PCT(100), LV_PCT(100));
    lv_obj_add_event_cb(tabview, tabview_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "烹饪状态");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "烹饪曲线");

    /*Add content to the tabs*/
    lv_page_steaming_create(tab1);
    lv_page_chart_create(tab2);

    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLL_ELASTIC);
    // lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
    lv_page_back_bar_init(page, "返回", NULL, back_event_cb);
}
