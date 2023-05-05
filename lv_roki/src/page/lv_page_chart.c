

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"
/*********************
 *      DEFINES
 *********************/
static lv_obj_t *chart1;
static lv_chart_series_t *ser1;
static lv_chart_series_t *ser2;
/**********************
 *  STATIC VARIABLES
 **********************/

static void draw_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);

    /*Add the faded area before the lines are drawn*/
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
    if (dsc->part == LV_PART_ITEMS)
    {
        if (!dsc->p1 || !dsc->p2)
            return;

        // /*Add a line mask that keeps the area below the line*/
        // lv_draw_mask_line_param_t line_mask_param;
        // lv_draw_mask_line_points_init(&line_mask_param, dsc->p1->x, dsc->p1->y, dsc->p2->x, dsc->p2->y, LV_DRAW_MASK_LINE_SIDE_BOTTOM);
        // int16_t line_mask_id = lv_draw_mask_add(&line_mask_param, NULL);

        // /*Add a fade effect: transparent bottom covering top*/
        // lv_coord_t h = lv_obj_get_height(obj);
        // lv_draw_mask_fade_param_t fade_mask_param;
        // lv_draw_mask_fade_init(&fade_mask_param, &obj->coords, LV_OPA_COVER, obj->coords.y1 + h / 8, LV_OPA_TRANSP, obj->coords.y2);
        // int16_t fade_mask_id = lv_draw_mask_add(&fade_mask_param, NULL);

        // /*Draw a rectangle that will be affected by the mask*/
        // lv_draw_rect_dsc_t draw_rect_dsc;
        // lv_draw_rect_dsc_init(&draw_rect_dsc);
        // draw_rect_dsc.bg_opa = LV_OPA_20;
        // draw_rect_dsc.bg_color = dsc->line_dsc->color;

        // lv_area_t a;
        // a.x1 = dsc->p1->x;
        // a.x2 = dsc->p2->x - 1;
        // a.y1 = LV_MIN(dsc->p1->y, dsc->p2->y);
        // a.y2 = obj->coords.y2;
        // lv_draw_rect(dsc->draw_ctx, &draw_rect_dsc, &a);

        // /*Remove the masks*/
        // lv_draw_mask_free_param(&line_mask_param);
        // lv_draw_mask_free_param(&fade_mask_param);
        // lv_draw_mask_remove_id(line_mask_id);
        // lv_draw_mask_remove_id(fade_mask_id);
    }
    /*Hook the division lines too*/
    else if (dsc->part == LV_PART_MAIN)
    {
        if (dsc->line_dsc == NULL || dsc->p1 == NULL || dsc->p2 == NULL)
            return;

        /*Vertical line*/
        if (dsc->p1->x == dsc->p2->x)
        {
            dsc->line_dsc->color = lv_palette_lighten(LV_PALETTE_GREY, 1);
            dsc->line_dsc->width = 1;
            dsc->line_dsc->dash_gap = 6;
            dsc->line_dsc->dash_width = 6;
        }
        /*Horizontal line*/
        else
        {
            dsc->line_dsc->color = lv_color_hex(0xFFFFFF);
            dsc->line_dsc->opa = LV_OPA_10;
            dsc->line_dsc->width = 3;
        }
    }
}

static void add_data(lv_timer_t *timer)
{
    LV_UNUSED(timer);
    static uint32_t cnt = 0;
    lv_chart_set_next_value(chart1, ser1, lv_rand(20, 90));

    if (cnt % 4 == 0)
        lv_chart_set_next_value(chart1, ser2, lv_rand(40, 60));

    cnt++;
}

static void btn_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        break;
    case 1:

        break;
    }
}
void lv_page_chart_create(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);

    chart1 = lv_chart_create(page);
    lv_obj_set_size(chart1, 605, 295);
    lv_obj_set_style_radius(chart1, 0, 0);
    lv_obj_set_style_bg_opa(chart1, LV_OPA_0, 0);
    lv_obj_set_style_border_opa(chart1, LV_OPA_0, 0);

    lv_chart_set_type(chart1, LV_CHART_TYPE_LINE);
    lv_obj_align(chart1, LV_ALIGN_TOP_LEFT, 110, 120);
    lv_chart_set_div_line_count(chart1, 6, 0);
    lv_chart_set_update_mode(chart1, LV_CHART_UPDATE_MODE_CIRCULAR);

    lv_chart_set_range(chart1, LV_CHART_AXIS_PRIMARY_X, 0, 100);
    lv_chart_set_range(chart1, LV_CHART_AXIS_PRIMARY_Y, 0, 250);
    lv_chart_set_point_count(chart1, 10);

    lv_chart_set_axis_tick(chart1, LV_CHART_AXIS_PRIMARY_X, 10, 5, 10, 3, true, 50);
    lv_chart_set_axis_tick(chart1, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 4, true, 60);

    lv_chart_series_t *ser1 = lv_chart_add_series(chart1, lv_color_hex(0xffffff), LV_CHART_AXIS_PRIMARY_Y);

    lv_obj_add_event_cb(chart1, draw_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);

    lv_chart_set_next_value(chart1, ser1, 0);
    lv_chart_set_next_value(chart1, ser1, 100);
    lv_chart_set_next_value(chart1, ser1, 200);
    lv_chart_set_next_value(chart1, ser1, 200);
    lv_chart_set_next_value(chart1, ser1, 210);
}
