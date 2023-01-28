

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *hood_speed_obj[3];
static lv_obj_t *stir_fried_rotate;

static lv_obj_t *smart_smoke_rotate;
/**********************
 *  STATIC VARIABLES
 **********************/
static void page_update_SmartSmokeSwitch(void)
{
    int value = get_attr_value_int("SmartSmokeSwitch");
    lv_obj_t *parent = lv_obj_get_parent(smart_smoke_rotate);
    if (value)
    {
        lv_obj_add_state(parent, LV_STATE_CHECKED);
        lv_obj_clear_flag(smart_smoke_rotate, LV_OBJ_FLAG_HIDDEN);
        lv_rotate_anim(smart_smoke_rotate, 1);
    }
    else
    {
        lv_obj_clear_state(parent, LV_STATE_CHECKED);
        lv_obj_add_flag(smart_smoke_rotate, LV_OBJ_FLAG_HIDDEN);
        lv_rotate_anim(smart_smoke_rotate, 0);
    }
}
static void page_update_HoodSpeed(void)
{
    int speed = get_attr_value_int("HoodSpeed");
    for (int i = 0; i < 3; ++i)
    {
        if (speed == 0 || speed != i + 1)
        {
            if (lv_obj_has_state(hood_speed_obj[i], LV_STATE_CHECKED))
            {
                lv_obj_clear_state(hood_speed_obj[i], LV_STATE_CHECKED);
                lv_obj_clear_state(lv_obj_get_child(hood_speed_obj[i], 0), LV_STATE_CHECKED);
            }
        }
        else
        {
            lv_obj_add_state(hood_speed_obj[i], LV_STATE_CHECKED);
            lv_obj_add_state(lv_obj_get_child(hood_speed_obj[i], 0), LV_STATE_CHECKED);
        }
    }

    lv_obj_t *parent = lv_obj_get_parent(stir_fried_rotate);
    if (speed == 4)
    {
        lv_obj_add_state(parent, LV_STATE_CHECKED);
        lv_obj_clear_flag(stir_fried_rotate, LV_OBJ_FLAG_HIDDEN);
        lv_rotate_anim(stir_fried_rotate, 1);
    }
    else
    {
        lv_obj_clear_state(parent, LV_STATE_CHECKED);
        lv_obj_add_flag(stir_fried_rotate, LV_OBJ_FLAG_HIDDEN);
        lv_rotate_anim(stir_fried_rotate, 0);
    }
}
static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("key:%s\n", key);
    if (strcmp("SmartSmokeSwitch", key) == 0)
    {
        page_update_SmartSmokeSwitch();
    }
    else if (strcmp("HoodSpeed", key) == 0)
    {
        page_update_HoodSpeed();
    }
}
static void page_update_cb(void)
{
    page_update_SmartSmokeSwitch();
    page_update_HoodSpeed();
}
static void event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_target(e); // lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);

    switch (user_data)
    {
    case 0:
    {
        int value = get_attr_value_int("SmartSmokeSwitch");
        set_num_toServer("SmartSmokeSwitch", !value);
    }
    break;
    case 1:
        break;
    case 2:
        break;
    }
}
static void hood_speed_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    if (lv_obj_has_state(target, LV_STATE_CHECKED))
        set_num_toServer("HoodSpeed", 0);
    else
        set_num_toServer("HoodSpeed", user_data);
}
static lv_obj_t *lv_hood_item_create(lv_obj_t *parent, const void *img_src, const void *name, lv_obj_t **img_out)
{
    lv_obj_t *item = lv_custom_image_button_create(parent, img_src, 200, LV_PCT(100), 0, 18);
    lv_obj_set_style_bg_color(item, lv_color_hex(0x000), 0);
    lv_obj_set_style_bg_opa(item, LV_OPA_100, 0);
    lv_obj_set_style_border_color(item, lv_color_hex(0x858585), 0);
    lv_obj_set_style_border_width(item, 1, 0);

    lv_obj_t *label_title = lv_label_create(item);
    lv_obj_set_style_text_font(label_title, &lv_font_SiYuanHeiTi_Normal_34, 0);
    lv_obj_set_style_text_color(label_title, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_title, name);
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 26);

    *img_out = lv_img_create(item);
    lv_obj_t *img = *img_out;
    lv_img_set_src(img, themesImagesPath "icon_runing.png");
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 81);
    lv_obj_update_layout(img);
    lv_img_set_pivot(img, lv_obj_get_width(img) / 2, lv_obj_get_height(img) / 2);

    return item;
}

