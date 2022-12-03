

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *left_steam_oven_rotate;
static lv_obj_t *right_steam_rotate;
/**********************
 *  STATIC VARIABLES
 **********************/
static void page_update_LStOvState(void)
{
    int value = get_attr_value_int("LStOvState");
    lv_obj_t *parent = lv_obj_get_parent(left_steam_oven_rotate);
    if (value)
    {
        lv_obj_clear_flag(left_steam_oven_rotate, LV_OBJ_FLAG_HIDDEN);
        lv_rotate_anim(left_steam_oven_rotate, 1);
        lv_100ask_page_manager_set_load_page_event(parent, NULL, "page_steaming");
    }
    else
    {
        lv_obj_add_flag(left_steam_oven_rotate, LV_OBJ_FLAG_HIDDEN);
        lv_rotate_anim(left_steam_oven_rotate, 0);
        lv_100ask_page_manager_set_load_page_event(parent, NULL, "page_steam_left");
    }
}
static void page_update_RStOvState(void)
{
    int value = get_attr_value_int("RStOvState");
    lv_obj_t *parent = lv_obj_get_parent(right_steam_rotate);
    if (value)
    {
        lv_obj_clear_flag(right_steam_rotate, LV_OBJ_FLAG_HIDDEN);
        lv_rotate_anim(right_steam_rotate, 1);
    }
    else
    {
        lv_obj_add_flag(right_steam_rotate, LV_OBJ_FLAG_HIDDEN);
        lv_rotate_anim(right_steam_rotate, 0);
    }
}
static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("key:%s\n", key);
    if (strcmp("LStOvState", key) == 0)
    {
        page_update_LStOvState();
    }
    else if (strcmp("RStOvState", key) == 0)
    {
        page_update_RStOvState();
    }
}
static void page_update_cb(void)
{
    page_update_LStOvState();
    page_update_RStOvState();
}

static void event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *target = lv_event_get_current_target(e); // lv_event_get_target(e);

    // static int run=1;
    // lv_obj_t * child = lv_obj_get_child(target, -1);
    // run=!run;
    // lv_rotate_anim(child, run);
}

static lv_obj_t *lv_steamoven_item_create(lv_obj_t *parent, const void *img_src, const void *text_img_src,
                                          lv_obj_t **img_out)
{
    lv_obj_t *item = lv_img_create(parent);
    lv_img_set_src(item, img_src);

    lv_obj_t *text_img = lv_img_create(item);
    lv_img_set_src(text_img, text_img_src);
    lv_obj_align(text_img, LV_ALIGN_TOP_MID, 0, 40);

    *img_out = lv_img_create(item);
    lv_obj_t *img = *img_out;
    lv_img_set_src(img, themesImagesPath "icon_runing.png");
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 115);
    lv_obj_update_layout(img);
    lv_img_set_pivot(img, lv_obj_get_width(img) / 2, lv_obj_get_height(img) / 2);

    return item;
}

static lv_obj_t *lv_steamoven_item2_create(lv_obj_t *parent, const void *img_src, const void *text_img_src)
{
    lv_obj_t *item = lv_img_create(parent);
    lv_img_set_src(item, img_src);

    lv_obj_t *text_img = lv_img_create(item);
    lv_img_set_src(text_img, text_img_src);
    lv_obj_align(text_img, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_add_event_cb(item, event_cb, LV_EVENT_CLICKED, NULL);
    return item;
}

void lv_page_steamoven_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;

    lv_obj_t *back_bar = lv_page_back_bar_init(page, "蒸烤箱", NULL, NULL);
    lv_page_top_bar_init(page, 1);

    lv_obj_t *cont_row = lv_obj_create(page);
    lv_obj_set_size(cont_row, 344 * 3 + 26 * 2, 266);
    lv_obj_align_to(cont_row, back_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 17);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *left_steam_oven =
        lv_steamoven_item_create(cont_row, themesImagesPath "left_steam_oven_background.png",
                                 themesImagesPath "left_steam_oven_text.png", &left_steam_oven_rotate);
    lv_rotate_anim(left_steam_oven_rotate, 1);

    lv_obj_t *right_steam = lv_steamoven_item_create(cont_row, themesImagesPath "right_steam_background.png",
                                                     themesImagesPath "right_steam_text.png", &right_steam_rotate);
    lv_rotate_anim(right_steam_rotate, 1);

    lv_obj_t *cont_col = lv_obj_create(cont_row);
    lv_obj_set_size(cont_col, 344, 266);
    lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont_col, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t *assist_cook = lv_steamoven_item2_create(cont_col, themesImagesPath "assist_cook_background.png",
                                                      themesImagesPath "assist_cook_text.png");
    lv_obj_t *multistage_cook = lv_steamoven_item2_create(cont_col, themesImagesPath "multistage_cook_background.png",
                                                          themesImagesPath "multistage_cook_text.png");
}
