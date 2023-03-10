

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"
/*********************
 *      DEFINES
 *********************/
static lv_obj_t *label_version;
/**********************
 *  STATIC VARIABLES
 **********************/
static void btn_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    switch (user_data)
    {
    case 0:
        break;
    }
}
void lv_page_update_visible(const int visible)
{
    char buf[80];
    sprintf(buf, "当前版本 %s", get_attr_value_string("ComSWVersion"));
    lv_label_set_text(label_version, buf);
}
static void scroll_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    //>>>>>>>>>>>-----------------<!>---Link
    /// E1.----------------------------得到容器
    lv_obj_t *cont = lv_event_get_target(e); // 通过绑定了该事件的对象，来获取这个对象

    /// E2.---------------------------通过一定算法，得到容器的y轴中心位置（目的：用于后面与子元素按钮进行y轴中心偏差比较）
    lv_area_t cont_a;                 // 区域 cont_area
    lv_obj_get_coords(cont, &cont_a); // 将cont的坐标赋值给cont_a (将cont_a约束为container的大小（200 * 200)  coords：坐标(x1,y1); (x2,y1); (x1,y2); (x2,y2)

    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2; // 获取Container的y轴中心

    lv_coord_t r = lv_obj_get_height(cont) * 7 / 10; // 200*7 / 10 = 140

    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont); // child_cnt : child count 儿子数量 获取container里面元素个数

    /// E3.------------------遍历容器里面的子元素（按钮），以便操作这些元素的属性（用户垂直滚动时候，按钮发生水平偏移，并且设置不同的透明度）
    for (i = 0; i < child_cnt; i++) // 遍历Buttons
    {
        lv_obj_t *child = lv_obj_get_child(cont, i);                               // 获取container的第i个button
        lv_area_t child_a;                                                         // 创建一个属于儿子区域
        lv_obj_get_coords(child, &child_a);                                        // 将child_a约束为button的大小（200 * 200）
        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2; // 计算得到button的y轴中心

        lv_coord_t diff_y = child_y_center - cont_y_center; // button的y轴中心 - Container的y轴中心 = Button相对于Container的垂直偏差
        diff_y = LV_ABS(diff_y);                            // 对偏差取绝对值（ABS）

        if (diff_y == 0)
            lv_obj_add_state(child, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(child, LV_STATE_CHECKED);
        /// E3.1------------------根据偏差（按钮相对于容器的y方向中心位置）来产生不同的x值
        /*Get the x of diff_y on a circle.*/
        lv_coord_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if (diff_y >= r) // diff_y >= 140
        {
            x = r; // x = 140
        }
        else // diff_y < 140
        {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y; // 140 * 140  - diff_y的平方
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000); /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }
        /// E3.2--------------------根据x值，将button移动x个单位距离，根据r映射出不同的透明度值，设置按钮不同透明度
        /*Translate the item by the calculated X coordinate*/
        // lv_obj_set_style_translate_x(child, x, 0); // 将button 移动 x个单位距离

        /*Use some opacity with larger translations*/
        // lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER); // 通过r的不同值，动态映射创建不透明度值 opa: opacity
        // lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);          // 给按钮应用不透明度值  opa: opacity
    }
}

