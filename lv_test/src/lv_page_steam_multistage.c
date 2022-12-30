

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static int listLastIndex = 0;

/**********************
 *  STATIC VARIABLES
 **********************/

static void dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *obj = lv_event_get_current_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
        left_cook_start(0);
    }
    break;
    }
    lv_obj_clean(lv_layer_top());
}
static void reserve_dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *obj = lv_event_get_current_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
        lv_obj_t *reserve_dialog = lv_obj_get_child(lv_layer_top(), 0);
        int orderTime = lv_get_reserve_dialog_time(reserve_dialog);
        LV_LOG_USER("%s,orderTime:%d\n", __func__, orderTime);
        left_cook_start(orderTime);
    }
    break;
    }
    lv_obj_clean(lv_layer_top());
}

static void btn_array_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    if (user_data == 0)
    {
        lv_custom_cook_dialog("请将食物放入左腔,水箱中加满水", dialog_event_cb);
    }
    else
    {
        lv_custom_reserve_dialog("左腔将在", reserve_dialog_event_cb);
    }
}
static void click_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
    }
    break;
    }
}

void lv_page_multistage_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t *back_bar = lv_page_back_bar_init(page, "多段烹饪(只有左腔可多段烹饪)", NULL, NULL);

    lv_obj_t *cont_row = lv_obj_create(page);
    lv_obj_set_size(cont_row, 300 * 3, 250);
    lv_obj_align_to(cont_row, back_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 50);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    //------------------------------
    for (int i = 0; i < 3; ++i)
    {
        lv_obj_t *first_obj = lv_obj_create(cont_row);
        lv_obj_set_size(first_obj, 300, LV_PCT(100));
        lv_obj_t *img = lv_img_create(first_obj);
        lv_img_set_src(img, themesImagesPath "icon_round_checkable.png");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        img = lv_img_create(first_obj);
        lv_img_set_src(img, themesImagesPath "icon_cook_add_checked.png");
        lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 66);

        lv_obj_t *label = lv_label_create(first_obj);
        lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_24, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0xDF932F), 0);
        lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 166);
        lv_label_set_text(label, "第1段");

        lv_obj_add_event_cb(first_obj, click_event_cb, LV_EVENT_CLICKED, (void *)i);
    }
    //------------------------------
    const char *text[] = {"启动", "预约"};
    lv_obj_t *btn_array = lv_custom_btn_array_create(page, text, 2, btn_array_event_cb);
    lv_obj_align_to(btn_array, back_bar, LV_ALIGN_OUT_BOTTOM_RIGHT, -60, 90);
}
