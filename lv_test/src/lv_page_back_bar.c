

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
lv_obj_t *lv_custom_back_button_create(lv_obj_t *parent, const void *img_src, const char *text)
{
    lv_obj_t *image_button = lv_obj_create(parent);
    lv_obj_set_size(image_button, LV_SIZE_CONTENT, 55);
    lv_obj_t *img = lv_img_create(image_button);
    lv_img_set_src(img, img_src);
    if (text == NULL)
    {
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    }
    else
    {
        lv_obj_align(img, LV_ALIGN_LEFT_MID, 10, 0);
        lv_obj_t *label_name = lv_label_create(image_button);
        lv_obj_set_style_text_font(label_name, &lv_font_SiYuanHeiTi_Normal_24, 0);
        lv_obj_set_style_text_color(label_name, lv_color_hex(themesTextColor2), 0);
        lv_label_set_text(label_name, text);
        lv_obj_align_to(label_name, img, LV_ALIGN_OUT_RIGHT_MID, 15, 0);
    }

    return image_button;
}
static void default_event_cb(lv_event_t *e)
{
    // LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *target = lv_event_get_current_target(e); // lv_event_get_target(e);
    lv_page_back_previous_page();
    // lv_page_back_top_page();
}

lv_obj_t *lv_page_back_bar_init(lv_obj_t *parent, const char *name, const char *title, lv_event_cb_t event_cb)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t *back_bar = lv_obj_create(parent);
    lv_obj_set_size(back_bar, LV_PCT(100), 55);
    lv_obj_set_align(back_bar, LV_ALIGN_TOP_MID);

    lv_obj_t *back_button = lv_custom_back_button_create(back_bar, themesImagesPath "back_button_background.png", name);

    if (event_cb == NULL)
        lv_obj_add_event_cb(back_button, default_event_cb, LV_EVENT_CLICKED, NULL);
    else
        lv_obj_add_event_cb(back_button, event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *back_line = lv_obj_create(back_bar);
    lv_obj_set_size(back_line, LV_PCT(100), 1);
    lv_obj_set_align(back_line, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_bg_color(back_line, lv_color_hex(0x3A3A3A), 0);
    lv_obj_set_style_bg_opa(back_line, LV_OPA_100, 0);

    if (title != NULL)
    {
        lv_obj_t *label_title = lv_label_create(back_bar);
        lv_obj_set_style_text_font(label_title, &lv_font_SiYuanHeiTi_Normal_30, 0);
        lv_label_set_text(label_title, title);
        lv_obj_set_style_text_color(label_title, lv_color_hex(themesTextColor2), 0);
        lv_obj_align(label_title, LV_ALIGN_CENTER, 0, 0);
    }
    return back_bar;
}