static void scroll_change(lv_obj_t *cont, int cur_index)
{
    uint32_t child_cnt = lv_obj_get_child_cnt(cont); // 获取子界面的数量
    int mid_btn_index = (child_cnt - 1) / 2;    // 如果界面为偶数，将中间数向下取整的界面设置为中间界面

    int roll_direction = cur_index - mid_btn_index; // 确定滚动方向

    /* 通过循环将指定界面移到中心位置 */
    for (int i = 0; i < LV_ABS(roll_direction); ++i)
    {
        if (roll_direction < 0)
        {
            lv_obj_move_to_index(lv_obj_get_child(cont, child_cnt - 1), 0); // 将最后一个界面的索引更改为 0 （移至第一个界面）
        }
        else
        {
            lv_obj_move_to_index(lv_obj_get_child(cont, 0), child_cnt - 1); // 将第一个界面的索引值改为最大值（移至最后一个界面）
        }
    }
    /*当按钮数为偶数时，确保按钮居中*/
    lv_obj_scroll_to_view(lv_obj_get_child(cont, mid_btn_index), LV_ANIM_OFF); // 滚动到一个对象，直到它在其父对象上可见
}
static void scroll_end_event(lv_event_t *e)
{

    lv_obj_t *cont = lv_event_get_target(e); // 获取事件的初始对象

    /* 获取事件的事件代码 */
    /* 判断是否在滚动中 */
    if (lv_obj_is_scrolling(cont))
    {
        return;
    }

    lv_coord_t child_cnt = lv_obj_get_child_cnt(cont); // 获取子界面的数量
    lv_coord_t mid_btn_index = (child_cnt - 1) / 2;    // 中间界面的位置

    /* 获取父对象y轴的中心坐标值 */
    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);                                       // 将cont对象的坐标复制到cont_a
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2; // 获取界面的宽像素大小/2

    /* 注意，这里的中心显示界面的坐标不在正中心，所以这里加上了差值 */
    // cont_y_center += 69;

    /* 遍历子界面 */
    for (lv_coord_t i = 0; i < child_cnt; i++)
    {
        lv_obj_t *child = lv_obj_get_child(cont, i); // 通过索引获取子对象

        /* 获取子对象y轴的中心坐标值 */
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);
        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2; // 获取界面中按钮宽像素值的大小/2
        /* 子界面的坐标与父界面的坐标相等时，说明当前界面在父界面中显示 */
        if (child_y_center == cont_y_center)
        {

            /* 当前显示界面的索引 */
            lv_coord_t current_btn_index = lv_obj_get_index(child);
            LV_LOG_USER("%s,code:%d current_btn_index:%d mid_btn_index:%d\n", __func__, e->code, current_btn_index, mid_btn_index);
            /* 判断界面移动的数数据，并将当前界面的索引改为中间位置 */
            /* 因为是在滑动结束后实现的，建议界面较多的情况下使用此方式，当界面较少，一次滑动太多界面时，容易滑倒边界出现卡顿现象 */
            // lv_coord_t move_btn_quantity = LV_ABS(current_btn_index - mid_btn_index);
            // for (lv_coord_t j = 0; j < move_btn_quantity; j++)
            // {
            //     /* 向右滑动 */
            //     if (current_btn_index < mid_btn_index)
            //     {
            //         lv_obj_move_to_index(lv_obj_get_child(cont, child_cnt - 1), 0); // 将最后一个界面索引改为第一个界面
            //         // lv_obj_scroll_to_view(lv_obj_get_child(cont, mid_btn_index), LV_ANIM_OFF); // lv_obj_get_child 通过子索引获取对象的子对象
            //     }
            //     /* 向左滑动 */
            //     if (current_btn_index > mid_btn_index)
            //     {
            //         lv_obj_move_to_index(lv_obj_get_child(cont, 0), child_cnt - 1); // 将第一个界面的索引值改为最后一个界面
            //         // lv_obj_scroll_to_view(lv_obj_get_child(cont, mid_btn_index), LV_ANIM_OFF); // lv_obj_get_child 通过子索引获取对象的子对象
            //     }
            // }
            // lv_obj_scroll_to_view(lv_obj_get_child(cont, mid_btn_index), LV_ANIM_OFF);

            scroll_change(cont, current_btn_index);
            /* 保证界面居中显示 */
            // lv_obj_set_style_translate_y(lv_obj_get_child(cont, mid_btn_index), 0, 0);
            // break;
            lv_obj_add_state(child, LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_clear_state(child, LV_STATE_CHECKED);
        }
    }
    LV_LOG_USER("%s,code:%d quit\n", __func__, e->code);
}
static void lv_example_scroll(lv_obj_t *parent)
{
    /// 父对象
    lv_obj_t *cont = lv_obj_create(parent);          // 在屏幕上创建一个container
    lv_obj_set_size(cont, 250, 250);                 // 设置cont的尺寸： w200, h200  （正方形）
    lv_obj_align(cont, LV_ALIGN_LEFT_MID, 0, 0);     // 让cont垂直水平居中（相对于父级）
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN); // 设置cont的子级的layout: 弹性布局弹性流（flex-flow）(布局+滚动功能)
    // lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    /// 对象的特性
    // 1.添加事件 // Link---<!>----------------------------------->>>>>>>>>>>>
    lv_obj_add_event_cb(cont, scroll_end_event, LV_EVENT_SCROLL_END, NULL); // 给cont添加event，事件的回调函数、事件类型（Scroll）
    // 2.设置样式
    // lv_obj_set_style_radius(cont, LV_RADIUS_CIRCLE, 0);     // 设置矩形圆角 LV_RADIUS_CIRCLE：圆角最大化
    // lv_obj_set_style_clip_corner(cont, true, 0);            // 儿子超出部分隐藏
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);                // 设置Scroll的允许方向direction：垂直方向
    lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER);  // 捕捉Cont Y方向的子对象，将他们与Container中心对齐 ； snap ：捕获；捕捉
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF); // 隐藏scrollbar

    // lv_obj_set_style_bg_opa(cont, LV_OPA_COVER,0);
    lv_obj_set_style_pad_all(cont, 10, 0);
    lv_obj_set_style_pad_row(cont, 16, 0);
    lv_obj_set_style_line_color(cont, lv_color_hex(themesTextColor), 0);
    lv_obj_set_style_line_width(cont, 16, 0);
    // lv_obj_set_style_pad_column(cont, 16,0);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    // lv_obj_add_flag(cont, LV_OBJ_FLAG_SCROLL_ONE);
    /// 子对象
    uint32_t i;
    /// 在container上创建若干个button
    for (i = 0; i < 10; i++)
    {
        lv_obj_t *btn = lv_btn_create(cont); // 在container上创建button
        lv_obj_set_width(btn, lv_pct(100));  // 设置button的width = lv_pct(100); //pct : percentage 相对于父亲的宽度100%
        lv_obj_set_height(btn, 50);
        lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
        lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor), LV_STATE_CHECKED);
        lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
        /// 孙对象
        lv_obj_t *label = lv_label_create(btn);       // 在button上创建一个label（标签）
        lv_label_set_text_fmt(label, "Button %d", i); // 动态设置label的文本内容  fmt: format（格式）
    }

    /*---------------------------------------- 指定中心显示界面 ----------------------------------------*/
    scroll_change(cont, 0);

    /*Update the buttons position manually for first*/ // 首次手动更新按钮的位置
    // lv_event_send(cont, LV_EVENT_SCROLL, NULL);        // TODO

    /*Be sure the fist button is in the middle*/ // 确保第一个按钮处于中间
    // lv_obj_scroll_to_view(lv_obj_get_child(cont, 0), LV_ANIM_OFF); // 第一个按钮是否以滚动动画，滚动到指定位置（默认位置）
}

