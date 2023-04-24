

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *tv;
/**********************
 *  STATIC VARIABLES
 **********************/
static void event_handler(lv_event_t *e)
{
    // lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t *target = lv_event_get_target(e);

    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
}
static void page_update_cb(void *arg)
{
    lv_tabview_set_act(tv, 0, LV_ANIM_OFF);
}
static void tv_tab_img_create(lv_obj_t *tabview, const char *img_src)
{
    lv_obj_t *tv_tab1 = lv_tabview_add_tab(tabview, "");
    lv_obj_t *img = lv_img_create(tv_tab1);
    lv_obj_set_size(img, LV_PCT(100), LV_PCT(100));
    lv_img_set_src(img, img_src);
}
static void btn_array_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%ld\n", __func__, e->code, user_data);
    lv_page_back_previous_page();
}
void lv_page_screen_lcd_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_update_cb = page_update_cb;

    tv = lv_tabview_create(page, LV_DIR_TOP, 0);
    lv_obj_set_size(tv, LV_PCT(100), LV_PCT(100));
    lv_obj_add_event_cb(tv, event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *tv_tab = lv_tabview_add_tab(tv, "");
    lv_obj_t *obj = lv_img_create(tv_tab);
    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
    lv_obj_t *label = lv_label_create(obj);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, "滑动屏幕,观察测试过程显示的一些图像上是否有黑点和亮点");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    tv_tab_img_create(tv, themesImagesPath "test/image1.png");
    tv_tab_img_create(tv, themesImagesPath "test/image2.png");
    tv_tab_img_create(tv, themesImagesPath "test/image3.png");
    tv_tab_img_create(tv, themesImagesPath "test/image4.png");
    tv_tab_img_create(tv, themesImagesPath "test/image5.png");
    tv_tab_img_create(tv, themesImagesPath "test/image6.png");
    tv_tab_img_create(tv, themesImagesPath "test/image7.png");
    tv_tab_img_create(tv, themesImagesPath "test/image8.png");
    tv_tab_img_create(tv, themesImagesPath "test/image9.png");
    tv_tab_img_create(tv, themesImagesPath "test/image10.png");

    tv_tab = lv_tabview_add_tab(tv, "");
    obj = lv_img_create(tv_tab);
    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
    const char *text[] = {"成功", "失败"};
    lv_obj_t *btn_array = lv_custom_btn_array_create(obj, text, 2, btn_array_event_cb);
    lv_obj_set_size(btn_array, 140 * 2 + 50, 50);
    lv_obj_set_flex_flow(btn_array, LV_FLEX_FLOW_ROW);
    lv_obj_align(btn_array, LV_ALIGN_CENTER, 0, 0);
}
