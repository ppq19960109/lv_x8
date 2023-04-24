

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *label_ingredient;
static lv_obj_t *tabview;
static lv_obj_t *tv_steps;
static lv_obj_t *recipe_img;
static recipe_t *cur_recipe;

static lv_obj_t *page_indicator = NULL;
/**********************
 *  STATIC VARIABLES
 **********************/
static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("%s,key:%s\n", __func__, key);
}
static void page_update_cb(void *user_data)
{
    lv_tabview_set_act(tabview, 0, LV_ANIM_OFF);
    lv_100ask_page_manager_page_t *page = (lv_100ask_page_manager_page_t *)user_data;
    cur_recipe = (recipe_t *)page->user_data;
    LV_LOG_USER("%s\n", __func__);
    lv_label_set_text(label_ingredient, cur_recipe->ingredients);

    lv_obj_t *cont = lv_tabview_get_content(tv_steps);
    lv_obj_clean(cont);
    char url[80];
    for (int j = 0; j < cur_recipe->details_count; ++j)
    {
        lv_obj_t *tab = lv_tabview_add_tab(tv_steps, "1");

        lv_obj_t *label_step = lv_label_create(tab);
        lv_obj_set_style_text_font(label_step, &lv_font_SiYuanHeiTi_Normal_26, 0);
        lv_obj_set_style_text_color(label_step, lv_color_hex(0xffffff), 0);

        sprintf(url, "%s%d%s%d", "步骤", j + 1, "/", cur_recipe->details_count);
        lv_label_set_text(label_step, url);

        lv_obj_t *label = lv_label_create(tab);
        lv_obj_set_width(label, LV_PCT(100));
        lv_obj_set_style_text_line_space(label, 20, 0);
        lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
        lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
        lv_label_set_text(label, cur_recipe->details[j]);
        lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 40);

        lv_obj_set_style_pad_all(label, 12, 0);

        static lv_style_t scrollbar;
        lv_style_init(&scrollbar);
        lv_style_set_bg_color(&scrollbar, lv_color_hex(themesTextColor));
        lv_style_set_radius(&scrollbar, LV_RADIUS_CIRCLE);
        lv_style_set_pad_all(&scrollbar, 0);
        lv_style_set_width(&scrollbar, 5);
        lv_style_set_bg_opa(&scrollbar, LV_OPA_80);
        lv_obj_add_style(tab, &scrollbar, LV_PART_SCROLLBAR);
    }
    if (cur_recipe->cookPos == 0)
        sprintf(url, "%s%s%s", recipesImagesPath, cur_recipe->imgUrl, "/0.png");
    else
        sprintf(url, "%s%s%s", recipesImagesPath, cur_recipe->imgUrl, ".png");
    LV_LOG_USER("%s,img url:%s\n", __func__, url);
    lv_img_set_src(recipe_img, url);

    if (page_indicator != NULL)
        lv_obj_del(page_indicator);
    page_indicator = lv_page_indicator_create((lv_obj_t *)page, cur_recipe->details_count);
    lv_obj_align(page_indicator, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_page_indicator_check(page_indicator, 0);
    lv_obj_add_flag(page_indicator, LV_OBJ_FLAG_HIDDEN);
}
static void tabview_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *current_target = lv_event_get_current_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    unsigned short index = lv_tabview_get_tab_act(current_target);
    LV_LOG_USER("%s,index:%d user_data:%ld\n", __func__, index, user_data);
    char url[90];
    if (user_data == 0 && index == 0)
    {
        sprintf(url, "%s%s%s", recipesImagesPath, cur_recipe->imgUrl, "/0.png");
        lv_obj_add_flag(page_indicator, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        index = lv_tabview_get_tab_act(tv_steps);
        sprintf(url, "%s%s%s%d%s", recipesImagesPath, cur_recipe->imgUrl, "/", index + 1, ".png");
        lv_obj_clear_flag(page_indicator, LV_OBJ_FLAG_HIDDEN);
        lv_page_indicator_check(page_indicator, index);
    }
    if (cur_recipe->cookPos == 0)
        lv_img_set_src(recipe_img, url);
}
static void dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
        recipe_cook_start(cur_recipe, 0);
    }
    break;
    }
    clean_manual_layer();
}
static void reserve_dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
        int orderTime = lv_get_reserve_dialog_time();
        LV_LOG_USER("%s,orderTime:%d\n", __func__, orderTime);
        recipe_cook_start(cur_recipe, orderTime);
    }
    break;
    }
    clean_manual_layer();
}
static void btn_array_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%ld\n", __func__, e->code, user_data);
    if (user_data == 0)
    {
        lv_manual_cook_dialog("请将食物放入左腔,水箱中加满水", dialog_event_cb);
    }
    else
    {
        lv_manual_reserve_dialog("左腔将在", "后启动", "预约", 12, reserve_dialog_event_cb);
    }
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
    recipe_img = lv_img_create(left_content);
    lv_obj_align(recipe_img, LV_ALIGN_CENTER, 0, 0);
    // lv_img_set_src(recipe_img, themesImagesPath);

    tabview = lv_tabview_create(page, LV_DIR_TOP, 40);
    lv_obj_set_size(tabview, 730, 300);
    lv_obj_align_to(tabview, left_content, LV_ALIGN_OUT_RIGHT_TOP, 0, 12);
    lv_obj_add_event_cb(tabview, tabview_event_cb, LV_EVENT_VALUE_CHANGED, (void *)0);

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

    lv_obj_t *tab_cont = lv_tabview_get_content(tabview);
    lv_obj_set_style_border_width(tab_cont, 10, 0);
    lv_obj_set_style_border_opa(tab_cont, LV_OPA_0, 0);
    lv_obj_set_style_border_color(tab_cont, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_border_side(tab_cont, LV_BORDER_SIDE_TOP, 0);
    // lv_obj_set_style_outline_width(tab1, 50, 0);
    // lv_obj_set_style_outline_color(tab1, lv_color_hex(themesTextColor), 0);

    lv_obj_set_style_bg_opa(tab1, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tab1, lv_color_hex(0x2C2C2C), 0);
    lv_obj_set_style_bg_opa(tab2, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tab2, lv_color_hex(0x2C2C2C), 0);
    lv_obj_set_style_pad_all(tab1, 20, 0);
    lv_obj_set_style_pad_all(tab2, 20, 0);
    /*Add content to the tabs*/
    label_ingredient = lv_label_create(tab1);
    lv_obj_set_width(label_ingredient, LV_PCT(100));
    lv_obj_set_style_text_line_space(label_ingredient, 20, 0);
    lv_obj_set_style_text_font(label_ingredient, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label_ingredient, lv_color_hex(0xffffff), 0);
    lv_label_set_long_mode(label_ingredient, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label_ingredient, "First tab1");

    tv_steps = lv_tabview_create(tab2, LV_DIR_TOP, 0);
    lv_obj_set_size(tv_steps, LV_PCT(100), LV_PCT(100));
    lv_obj_add_event_cb(tv_steps, tabview_event_cb, LV_EVENT_VALUE_CHANGED, (void *)1);

    lv_obj_t *tv_btns = lv_tabview_get_tab_btns(tv_steps);
    lv_obj_set_style_text_font(tv_btns, &lv_font_SiYuanHeiTi_Normal_26, 0);
    lv_obj_t *tv_tab1 = lv_tabview_add_tab(tv_steps, "食材1");
    lv_obj_t *tv_tab2 = lv_tabview_add_tab(tv_steps, "食材2");
    lv_obj_t *label = lv_label_create(tv_tab1);
    lv_label_set_text(label, "First tv1");
    label = lv_label_create(tv_tab2);
    lv_label_set_text(label, "First tv2");
    //------------------------------
    const char *text[] = {"启动", "预约"};
    lv_obj_t *btn_array = lv_custom_btn_array_create(page, text, 2, btn_array_event_cb);
    lv_obj_align_to(btn_array, back_bar, LV_ALIGN_OUT_BOTTOM_RIGHT, -25, 90);
}
