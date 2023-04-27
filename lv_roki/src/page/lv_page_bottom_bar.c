

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static lv_obj_t *lv_bottom_btn_create(lv_obj_t *parent, const char *text)
{
    lv_obj_t *btn = lv_img_create(parent);
    lv_img_set_src(btn, getThemesPath("bg_button_1.png"));
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label, g_robam_font.FZLTHJW_36.font, 0);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return btn;
}
lv_obj_t *lv_page_bottom_bar_init(lv_obj_t *parent, const char *left_btn_text, const char *right_btn_text, lv_event_cb_t event_cb)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t *back_bar = lv_img_create(parent);
    lv_img_set_src(back_bar, publicImagesPath "bottom_bg.png");
    // lv_obj_set_size(back_bar, LV_PCT(100), 96);
    lv_obj_set_align(back_bar, LV_ALIGN_BOTTOM_MID);
    
    if (left_btn_text != NULL)
    {
        lv_obj_t *left_button = lv_bottom_btn_create(back_bar, left_btn_text);
        lv_obj_align(left_button, LV_ALIGN_LEFT_MID, 40, 0);
        if (event_cb != NULL)
            lv_obj_add_event_cb(left_button, event_cb, LV_EVENT_CLICKED, (void *)0);
    }

    lv_obj_t *right_button = lv_bottom_btn_create(back_bar, right_btn_text);
    lv_obj_align(right_button, LV_ALIGN_RIGHT_MID, -40, 0);
    if (event_cb != NULL)
        lv_obj_add_event_cb(right_button, event_cb, LV_EVENT_CLICKED, (void *)1);

    return back_bar;
}
