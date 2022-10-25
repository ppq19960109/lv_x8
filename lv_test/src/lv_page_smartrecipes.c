

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
static lv_obj_t * currentButton = NULL;

static void recipe_event_cb(lv_event_t * e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t * target = lv_event_get_target(e);
}

static void list_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj       = lv_event_get_current_target(e);
    lv_obj_t * list      = lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked: %s", lv_list_get_btn_text(list, obj));

        if(currentButton == obj) {
            return;
        } else {
            currentButton = obj;
        }
        lv_obj_t * parent = list;
        uint32_t i;
        for(i = 0; i < lv_obj_get_child_cnt(parent); i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);

            lv_obj_t * child1 = lv_obj_get_child(child, 0);
            if(child == currentButton) {
                lv_obj_add_state(child, LV_STATE_CHECKED);
                lv_obj_add_state(child1, LV_STATE_CHECKED);
            } else {
                lv_obj_clear_state(child, LV_STATE_CHECKED);
                lv_obj_clear_state(child1, LV_STATE_CHECKED);
            }
            lv_obj_center(child1);
        }
    }
}

static lv_obj_t * lv_text_img_btn_create(lv_obj_t * parent, const char * text)
{
    lv_obj_t * btn1 = lv_btn_create(parent);
    lv_obj_set_size(btn1, LV_PCT(100), 62);

    lv_obj_add_event_cb(btn1, list_event_handler, LV_EVENT_CLICKED, parent);
    lv_obj_set_style_bg_img_src(btn1, themesImagesPath "menulist_item_background.png", LV_STATE_CHECKED);

    lv_obj_t * label = lv_label_create(btn1);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_24, 0);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, LV_STATE_CHECKED);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    // lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return btn1;
}
static lv_obj_t * lv_recipe_create(lv_obj_t * parent, const char * img_src, const char * text)
{
    char imgUrl[256];
    sprintf(imgUrl, "%s%s%s", recipesImagesPath, img_src, "/0.png");
    LV_LOG_USER("img_src:%s text:%s imgUrl:%s", img_src, text, imgUrl);

    lv_obj_t * img = lv_img_create(parent);
    lv_obj_set_size(img, 142, 210);
    lv_img_set_src(img, imgUrl);
    lv_obj_add_event_cb(img, recipe_event_cb, LV_EVENT_CLICKED, parent);

    lv_obj_t * label = lv_label_create(img);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_20, 0);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_24, LV_STATE_CHECKED);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    return NULL;
}
void lv_page_smartrecipes_init(lv_obj_t * page)
{
    LV_LOG_USER("%s...", __func__);
    // lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t * back_bar = lv_page_back_bar_init(page, "智慧菜谱 (仅左腔支持智慧菜谱)", NULL);
    lv_page_top_bar_init(page, 2);

    lv_obj_t * left_content = lv_obj_create(page);
    lv_obj_set_size(left_content, 156, 345);
    lv_obj_align_to(left_content, back_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_t * menulist = lv_img_create(left_content);
    lv_obj_set_size(menulist, LV_PCT(100), LV_PCT(100));
    lv_img_set_src(menulist, themesImagesPath "menulist_background.png");

    lv_obj_t * list = lv_list_create(left_content);

    lv_obj_set_size(list, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 15);
    lv_text_img_btn_create(list, "蔬菜杂粮");
    lv_text_img_btn_create(list, "家禽肉类");
    lv_text_img_btn_create(list, "河海鲜类");
    lv_text_img_btn_create(list, "烘焙甜点");
    lv_text_img_btn_create(list, "其他食材");

    currentButton = lv_obj_get_child(list, 0);
    lv_obj_add_state(currentButton, LV_STATE_CHECKED);

    lv_obj_t * right_content = lv_obj_create(page);
    lv_obj_set_size(right_content, 152 * 4 + 180 + 30, 258);
    lv_obj_align_to(right_content, left_content, LV_ALIGN_OUT_RIGHT_TOP, 0, 26);

    lv_obj_set_flex_flow(right_content, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(right_content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    for(int i = 0; i < 20; ++i) {
        lv_recipe_create(right_content, g_recipes[i].imgUrl, g_recipes[i].dishName);
    }
}
