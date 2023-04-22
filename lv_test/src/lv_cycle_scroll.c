#include "lv_cycle_scroll.h"

static cycle_scroll_child_cb cycle_scroll_child_func;

/**********************************************************************
**函数名称: cycle_scroll_change
**函数说明: 循环滚轮修改中心索引
**输入参数: cycle_scroll：循环滚轮对象 cur_index：目标中心索引
**输出参数: none
**返 回 值: none
************************************************************************/
void cycle_scroll_change(lv_obj_t * cycle_scroll, const int cur_index)
{
    uint32_t child_cnt = lv_obj_get_child_cnt(cycle_scroll); // 获取子界面的数量
    int mid_btn_index = (child_cnt - 1) / 2; // 如果界面为偶数，将中间数向下取整的界面设置为中间界面

    int roll_direction = cur_index - mid_btn_index; // 确定滚动方向
    /* 通过循环将指定界面移到中心位置 */
    for(int i = 0; i < LV_ABS(roll_direction); ++i) {
        if(roll_direction < 0) {
            lv_obj_move_to_index(lv_obj_get_child(cycle_scroll, child_cnt - 1), 0); // 将最后一个界面移至第一个界面
        } else {
            lv_obj_move_to_index(lv_obj_get_child(cycle_scroll, 0), child_cnt - 1); // 将第一个界面移至最后一个界面
        }
    }
    /*当按钮数为偶数时，确保按钮居中*/
    lv_obj_scroll_to_view(lv_obj_get_child(cycle_scroll, mid_btn_index), LV_ANIM_OFF);
}
/**********************************************************************
**函数名称: scroll_end_event
**函数说明: 循环滚轮的事件
**输入参数: none
**输出参数: none
**返 回 值: none
************************************************************************/
static void scroll_end_event(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e); // 获取事件的初始对象
    if(e->code != LV_EVENT_SCROLL_END && e->code != LV_EVENT_SCROLL) return;

    // LV_LOG_USER("%s,code:%d enter\n", __func__, e->code);
    /* 获取事件的事件代码 */
    /* 判断是否在滚动中 */
    // if(lv_obj_is_scrolling(cont)) {
    //     LV_LOG_USER("%s,lv_obj_is_scrolling code:%d\n", __func__, e->code);
    //     return;
    // }
    lv_coord_t child_cnt     = lv_obj_get_child_cnt(cont); // 获取子界面的数量
    lv_coord_t mid_btn_index = (child_cnt - 1) / 2;        // 中间界面的位置

    /* 获取父对象y轴的中心坐标值 */
    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);                                       // 将cont对象的坐标复制到cont_a
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2; // 获取界面的宽像素大小/2
    // lv_coord_t cont_y_center = lv_obj_get_height(cont) / 2;
    /* 遍历子界面 */
    for(lv_coord_t i = 0; i < child_cnt; i++) {
        lv_obj_t * child = lv_obj_get_child(cont, i); // 通过索引获取子对象

        /* 获取子对象y轴的中心坐标值 */
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);
        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2; // 获取界面中按钮宽像素值的大小/2
        // lv_coord_t child_y_center = lv_obj_get_y(child) + lv_obj_get_height(child) / 2;
        // LV_LOG_USER("%s,1 parent y1:%d height:%d child y1:%d height:%d\n", __func__, cont_a.y1,
        //             lv_area_get_height(&cont_a), child_a.y1, lv_area_get_height(&child_a));
        // LV_LOG_USER("%s,2 parent height:%d child y1:%d %d %d height:%d\n", __func__, lv_obj_get_height(cont),
        //             lv_obj_get_y(child), cont->coords.y1, child->coords.y1, lv_obj_get_height(child));
        if(e->code == LV_EVENT_SCROLL_END) {
            /* 子界面的坐标与父界面的坐标相等时，说明当前界面在父界面中显示 */
            if(child_y_center == cont_y_center) {

                /* 当前显示界面的索引 */
                lv_coord_t current_btn_index = lv_obj_get_index(child);
                LV_LOG_USER("%s,code:%d current_btn_index:%d mid_btn_index:%d \n", __func__, e->code, current_btn_index,
                            mid_btn_index);
                /* 判断界面移动的数数据，并将当前界面的索引改为中间位置 */
                cycle_scroll_change(cont, current_btn_index);
                /* 保证界面居中显示 */
                if(cycle_scroll_child_func != NULL) cycle_scroll_child_func(child, 1);
            } else {
                if(cycle_scroll_child_func != NULL) cycle_scroll_child_func(child, 0);
            }
        } else {
            if(child_y_center <= cont_y_center + lv_area_get_height(&child_a) / 2 &&
               child_y_center >= cont_y_center - lv_area_get_height(&child_a) / 2) {
                if(cycle_scroll_child_func != NULL) cycle_scroll_child_func(child, 1);
            } else {
                if(cycle_scroll_child_func != NULL) {
                    cycle_scroll_child_func(child, 0);
                }
            }
        }
    }
    // LV_LOG_USER("%s,code:%d quit\n", __func__, e->code);
}
/**********************************************************************
**函数名称: lv_cycle_scroll_create
**函数说明: 循环滚轮创建
**输入参数: parent：父对象 width：宽 height：高 flow：布局方向 cycle_scroll_child_cb：子对象选中和不选中的回调
**输出参数: none
**返 回 值: 循环滚轮对象
************************************************************************/
lv_obj_t * lv_cycle_scroll_create(lv_obj_t * parent, int width, int height, lv_flex_flow_t flow,
                                  cycle_scroll_child_cb cb)
{
    cycle_scroll_child_func = cb;
    lv_obj_t * cont         = lv_obj_create(parent); // 在屏幕上创建一个container
    lv_obj_set_size(cont, width, height);            // 设置cont的尺寸
    lv_obj_set_flex_flow(cont, flow);                // 设置cont的子级的layout: 弹性布局
    // lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    // 1.添加事件
    lv_obj_add_event_cb(cont, scroll_end_event, LV_EVENT_ALL, NULL); // 给cont添加event，事件的回调函数
    // 2.设置样式
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);               // 设置Scroll的允许方向direction：垂直方向
    lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER); // 捕捉Cont Y方向的子对象，将他们与Container中心对齐
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF); // 隐藏scrollbar

    lv_obj_set_style_bg_opa(cont, LV_OPA_100, 0);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_pad_all(cont, 5, 0);
    lv_obj_set_style_pad_row(cont, 10, 0);
    lv_obj_set_style_pad_column(cont, 10, 0);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    return cont;
}
/**********************************************************************
**函数名称: lv_cycle_scroll_get_selected_index
**函数说明: 获取循环滚轮中心索引
**输入参数: cycle_scroll：循环滚轮对象
**输出参数: none
**返 回 值: 滚轮中心索引
************************************************************************/
int lv_cycle_scroll_get_selected_index(lv_obj_t * cycle_scroll)
{
    lv_coord_t child_cnt = lv_obj_get_child_cnt(cycle_scroll); // 获取子界面的数量
    return (child_cnt - 1) / 2;                                // 中间界面的位置
}
/**********************************************************************
**函数名称: lv_cycle_scroll_get_selected
**函数说明: 获取循环滚轮中心索引对象
**输入参数: cycle_scroll：循环滚轮对象
**输出参数: none
**返 回 值: 滚轮中心索引对象
************************************************************************/
lv_obj_t * lv_cycle_scroll_get_selected(lv_obj_t * cycle_scroll)
{
    return lv_obj_get_child(cycle_scroll, lv_cycle_scroll_get_selected_index(cycle_scroll));
}
#if 1
static void child_select_test(lv_obj_t * child, int select)
{
    if(select) {
        lv_obj_add_state(child, LV_STATE_CHECKED);
        lv_obj_add_state(lv_obj_get_child(child, 0), LV_STATE_CHECKED);
        lv_obj_center(lv_obj_get_child(child, 0));
    } else {
        lv_obj_clear_state(child, LV_STATE_CHECKED);
        lv_obj_clear_state(lv_obj_get_child(child, 0), LV_STATE_CHECKED);
        lv_obj_center(lv_obj_get_child(child, 0));
    }
}
void cycle_scroll_test(lv_obj_t * parent)
{
    lv_obj_t * cont = lv_cycle_scroll_create(parent, 280, 240, LV_FLEX_FLOW_COLUMN, child_select_test);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    /// 子对象
    uint32_t i;
    /// 在container上创建若干个button
    for(i = 0; i < 10; i++) {
        lv_obj_t * btn = lv_obj_create(cont); // 在container上创建button
        lv_obj_set_width(btn, lv_pct(100)); // 设置button的width = lv_pct(100); //pct : percentage 相对于父亲的宽度100%
        lv_obj_set_height(btn, 60);
        lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
        // lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor), LV_STATE_CHECKED);
        // lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
        /// 孙对象
        lv_obj_t * label = lv_label_create(btn); // 在button上创建一个label（标签）
        lv_obj_set_style_text_color(label, lv_color_hex(themesTextColor), 0);
        lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
        lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_40, LV_STATE_CHECKED);
        lv_label_set_text_fmt(label, "Button %d", i); // 动态设置label的文本内容  fmt: format（格式）
        lv_obj_center(label);
    }
    cycle_scroll_change(cont, 0);
}
#endif