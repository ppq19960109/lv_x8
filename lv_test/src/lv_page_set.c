

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

void lv_page_set_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t *back_bar = lv_page_back_bar_init(page, "设置", NULL, NULL);

    lv_obj_t *tabview = lv_tabview_create(page, LV_DIR_LEFT, 180);
    lv_obj_set_size(tabview, LV_PCT(100), 340);
    lv_obj_align_to(tabview, back_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    lv_obj_t *tab_btns = lv_tabview_get_tab_btns(tabview);
    lv_obj_set_style_bg_img_src(tab_btns, themesImagesPath "menulist_item_background.png", LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(tab_btns, lv_color_hex(0xffffff), 0);

    lv_obj_set_style_text_font(tab_btns, &lv_font_SiYuanHeiTi_Normal_24, 0);
    lv_obj_set_style_text_font(tab_btns, &lv_font_SiYuanHeiTi_Normal_30, LV_STATE_CHECKED);
    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "本机设置");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "网络连接");
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "关于本机");
    lv_obj_t *tab4 = lv_tabview_add_tab(tabview, "系统更新");
    lv_obj_t *tab5 = lv_tabview_add_tab(tabview, "恢复出厂");

    lv_obj_set_style_bg_opa(tab1, LV_OPA_COVER, 0);
    // lv_obj_set_style_bg_opa(tab2, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(tab3, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(tab4, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(tab5, LV_OPA_COVER, 0);

    /*Add content to the tabs*/
    lv_obj_t *label = lv_label_create(tab1);
    lv_label_set_text(label, "First tab");

    lv_page_wifi_create(tab2);

    label = lv_label_create(tab3);
    lv_label_set_text(label, "Third tab");

    label = lv_label_create(tab4);
    lv_label_set_text(label, "Forth tab");

    label = lv_label_create(tab5);
    lv_label_set_text(label, "Fifth tab");

    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
}
