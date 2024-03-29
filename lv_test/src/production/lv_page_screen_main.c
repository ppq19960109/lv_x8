

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

static void btn_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        lv_100ask_page_manager_set_open_page(NULL, "page_screen_lcd");
        break;
    case 1:
        lv_100ask_page_manager_set_open_page(NULL, "page_screen_touch");
        break;
    case 2:

        break;
    case 3:
        lv_100ask_page_manager_set_open_page(NULL, "page_screen_line");
        break;
    case 4:
        lv_100ask_page_manager_set_open_page(NULL, "page_screen_click");
        break;
    case 5:
        lv_100ask_page_manager_set_open_page(NULL, "page_screen_aging");
        break;
    }
}
static lv_obj_t *lv_text_btn_create(lv_obj_t *parent, const char *text, void *user_data)
{
    lv_obj_t *btn1 = lv_btn_create(parent);
    lv_obj_set_size(btn1, 200, 100);
    lv_obj_set_style_bg_color(btn1, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(btn1, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn1, 10, 0);
    lv_obj_add_event_cb(btn1, btn_event_cb, LV_EVENT_CLICKED, user_data);

    lv_obj_t *label = lv_label_create(btn1);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return btn1;
}
void lv_page_screen_main_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t *back_bar = lv_page_back_bar_init(page, "屏幕检测", NULL, NULL);

    static lv_coord_t col_dsc[] = {200, 200, 200, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {100, 100, LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    lv_obj_t *cont = lv_obj_create(page);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);
    // lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 200 * 3 + 200, 100 * 2 + 50);
    lv_obj_align_to(cont, back_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
    lv_obj_set_grid_align(cont, LV_GRID_ALIGN_SPACE_BETWEEN, LV_GRID_ALIGN_SPACE_BETWEEN);

    lv_obj_t *element_obj = lv_text_btn_create(cont, "LCD", (void *)0);
    lv_obj_set_grid_cell(element_obj, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    element_obj = lv_text_btn_create(cont, "触摸", (void *)1);
    lv_obj_set_grid_cell(element_obj, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    element_obj = lv_text_btn_create(cont, "背光", (void *)2);
    lv_obj_set_grid_cell(element_obj, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    element_obj = lv_text_btn_create(cont, "划线耐久", (void *)3);
    lv_obj_set_grid_cell(element_obj, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    element_obj = lv_text_btn_create(cont, "点击耐久", (void *)4);
    lv_obj_set_grid_cell(element_obj, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    element_obj = lv_text_btn_create(cont, "老化测试", (void *)5);
    lv_obj_set_grid_cell(element_obj, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
}
