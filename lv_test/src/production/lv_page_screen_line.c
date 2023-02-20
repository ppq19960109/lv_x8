

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
#define CANVAS_WIDTH 1280
#define CANVAS_HEIGHT 400
static int point_x = 0, point_y = 0;
static lv_obj_t *row_canvas, *col_canvas;
static lv_draw_line_dsc_t draw_line_dsc;
static lv_point_t line_point[2];
/**********************
 *  STATIC VARIABLES
 **********************/
static void line_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t *current_target = lv_event_get_current_target(e);
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%ld\n", __func__, e->code, user_data);
    if (user_data == 2)
    {
        if (code == LV_EVENT_CLICKED)
            lv_page_back_previous_page();
        return;
    }

    if (code == LV_EVENT_PRESSED)
    {
        if (user_data == 0)
        {
            lv_obj_move_foreground(row_canvas);
        }
        else
        {
            lv_obj_move_foreground(col_canvas);
        }
        lv_indev_t *indev = lv_event_get_indev(e);
        point_x = indev->proc.types.pointer.act_point.x;
        point_y = indev->proc.types.pointer.act_point.y;
    }
    else if (code == LV_EVENT_PRESSING)
    {
        lv_indev_t *indev = lv_event_get_indev(e);
        LV_LOG_USER("%s,x:%d y:%d\n", __func__, indev->proc.types.pointer.act_point.x, indev->proc.types.pointer.act_point.y);

        if (user_data == 0)
        {
            line_point[0].x = point_x;
            line_point[0].y = point_y - 150;
            line_point[1].x = indev->proc.types.pointer.act_point.x;
            line_point[1].y = indev->proc.types.pointer.act_point.y - 150;
            lv_canvas_draw_line(row_canvas, line_point, 2, &draw_line_dsc);
        }
        else
        {
            line_point[0].x = point_x - 590;
            line_point[0].y = point_y;
            line_point[1].x = indev->proc.types.pointer.act_point.x - 590;
            line_point[1].y = indev->proc.types.pointer.act_point.y;
            lv_canvas_draw_line(col_canvas, line_point, 2, &draw_line_dsc);
        }
        point_x = indev->proc.types.pointer.act_point.x;
        point_y = indev->proc.types.pointer.act_point.y;
    }
    else if (code == LV_EVENT_RELEASED)
    {
        if (user_data == 0)
        {
            lv_canvas_fill_bg(row_canvas, lv_color_hex(0xff0000), LV_OPA_40);
        }
        else
        {
            lv_canvas_fill_bg(col_canvas, lv_color_hex(0xff0000), LV_OPA_40);
        }
    }
}

void lv_page_screen_line_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_obj_t *row = lv_obj_create(page);
    lv_obj_set_size(row, LV_PCT(100), 100);
    lv_obj_align(row, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_color(row, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(row, LV_OPA_100, 0);
    lv_obj_add_event_cb(row, line_event_handler, LV_EVENT_ALL, 0);

    lv_obj_t *col = lv_obj_create(page);
    lv_obj_set_size(col, 100, LV_PCT(100));
    lv_obj_align(col, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(col, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(col, LV_OPA_100, 0);
    lv_obj_add_event_cb(col, line_event_handler, LV_EVENT_ALL, (void*)1);

    static char cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, 100)];
    row_canvas = lv_canvas_create(page);
    lv_canvas_set_buffer(row_canvas, cbuf, CANVAS_WIDTH, 100, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(row_canvas, lv_color_hex(0xff0000), LV_OPA_40);
    lv_obj_align(row_canvas, LV_ALIGN_LEFT_MID, 0, 0);

    static char col_cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(100, CANVAS_HEIGHT)];
    col_canvas = lv_canvas_create(page);
    lv_canvas_set_buffer(col_canvas, col_cbuf, 100, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(col_canvas, lv_color_hex(0xff0000), LV_OPA_40);
    lv_obj_align(col_canvas, LV_ALIGN_TOP_MID, 0, 0);

    lv_draw_line_dsc_init(&draw_line_dsc);
    draw_line_dsc.width = 4;
    draw_line_dsc.color = lv_color_hex(themesTextColor);
    draw_line_dsc.opa = LV_OPA_100;

    line_point[0].x = 50;
    line_point[0].y = 50;
    line_point[1].x = 90;
    line_point[1].y = 90;
    lv_canvas_draw_line(row_canvas, line_point, 2, &draw_line_dsc);

    lv_obj_t *btn = lv_btn_create(page);
    lv_obj_set_size(btn, 140, 50);
    lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn, 25, 0);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(btn, line_event_handler, LV_EVENT_CLICKED, (void *)2);
    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
    lv_label_set_text(label, "退出");
    lv_obj_center(label);
}
