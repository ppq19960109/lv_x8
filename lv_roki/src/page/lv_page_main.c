/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"
/*********************
 *      DEFINES
 *********************/
static char *home_option[] = {"蒸", "烤", "炸", "加湿烤", "多段", "菜谱", "辅助功能", "系统设置"};
static char *home_img_option[] = {"steam.png", "bake.png", "fry.png", "humidify_bake.png", "multistage.png",
                                  "menu.png", "accessibility.png", "system_setup.png"};
static lv_obj_t *home_mode_img;
/**********************
 *  STATIC VARIABLES
 **********************/
static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("key:%s\n", key);
}
static void page_update_cb(void *arg)
{
    LV_LOG_USER("%s\n", __func__);
    lv_page_top_hide(-1);
}
static void scroll_child_select_cb(lv_obj_t *child, char select, char select_end)
{
    lv_obj_t *child2 = lv_obj_get_child(child, 0);
    if (select)
    {
        lv_obj_add_state(child, LV_STATE_CHECKED);
        lv_obj_add_state(child2, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(child, LV_STATE_CHECKED);
        lv_obj_clear_state(child2, LV_STATE_CHECKED);
    }
    lv_obj_center(child2);
    if (select_end)
    {
        lv_img_set_src(home_mode_img, getImagePath(publicImagesPath, home_img_option[lv_obj_get_index(child)]));
    }
}
static void mode_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    switch (user_data)
    {
    case 0:
        lv_100ask_page_manager_set_open_page(NULL, "page_steam_mode");
        break;
    case 1:
        break;
    case 2:
        break;
    case 4:
        lv_100ask_page_manager_set_open_page(NULL, "page_multistage");
        break;
    }
}
void lv_page_main_init(lv_obj_t *page)
{
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;

    lv_obj_t *bg = lv_img_create(page);
    lv_img_set_src(bg, getThemesPath("bg_home_bg.png"));

    home_mode_img = lv_img_create(page);
    lv_img_set_src(home_mode_img, getImagePath(publicImagesPath, home_img_option[0]));

    static lv_cycle_scroll_t lv_cycle_scroll = {0};
    lv_cycle_scroll.cb = scroll_child_select_cb;
    lv_cycle_scroll.cycle_flag = 0;
    lv_cycle_scroll.mask_flag = 1;
    lv_obj_t *cont = lv_cycle_scroll_create(page, 360, 360, LV_FLEX_FLOW_COLUMN, &lv_cycle_scroll);

    lv_obj_align(cont, LV_ALIGN_RIGHT_MID, -90, 23);
    // lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    /// 子对象
    for (int i = 0; i < sizeof(home_option) / sizeof(home_option[0]); ++i)
    {
        lv_obj_t *btn = lv_obj_create(cont);
        lv_obj_set_size(btn, LV_PCT(100), LV_PCT(33));
        // lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
        // lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor), LV_STATE_CHECKED);
        // lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
        /// 孙对象
        lv_obj_t *label = lv_label_create(btn);
        lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
        lv_obj_set_style_text_font(label, g_robam_font.FZLTHC_48.font, 0);
        lv_obj_set_style_text_font(label, g_robam_font.FZLTHC_66.font, LV_STATE_CHECKED);
        lv_label_set_text(label, home_option[i]);
        lv_obj_center(label);

        lv_obj_add_event_cb(btn, mode_event_cb, LV_EVENT_CLICKED, (void *)i);
    }
    cycle_scroll_change(cont, 1);

    // gif = lv_gif_create(lv_scr_act());
    // lv_gif_set_src(gif, "S:/oem/boot.gif");
    // lv_gif_restart(gif);
}
