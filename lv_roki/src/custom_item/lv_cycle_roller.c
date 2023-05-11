#include "lv_cycle_roller.h"

#define MY_CLASS &lv_cycle_roller_class

static void lv_cycle_roller_constructor(const lv_obj_class_t *class_p, lv_obj_t *obj);
static void lv_cycle_roller_destructor(const lv_obj_class_t *class_p, lv_obj_t *obj);
static void lv_cycle_roller_event(const lv_obj_class_t *class_p, lv_event_t *e);
/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_cycle_roller_class = {
    .constructor_cb = lv_cycle_roller_constructor,
    .destructor_cb = lv_cycle_roller_destructor,
    .event_cb = lv_cycle_roller_event,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_cycle_roller_t),
    .base_class = &lv_obj_class};

lv_obj_t *lv_cycle_roller_create(lv_obj_t *parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t *obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}
void lv_cycle_roller_set_state(lv_obj_t *obj, char cycle_flag, char mask_flag, char flex_flow, cycle_roller_child_cb cb)
{
    lv_cycle_roller_t *cycle_roller = (lv_cycle_roller_t *)obj;
    cycle_roller->cycle_flag = cycle_flag;
    cycle_roller->mask_flag = mask_flag;
    cycle_roller->flex_flow = flex_flow;
    cycle_roller->cb = cb;

    lv_obj_set_flex_flow(obj, flex_flow);
    if (LV_FLEX_FLOW_COLUMN == (flex_flow & 0x01))
    {
        lv_obj_set_scroll_dir(obj, LV_DIR_VER);               // 设置Scroll的允许方向direction：垂直方向
        lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_CENTER); // 捕捉Cont Y方向的子对象，将他们与Container中心对齐
    }
    else
    {
        lv_obj_set_scroll_dir(obj, LV_DIR_HOR);               // 设置Scroll的允许方向direction：垂直方向
        lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_CENTER); // 捕捉Cont Y方向的子对象，将他们与Container中心对齐
    }
}
/**********************************************************************
**函数名称: cycle_roller_to_index
**函数说明: 循环滚轮修改中心索引
**输入参数: cycle_roller：循环滚轮对象 cur_index：目标中心索引
**输出参数: none
**返 回 值: none
************************************************************************/
void cycle_roller_to_index(lv_obj_t *cycle_roller, const unsigned int cur_index, const int event_scroll)
{
    lv_cycle_roller_t *lv_cycle_roller = (lv_cycle_roller_t *)cycle_roller;
    if (lv_cycle_roller->cycle_flag == 0)
    {
        lv_obj_scroll_to_view(lv_obj_get_child(cycle_roller, cur_index), LV_ANIM_OFF);
        lv_cycle_roller->selected_index = cur_index;
        return;
    }
    uint32_t child_cnt = lv_obj_get_child_cnt(cycle_roller); // 获取子界面的数量
    int mid_btn_index = (child_cnt - 1) / 2;                 // 如果界面为偶数，将中间数向下取整的界面设置为中间界面

    int roll_direction = cur_index - mid_btn_index; // 确定滚动方向
    if (roll_direction != 0)
    {
        /* 通过循环将指定界面移到中心位置 */
        for (int i = 0; i < LV_ABS(roll_direction); ++i)
        {
            if (roll_direction < 0)
            {
                lv_obj_move_to_index(lv_obj_get_child(cycle_roller, child_cnt - 1), 0); // 将最后一个界面移至第一个界面
            }
            else
            {
                lv_obj_move_to_index(lv_obj_get_child(cycle_roller, 0), child_cnt - 1); // 将第一个界面移至最后一个界面
            }
        }
    }
    /*当按钮数为偶数时，确保按钮居中*/
    lv_obj_scroll_to_view(lv_obj_get_child(cycle_roller, mid_btn_index), LV_ANIM_OFF);
    if (event_scroll > 0)
        lv_event_send(cycle_roller, LV_EVENT_SCROLL, NULL);
}
/**********************************************************************
**函数名称: cycle_roller_to_userdata
**函数说明: 循环滚轮修改中心索引
**输入参数: cycle_roller：循环滚轮对象 cur_index：目标中心索引
**输出参数: none
**返 回 值: none
************************************************************************/
void cycle_roller_to_userdata(lv_obj_t *cycle_roller, const unsigned int value, const int event_scroll)
{
    lv_cycle_roller_t *lv_cycle_roller = (lv_cycle_roller_t *)cycle_roller;
    if (lv_cycle_roller->cycle_flag == 0)
    {
        lv_obj_scroll_to_view(lv_obj_get_child(cycle_roller, value), LV_ANIM_OFF);
        lv_cycle_roller->selected_index = value;
        return;
    }
    int i = 0;
    lv_obj_t *child;
    uint32_t child_cnt = lv_obj_get_child_cnt(cycle_roller); // 获取子界面的数量
    for (i = 0; i < child_cnt; ++i)
    {
        child = lv_obj_get_child(cycle_roller, i);
        if ((int)child->user_data == value)
            break;
    }
    LV_LOG_USER("%s,value:%d index:%d\n", __func__, value, i);
    cycle_roller_to_index(cycle_roller, i, event_scroll);
}
/**********************************************************************
**函数名称: lv_cycle_roller_get_selected_index
**函数说明: 获取循环滚轮中心索引
**输入参数: cycle_roller：循环滚轮对象
**输出参数: none
**返 回 值: 滚轮中心索引
************************************************************************/
int lv_cycle_roller_get_selected_index(lv_obj_t *cycle_roller)
{
    lv_cycle_roller_t *lv_cycle_roller = (lv_cycle_roller_t *)cycle_roller;
    if (lv_cycle_roller->cycle_flag == 0)
        return lv_cycle_roller->selected_index;
    lv_coord_t child_cnt = lv_obj_get_child_cnt(cycle_roller); // 获取子界面的数量
    return (child_cnt - 1) / 2;                                // 中间界面的位置
}
/**********************************************************************
**函数名称: lv_cycle_roller_get_selected
**函数说明: 获取循环滚轮中心索引对象
**输入参数: cycle_roller：循环滚轮对象
**输出参数: none
**返 回 值: 滚轮中心索引对象
************************************************************************/
lv_obj_t *lv_cycle_roller_get_selected(lv_obj_t *cycle_roller)
{
    if (lv_obj_get_child_cnt(cycle_roller) == 0)
    {
        LV_LOG_USER("%s,cycle_roller parent is NULL\n", __func__);
        return NULL;
    }
    int selected_index = lv_cycle_roller_get_selected_index(cycle_roller);
    // printf("%s,selected_index:%d\n", __func__, selected_index);
    return lv_obj_get_child(cycle_roller, selected_index);
}
/**********************************************************************
**函数名称: lv_cycle_roller_unit_create
**函数说明: 循环滚轮创建
**输入参数: parent：父对象 width：宽 height：高 flow：布局方向 cycle_roller_child_cb：子对象选中和不选中的回调
**输出参数: none
**返 回 值: 循环滚轮对象
************************************************************************/
lv_obj_t *lv_cycle_roller_unit_create(lv_obj_t *parent, int width, int height, lv_flex_flow_t flow)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, width, height);

    lv_obj_t *cycle_roller = lv_cycle_roller_create(obj);
    lv_obj_set_size(cycle_roller, LV_PCT(75), height);

    return cycle_roller;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static void mask_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    static int16_t mask_top_id = -1;
    static int16_t mask_bottom_id = -1;

    if (code == LV_EVENT_COVER_CHECK)
    {
        lv_event_set_cover_res(e, LV_COVER_RES_MASKED);
    }
    else if (code == LV_EVENT_DRAW_MAIN_BEGIN)
    {
        /* add mask */
        const lv_font_t *font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        lv_coord_t line_space = lv_obj_get_style_text_line_space(obj, LV_PART_MAIN);
        lv_coord_t font_h = lv_font_get_line_height(font);

        lv_area_t roller_coords;
        lv_obj_get_coords(obj, &roller_coords);

        lv_area_t rect_area;
        rect_area.x1 = roller_coords.x1;
        rect_area.x2 = roller_coords.x2;
        rect_area.y1 = roller_coords.y1;
        rect_area.y2 = roller_coords.y1 + (lv_obj_get_height(obj) - font_h - line_space) / 2;

        lv_draw_mask_fade_param_t *fade_mask_top = lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_top, &rect_area, LV_OPA_TRANSP, rect_area.y1, LV_OPA_COVER, rect_area.y2);
        mask_top_id = lv_draw_mask_add(fade_mask_top, NULL);

        rect_area.y1 = rect_area.y2 + font_h + line_space - 1;
        rect_area.y2 = roller_coords.y2;

        lv_draw_mask_fade_param_t *fade_mask_bottom = lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_bottom, &rect_area, LV_OPA_COVER, rect_area.y1, LV_OPA_TRANSP, rect_area.y2);
        mask_bottom_id = lv_draw_mask_add(fade_mask_bottom, NULL);
    }
    else if (code == LV_EVENT_DRAW_POST_END)
    {
        lv_draw_mask_fade_param_t *fade_mask_top = lv_draw_mask_remove_id(mask_top_id);
        lv_draw_mask_fade_param_t *fade_mask_bottom = lv_draw_mask_remove_id(mask_bottom_id);
        lv_draw_mask_free_param(fade_mask_top);
        lv_draw_mask_free_param(fade_mask_bottom);
        lv_mem_buf_release(fade_mask_top);
        lv_mem_buf_release(fade_mask_bottom);
        mask_top_id = -1;
        mask_bottom_id = -1;
    }
}
/**********************************************************************
**函数名称: roller_scroll_y_event
**函数说明: 循环滚轮的事件
**输入参数: none
**输出参数: none
**返 回 值: none
************************************************************************/
static void roller_scroll_y_event(lv_event_t *e)
{
    lv_obj_t *cont = lv_event_get_target(e); // 获取事件的初始对象
    lv_cycle_roller_t *lv_cycle_roller = (lv_cycle_roller_t *)cont;
    if (LV_FLEX_FLOW_COLUMN != (lv_cycle_roller->flex_flow & 0x01))
        return;
    if (lv_cycle_roller != NULL && lv_cycle_roller->mask_flag)
        mask_event_cb(e);
    if (e->code != LV_EVENT_SCROLL_END && e->code != LV_EVENT_SCROLL)
        return;

    /* 获取事件的事件代码 */
    /* 判断是否在滚动中 */
    if (lv_obj_is_scrolling(cont) && e->code == LV_EVENT_SCROLL_END)
    {
        // LV_LOG_ERROR("%s,lv_obj_is_scrolling code:%d\n", __func__, e->code);
        return;
    }
    // LV_LOG_USER("%s,code:%d enter\n", __func__, e->code);

    lv_coord_t child_cnt = lv_obj_get_child_cnt(cont); // 获取子界面的数量
    // lv_coord_t mid_btn_index = (child_cnt - 1) / 2;    // 中间界面的位置

    /* 获取父对象y轴的中心坐标值 */
    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);                                       // 将cont对象的坐标复制到cont_a
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2; // 获取界面的宽像素大小/2
    // lv_coord_t cont_y_center = lv_obj_get_height(cont) / 2;
    /* 遍历子界面 */
    for (lv_coord_t i = 0; i < child_cnt; i++)
    {
        lv_obj_t *child = lv_obj_get_child(cont, i); // 通过索引获取子对象

        /* 获取子对象y轴的中心坐标值 */
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);
        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2; // 获取界面中按钮宽像素值的大小/2
        // lv_coord_t child_y_center = lv_obj_get_y(child) + lv_obj_get_height(child) / 2;
        // LV_LOG_USER("%s,1 parent y1:%d height:%d child y1:%d height:%d\n", __func__, cont_a.y1,
        //             lv_area_get_height(&cont_a), child_a.y1, lv_area_get_height(&child_a));
        // LV_LOG_USER("%s,2 parent height:%d child y1:%d %d %d height:%d\n", __func__, lv_obj_get_height(cont),
        //             lv_obj_get_y(child), cont->coords.y1, child->coords.y1, lv_obj_get_height(child));
        if (e->code == LV_EVENT_SCROLL_END && lv_cycle_roller->cycle_flag > 0)
        {
            /* 子界面的坐标与父界面的坐标相等时，说明当前界面在父界面中显示 */
            if (child_y_center == cont_y_center)
            {
                /* 当前显示界面的索引 */
                // lv_coord_t current_btn_index = lv_obj_get_index(child);
                // LV_LOG_USER("%s,code:%d current_btn_index:%d mid_btn_index:%d \n", __func__, e->code, current_btn_index,
                //             mid_btn_index);
                /* 判断界面移动的数数据，并将当前界面的索引改为中间位置 */
                cycle_roller_to_index(cont, i, 0);
                /* 保证界面居中显示 */
                if (lv_cycle_roller->cb != NULL)
                    lv_cycle_roller->cb(child, 1, 1);
            }
            else
            {
                if (lv_cycle_roller->cb != NULL)
                    lv_cycle_roller->cb(child, 0, 0);
            }
        }
        else
        {
            if (child_y_center <= cont_y_center + lv_area_get_height(&child_a) / 2 &&
                child_y_center >= cont_y_center - lv_area_get_height(&child_a) / 2)
            {
                if (e->code == LV_EVENT_SCROLL_END)
                {
                    lv_cycle_roller->selected_index = i;
                    LV_LOG_USER("%s,selected_index:%d\n", __func__, lv_cycle_roller->selected_index);
                    if (lv_cycle_roller->cb != NULL)
                        lv_cycle_roller->cb(child, 1, 1);
                }
                else
                {
                    // cycle_roller_to_index(cont, i, 0);
                    if (lv_cycle_roller->cb != NULL)
                        lv_cycle_roller->cb(child, 1, 0);
                }
            }
            else
            {
                if (lv_cycle_roller->cb != NULL)
                {
                    lv_cycle_roller->cb(child, 0, 0);
                }
            }
        }
    }
    // LV_LOG_USER("%s,code:%d quit\n", __func__, e->code);
}
/**********************************************************************
**函数名称: roller_scroll_x_event
**函数说明: 循环滚轮的事件
**输入参数: none
**输出参数: none
**返 回 值: none
************************************************************************/
static void roller_scroll_x_event(lv_event_t *e)
{
    lv_obj_t *cont = lv_event_get_target(e); // 获取事件的初始对象
    lv_cycle_roller_t *lv_cycle_roller = (lv_cycle_roller_t *)cont;
    if (LV_FLEX_FLOW_ROW != (lv_cycle_roller->flex_flow & 0x01))
        return;
    if (lv_cycle_roller != NULL && lv_cycle_roller->mask_flag)
        mask_event_cb(e);
    if (e->code != LV_EVENT_SCROLL_END && e->code != LV_EVENT_SCROLL)
        return;

    // LV_LOG_USER("%s,code:%d enter\n", __func__, e->code);
    /* 获取事件的事件代码 */
    /* 判断是否在滚动中 */
    if (lv_obj_is_scrolling(cont) && e->code == LV_EVENT_SCROLL_END)
    {
        // LV_LOG_ERROR("%s,lv_obj_is_scrolling code:%d\n", __func__, e->code);
        return;
    }
    lv_coord_t child_cnt = lv_obj_get_child_cnt(cont); // 获取子界面的数量
    // lv_coord_t mid_btn_index = (child_cnt - 1) / 2;    // 中间界面的位置

    /* 获取父对象x轴的中心坐标值 */
    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);                                      // 将cont对象的坐标复制到cont_a
    lv_coord_t cont_x_center = cont_a.x1 + lv_area_get_width(&cont_a) / 2; // 获取界面的宽像素大小/2
    // lv_coord_t cont_y_center = lv_obj_get_height(cont) / 2;
    /* 遍历子界面 */
    for (lv_coord_t i = 0; i < child_cnt; i++)
    {
        lv_obj_t *child = lv_obj_get_child(cont, i); // 通过索引获取子对象

        /* 获取子对象x轴的中心坐标值 */
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);
        lv_coord_t child_x_center = child_a.x1 + lv_area_get_width(&child_a) / 2; // 获取界面中按钮宽像素值的大小/2
        // lv_coord_t child_y_center = lv_obj_get_y(child) + lv_obj_get_height(child) / 2;
        // LV_LOG_USER("%s,1 parent y1:%d height:%d child y1:%d height:%d\n", __func__, cont_a.y1,
        //             lv_area_get_height(&cont_a), child_a.y1, lv_area_get_height(&child_a));
        // LV_LOG_USER("%s,2 parent height:%d child y1:%d %d %d height:%d\n", __func__, lv_obj_get_height(cont),
        //             lv_obj_get_y(child), cont->coords.y1, child->coords.y1, lv_obj_get_height(child));
        if (e->code == LV_EVENT_SCROLL_END && lv_cycle_roller->cycle_flag > 0)
        {
            /* 子界面的坐标与父界面的坐标相等时，说明当前界面在父界面中显示 */
            if (child_x_center == cont_x_center)
            {

                /* 当前显示界面的索引 */
                // lv_coord_t current_btn_index = lv_obj_get_index(child);
                // LV_LOG_USER("%s,code:%d current_btn_index:%d mid_btn_index:%d \n", __func__, e->code, current_btn_index,
                //             mid_btn_index);
                /* 判断界面移动的数数据，并将当前界面的索引改为中间位置 */
                cycle_roller_to_index(cont, i, 0);
                /* 保证界面居中显示 */
                if (lv_cycle_roller->cb != NULL)
                    lv_cycle_roller->cb(child, 1, 1);
            }
            else
            {
                if (lv_cycle_roller->cb != NULL)
                    lv_cycle_roller->cb(child, 0, 0);
            }
        }
        else
        {
            if (child_x_center <= cont_x_center + lv_area_get_width(&child_a) / 2 &&
                child_x_center >= cont_x_center - lv_area_get_width(&child_a) / 2)
            {
                if (e->code == LV_EVENT_SCROLL_END)
                {
                    lv_cycle_roller->selected_index = i;
                    LV_LOG_USER("%s,selected_index:%d\n", __func__, lv_cycle_roller->selected_index);
                    if (lv_cycle_roller->cb != NULL)
                        lv_cycle_roller->cb(child, 1, 1);
                }
                else
                {
                    if (lv_cycle_roller->cb != NULL)
                        lv_cycle_roller->cb(child, 1, 0);
                }
            }
            else
            {
                if (lv_cycle_roller->cb != NULL)
                {
                    lv_cycle_roller->cb(child, 0, 0);
                }
            }
        }
    }
    // LV_LOG_USER("%s,code:%d quit\n", __func__, e->code);
}

