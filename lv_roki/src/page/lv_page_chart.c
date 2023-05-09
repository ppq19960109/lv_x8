

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"
/*********************
 *      DEFINES
 *********************/
static lv_obj_t *chart1;
static lv_obj_t *btn_preheat, *btn_vapour, *btn_start_stop;
static lv_chart_series_t *ser1;
static lv_timer_t *chart_timer;
static char x_time_text[6][10];
/**********************
 *  STATIC VARIABLES
 **********************/

static void draw_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);

    /*Add the faded area before the lines are drawn*/
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);

    if (dsc->id == LV_CHART_AXIS_PRIMARY_X && dsc->text)
    {
        int time_sec = lv_chart_get_point_count(obj) * 2 / 6;

        int cur_time = 0;
        for (int i = 1; i < 6; i++)
        {
            cur_time = time_sec * i;
            if (cur_time < 60)
            {
                sprintf(x_time_text[i], "%ds", cur_time);
            }
            else
            {
                if (cur_time % 60 == 0)
                {
                    sprintf(x_time_text[i], "%dmin", cur_time / 60);
                }
                else
                {
                    sprintf(x_time_text[i], "%dmin%ds", cur_time / 60, cur_time % 60);
                }
            }
        }
        lv_snprintf(dsc->text, dsc->text_length, "%s", x_time_text[dsc->value]);
    }
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
    lv_chart_set_next_value(chart1, ser1, lv_rand(10, 250));

    unsigned short point_count = lv_chart_get_point_count(chart1);
    unsigned short x_start_point = lv_chart_get_x_start_point(chart1, ser1);
    LV_LOG_USER("%s,point_count:%d x_start_point:%d\n", __func__, point_count, x_start_point);
    if (point_count < 45 * 60) // * 60
    {
        if (point_count - 1 <= x_start_point)
        {
            lv_chart_set_x_start_point(chart1, ser1, 0);
            lv_chart_set_point_count(chart1, point_count * 1.3);
            lv_chart_set_x_start_point(chart1, ser1, x_start_point);
        }
    }
    else
    {
        if (point_count - 1 <= x_start_point)
        {
            unsigned short start_point2 = point_count / 4;
            unsigned short point_count2 = point_count - start_point2;
            lv_chart_set_x_start_point(chart1, ser1, start_point2);
            lv_chart_set_point_count(chart1, point_count2);
            lv_chart_set_x_start_point(chart1, ser1, point_count2 - 2);
        }
    }
}
// static void btn_img_array_event_cb(int start_index, char *text[], const int count)
// {
//     lv_obj_t *child;
//     for (int i = start_index; i < count; ++i)
//     {
//         child = lv_obj_get_child(btn_img_array, i);
//         // lv_obj_set_ext_click_area(child, 10);
//         lv_img_set_src(lv_obj_get_child(child, 0), getThemesPath(text[i]));
//     }
// }
static void btn_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        break;
    case 1:
    {
    }
    break;
    }
}
void lv_page_chart_completed_cb(void *arg)
{
    lv_timer_resume(chart_timer);
    lv_timer_ready(chart_timer);
}
void lv_page_chart_destroyed_cb(void *arg)
{
    lv_timer_pause(chart_timer);
}
void lv_page_chart_create(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);

    chart1 = lv_chart_create(page);
    lv_obj_set_size(chart1, 605, 295);
    lv_obj_set_style_radius(chart1, 0, 0);
    lv_obj_set_style_bg_opa(chart1, LV_OPA_0, 0);
    lv_obj_set_style_border_opa(chart1, LV_OPA_0, 0);
    // lv_obj_set_style_line_color(chart1, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_TICKS);

    lv_chart_set_type(chart1, LV_CHART_TYPE_LINE);
    lv_obj_align(chart1, LV_ALIGN_TOP_LEFT, 110, 120);
    lv_chart_set_div_line_count(chart1, 6, 0);
    lv_chart_set_update_mode(chart1, LV_CHART_UPDATE_MODE_CIRCULAR);

    lv_obj_set_style_size(chart1, 0, LV_PART_INDICATOR);
    // lv_obj_set_style_bg_img_src(chart1, publicImagesPath "knob.png", LV_PART_INDICATOR);
    // lv_obj_set_style_pad_all(chart1, 30, LV_PART_INDICATOR);

    lv_chart_set_range(chart1, LV_CHART_AXIS_PRIMARY_X, 0, 250);
    lv_chart_set_range(chart1, LV_CHART_AXIS_PRIMARY_Y, 0, 250);
    lv_chart_set_point_count(chart1, 6);

    lv_chart_set_axis_tick(chart1, LV_CHART_AXIS_PRIMARY_X, 10, 5, 6, 2, true, 50);
    lv_chart_set_axis_tick(chart1, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 2, true, 60);

    ser1 = lv_chart_add_series(chart1, lv_color_hex(0xffffff), LV_CHART_AXIS_PRIMARY_Y);

    lv_obj_add_event_cb(chart1, draw_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
    // lv_chart_set_zoom_x(chart1, LV_IMG_ZOOM_NONE / 2);

    // lv_chart_set_next_value(chart1, ser1, 0);
    // lv_chart_set_next_value(chart1, ser1, 100);
    // lv_chart_set_next_value(chart1, ser1, 200);
    // lv_chart_set_next_value(chart1, ser1, 200);

    lv_chart_refresh(chart1);

    if (chart_timer == NULL)
        chart_timer = lv_timer_create(add_data, 2000, NULL);

    //----------------------------------------------------------------
    lv_obj_t *img = lv_img_create(page);
    lv_img_set_src(img, getThemesPath("bg_stop.png"));
    lv_obj_align(img, LV_ALIGN_TOP_RIGHT, -45, 300);
    lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img, btn_event_cb, LV_EVENT_CLICKED, (void *)0);
    lv_obj_set_ext_click_area(img, 5);
    btn_start_stop = img;

    img = lv_img_create(page);
    lv_img_set_src(img, getThemesPath("bg_preheat.png"));
    lv_obj_align(img, LV_ALIGN_TOP_RIGHT, -45, 170);
    lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img, btn_event_cb, LV_EVENT_CLICKED, (void *)1);
    lv_obj_set_ext_click_area(img, 5);
    btn_preheat = img;

    img = lv_img_create(page);
    lv_img_set_src(img, getThemesPath("bg_vapour.png"));
    lv_obj_align(img, LV_ALIGN_TOP_RIGHT, -45, 170);
    lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img, btn_event_cb, LV_EVENT_CLICKED, (void *)2);
    lv_obj_set_ext_click_area(img, 5);
    btn_vapour = img;

    lv_obj_add_flag(btn_preheat, LV_OBJ_FLAG_HIDDEN);
}
