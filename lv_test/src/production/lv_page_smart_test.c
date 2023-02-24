

/*********************
 *      INCLUDES
 *********************/
#include "lv_test_main.h"
/*********************
 *      DEFINES
 *********************/
static lv_obj_t *child[6];
/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *item_create(lv_obj_t *parent, const char *text)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, LV_PCT(100), 50);

    lv_obj_t *left_obj = lv_obj_create(obj);
    lv_obj_set_size(left_obj, 300, LV_PCT(100));
    lv_obj_t *label = lv_label_create(left_obj);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    lv_obj_t *right_obj = lv_obj_create(obj);
    lv_obj_set_size(right_obj, 700, LV_PCT(100));
    lv_obj_align(right_obj, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_color(right_obj, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_set_style_bg_opa(right_obj, LV_OPA_100, 0);
    label = lv_label_create(right_obj);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, "检测中..");
    lv_obj_center(label);
    return obj;
}
static void property_change_cb(const char *key, void *value)
{
}
static void page_update_cb(void *arg)
{
}
void lv_page_smart_test_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;
    lv_page_back_bar_init(page, "智能模块检测", NULL, NULL);

    // lv_obj_t *list = lv_list_create(page);
    lv_obj_t *list = lv_obj_create(page);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(list, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_size(list, 1100, 340);
    lv_obj_align(list, LV_ALIGN_BOTTOM_MID, 0, 0);

    child[0] = item_create(list, "通讯及版本检测:");
    lv_obj_set_height(child[0], 80);

    child[1] = item_create(list, "wifi信号检测:");
    child[2] = item_create(list, "wifi连接:");
    child[3] = item_create(list, "四元组检测:");
    child[4] = item_create(list, "产测序号:");
    child[5] = item_create(list, "恢复出厂设置:");
}
