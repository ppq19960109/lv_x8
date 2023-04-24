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
void lv_page_main_init(lv_obj_t *page)
{
    lv_obj_t *cont_row = lv_obj_create(page);

    lv_obj_set_size(cont_row, 292 * 3 + 100, 360);
    lv_obj_align(cont_row, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *home_hood = lv_img_create(cont_row);
    lv_img_set_src(home_hood, themesImagesPath "home_hood_background.png");
    lv_obj_t *label_title1 = lv_label_create(home_hood);
    lv_obj_set_style_text_font(label_title1, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_label_set_text(label_title1, "烟机灶具");
    lv_obj_set_style_text_color(label_title1, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_title1, LV_ALIGN_TOP_MID, 0, 45);
    lv_100ask_page_manager_set_load_page_event(home_hood, NULL, "page_hood");

    lv_obj_t *home_steamoven = lv_img_create(cont_row);
    lv_img_set_src(home_steamoven, themesImagesPath "home_steamoven_background.png");
    lv_obj_t *label_title2 = lv_label_create(home_steamoven);
    lv_obj_set_style_text_font(label_title2, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_label_set_text(label_title2, "蒸烤箱");
    lv_obj_set_style_text_color(label_title2, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_title2, LV_ALIGN_TOP_MID, 0, 45);
    lv_100ask_page_manager_set_load_page_event(home_steamoven, NULL, "page_steamoven");

    lv_obj_t *home_smartrecipes = lv_img_create(cont_row);
    lv_img_set_src(home_smartrecipes, themesImagesPath "home_smartrecipes_background.png");
    lv_obj_t *label_title3 = lv_label_create(home_smartrecipes);
    lv_obj_set_style_text_font(label_title3, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_label_set_text(label_title3, "智慧菜谱");
    lv_obj_set_style_text_color(label_title3, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_title3, LV_ALIGN_TOP_MID, 0, 45);
    lv_100ask_page_manager_set_load_page_event(home_smartrecipes, NULL, "page_smartrecipes");

    // gif = lv_gif_create(lv_scr_act());
    // lv_gif_set_src(gif, "S:/oem/boot.gif");
    // lv_gif_restart(gif);
}