static lv_obj_t *lv_text_btn_create(lv_obj_t *parent, const char *text, const void *user_data)
{
    lv_obj_t *btn1 = lv_btn_create(parent);
    lv_obj_set_size(btn1, 90, 45);
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0x191919), 0);
    lv_obj_set_style_bg_color(btn1, lv_color_hex(themesTextColor), LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(btn1, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn1, 45, 0);
    lv_obj_add_event_cb(btn1, hood_speed_event_cb, LV_EVENT_CLICKED, user_data);

    lv_obj_t *label = lv_label_create(btn1);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x7C7C7C), 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_CHECKED);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return btn1;
}

void lv_page_hood_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;

    lv_obj_t *back_bar = lv_page_back_bar_init(page, "烟机灶具", NULL, NULL);
    lv_page_top_bar_init(page, 0);

    lv_obj_t *cont_row = lv_obj_create(page);
    lv_obj_set_size(cont_row, 200 * 5 + 20 * 4, 260);
    lv_obj_align_to(cont_row, back_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *smart_smoke =
        lv_hood_item_create(cont_row, themesImagesPath "smart_smoke_background.png", "智能排烟", &smart_smoke_rotate);
    lv_obj_add_event_cb(smart_smoke, event_cb, LV_EVENT_CLICKED, 0);

    lv_obj_t *stir_fried =
        lv_hood_item_create(cont_row, themesImagesPath "stir_fried_background.png", "爆炒", &stir_fried_rotate);
    lv_obj_add_event_cb(stir_fried, hood_speed_event_cb, LV_EVENT_CLICKED, 4);

    lv_obj_t *hood_speed = lv_obj_create(cont_row);
    lv_obj_set_size(hood_speed, 200, LV_PCT(100));
    lv_obj_set_style_bg_color(hood_speed, lv_color_hex(0x000), 0);
    lv_obj_set_style_bg_opa(hood_speed, LV_OPA_100, 0);
    lv_obj_set_style_border_color(hood_speed, lv_color_hex(0x858585), 0);
    lv_obj_set_style_border_width(hood_speed, 1, 0);
    lv_obj_t *label_title = lv_label_create(hood_speed);
    lv_obj_set_style_text_font(label_title, &lv_font_SiYuanHeiTi_Normal_34, 0);
    lv_label_set_text(label_title, "风速");
    lv_obj_set_style_text_color(label_title, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 26);

    lv_obj_t *cont_col = lv_obj_create(hood_speed);
    lv_obj_set_size(cont_col, 90, 45 * 3 + 15 * 2);
    lv_obj_align(cont_col, LV_ALIGN_TOP_MID, 0, 72);
    lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont_col, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    hood_speed_obj[2] = lv_text_btn_create(cont_col, "高", (void *)3);
    hood_speed_obj[1] = lv_text_btn_create(cont_col, "中", (void *)2);
    hood_speed_obj[0] = lv_text_btn_create(cont_col, "低", (void *)1);

    lv_obj_t *turn_off_fire_rotate;
    lv_obj_t *turn_off_fire = lv_hood_item_create(cont_row, themesImagesPath "turn_off_fire_background.png",
                                                  "定时关火", &turn_off_fire_rotate);
    lv_rotate_anim(turn_off_fire_rotate, 1);

    lv_obj_t *intelligent_cooking_rotate;
    lv_obj_t *intelligent_cooking = lv_hood_item_create(cont_row, themesImagesPath "intelligent_cooking.png", "智慧烹饪",
                                                        &intelligent_cooking_rotate);
    lv_rotate_anim(intelligent_cooking_rotate, 0);
    lv_100ask_page_manager_set_load_page_event(intelligent_cooking, NULL, "page_smart_cook");
}