static void lv_cycle_roller_constructor(const lv_obj_class_t *class_p, lv_obj_t *obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");
    LV_LOG_USER("%s", __func__);
    lv_cycle_roller_t *cycle_roller = (lv_cycle_roller_t *)obj;

    // LV_LOG_USER("%s,cycle_flag:%d,mask_flag:%d\n", __func__, cycle_roller->cycle_flag, cycle_roller->mask_flag);
    cycle_roller->flex_flow = LV_FLEX_FLOW_COLUMN;
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scroll_dir(obj, LV_DIR_VER);               // 设置Scroll的允许方向direction：垂直方向
    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_CENTER); // 捕捉Cont Y方向的子对象，将他们与Container中心对齐

    // 2.设置样式
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF); // 隐藏scrollbar
    lv_obj_set_style_pad_all(obj, 5, 0);
    lv_obj_set_style_pad_row(obj, 10, 0);
    lv_obj_set_style_pad_column(obj, 10, 0);

    LV_TRACE_OBJ_CREATE("finished");
}

static void lv_cycle_roller_destructor(const lv_obj_class_t *class_p, lv_obj_t *obj)
{
    LV_UNUSED(class_p);
    LV_LOG_USER("%s", __func__);
    // lv_cycle_roller_t *cycle_roller = (lv_cycle_roller_t *)obj;
}

static void lv_cycle_roller_event(const lv_obj_class_t *class_p, lv_event_t *e)
{
    LV_UNUSED(class_p);
    lv_res_t res;

    /*Call the ancestor's event handler*/
    res = lv_obj_event_base(MY_CLASS, e);
    if (res != LV_RES_OK)
        return;

    // lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t *obj = lv_event_get_target(e);

    roller_scroll_y_event(e);
    roller_scroll_x_event(e);
}
