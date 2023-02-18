

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
static lv_obj_t *row_canvas, *row;
static lv_draw_line_dsc_t draw_line_dsc;
static lv_point_t line_point[2];
static char steps = 0;
/**********************
 *  STATIC VARIABLES
 **********************/
static void line_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t *target = lv_event_get_target(e);
    // LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);

    if (code == LV_EVENT_PRESSED)
    {
        lv_indev_t *indev = lv_event_get_indev(e);
        point_x = indev->proc.types.pointer.act_point.x;
        point_y = indev->proc.types.pointer.act_point.y;
    }
    else if (code == LV_EVENT_PRESSING)
    {
        lv_indev_t *indev = lv_event_get_indev(e);
        LV_LOG_USER("%s,x:%d y:%d\n", __func__, indev->proc.types.pointer.act_point.x, indev->proc.types.pointer.act_point.y);

        line_point[0].x = point_x;
        line_point[0].y = point_y;
        line_point[1].x = indev->proc.types.pointer.act_point.x;
        line_point[1].y = indev->proc.types.pointer.act_point.y;
        lv_canvas_draw_line(row_canvas, line_point, 2, &draw_line_dsc);

        point_x = indev->proc.types.pointer.act_point.x;
        point_y = indev->proc.types.pointer.act_point.y;
    }
    else if (code == LV_EVENT_RELEASED)
    {
        lv_canvas_fill_bg(row_canvas, lv_color_hex(0xff0000), LV_OPA_40);
        ++steps;
        if (steps == 1)
        {
            lv_obj_align(row, LV_ALIGN_LEFT_MID, 0, 0);
            lv_obj_align(row_canvas, LV_ALIGN_LEFT_MID, 0, 0);
        }
        else if (steps == 2)
        {
            lv_obj_align(row, LV_ALIGN_BOTTOM_MID, 0, 0);
            lv_obj_align(row_canvas, LV_ALIGN_BOTTOM_MID, 0, 0);
        }
        else if (steps == 3)
        {
            lv_obj_set_size(row, 100, LV_PCT(100));
            lv_obj_align(row, LV_ALIGN_LEFT_MID, 0, 0);
            lv_obj_align(row_canvas, LV_ALIGN_LEFT_MID, 0, 0);
        }
        else if (steps == 4)
        {
            lv_obj_align(row, LV_ALIGN_TOP_MID, 0, 0);
            lv_obj_align(row_canvas, LV_ALIGN_TOP_MID, 0, 0);
        }
        else if (steps == 5)
        {
            lv_obj_align(row, LV_ALIGN_RIGHT_MID, 0, 0);
            lv_obj_align(row_canvas, LV_ALIGN_RIGHT_MID, 0, 0);
        }
        else if (steps == 6)
        {
            lv_obj_align(row, LV_ALIGN_TOP_MID, 25, 0);
            lv_obj_align(row_canvas, LV_ALIGN_TOP_MID, 0, 0);
            lv_obj_set_size(row, LV_PCT(100), 100);
            lv_obj_set_style_transform_angle(row, 130, 0);
        }
        else if (steps == 7)
        {
            lv_obj_set_style_transform_angle(row, -130, 0);
            lv_obj_align(row, LV_ALIGN_BOTTOM_MID, 0, 0);
            lv_obj_align(row_canvas, LV_ALIGN_BOTTOM_MID, 0, 0);
        }
        else
        {
            lv_page_back_previous_page();
        }
    }
}

void lv_page_screen_touch_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    row = lv_obj_create(page);
    lv_obj_set_size(row, LV_PCT(100), 100);
    lv_obj_align(row, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(row, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(row, LV_OPA_100, 0);
    lv_obj_add_event_cb(row, line_event_handler, LV_EVENT_ALL, 0);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);

    static char cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
    row_canvas = lv_canvas_create(row);
    lv_canvas_set_buffer(row_canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(row_canvas, lv_color_hex(0xff0000), LV_OPA_0);
    // lv_obj_align(row_canvas, LV_ALIGN_LEFT_MID, 0, 0);

    lv_draw_line_dsc_init(&draw_line_dsc);
    draw_line_dsc.width = 4;
    draw_line_dsc.color = lv_color_hex(themesTextColor);
    draw_line_dsc.opa = LV_OPA_100;

    line_point[0].x = 50;
    line_point[0].y = 50;
    line_point[1].x = 90;
    line_point[1].y = 90;
    lv_canvas_draw_line(row_canvas, line_point, 2, &draw_line_dsc);
}