void lv_page_update_create(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);

    lv_obj_t *img = lv_img_create(page);
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 40);
    lv_img_set_src(img, themesImagesPath "icon_logo.png");

    lv_obj_t *btn = lv_btn_create(page);
    lv_obj_set_size(btn, 440, 50);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 140);

    label_version = lv_label_create(btn);
    lv_obj_set_style_text_font(label_version, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_obj_set_style_text_color(label_version, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_version, "当前版本 1.0.1");
    lv_obj_align(label_version, LV_ALIGN_CENTER, -10, 0);

    img = lv_img_create(btn);
    lv_img_set_src(img, themesImagesPath "icon_more.png");
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -20, 0);

    btn = lv_btn_create(page);
    lv_obj_set_size(btn, 180, 50);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -35);
    lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, (void *)0);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0), 0);
    lv_label_set_text(label, "检查更新");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    static lv_style_t style;

    lv_style_init(&style);
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_bg_color(&style, lv_color_hex(0xff0000));
    lv_style_set_bg_grad_color(&style, lv_color_hex(0x0000ff));
    lv_style_set_bg_grad_dir(&style, LV_GRAD_DIR_VER);
    lv_style_set_bg_main_stop(&style, 0);
    lv_style_set_bg_grad_stop(&style, 255);

    // lv_obj_t *btn_test = lv_btn_create(page);
    // lv_obj_set_size(btn_test, 100, 300);
    // lv_obj_add_style(btn_test, &style, LV_PART_MAIN);
    // lv_obj_align(btn_test, LV_ALIGN_TOP_RIGHT, 0, 0);

    // lv_obj_t *obj = lv_btn_create(btn_test);
    // lv_obj_set_size(obj, 100, 100);
    // lv_obj_set_style_bg_color(obj, lv_color_hex(themesTextColor2), 0);
    // lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
    // lv_obj_align(btn_test, LV_ALIGN_TOP_RIGHT, -20, 0);

    // lv_obj_t *arc = lv_arc_create(page);
    // lv_obj_set_size(arc, 240, 240);
    // lv_arc_set_rotation(arc, 270);
    // lv_arc_set_bg_angles(arc, 0, 360);
    // lv_arc_set_value(arc, 50);
    // lv_obj_set_style_arc_color(arc, lv_color_hex(themesTextColor), LV_PART_INDICATOR);
    // lv_obj_set_style_arc_width(arc, 20, LV_PART_INDICATOR);
    // lv_obj_align(arc, LV_ALIGN_TOP_LEFT, 0, 0);
    // lv_obj_add_style(arc, &style, LV_PART_INDICATOR);

    lv_example_scroll(page);
}
