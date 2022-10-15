/**
 * @file lv_100ask_demo_layer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../lv_test.h"


/*********************
 *      DEFINES
 *********************/


 /**********************
 *      TYPEDEFS
 **********************/


/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * bottom_bg;
static lv_obj_t * calendar;
static lv_obj_t * calendar_header;


static void ta_event_cb(lv_event_t * e);
static void birthday_event_cb(lv_event_t * e);
static void calendar_event_cb(lv_event_t * e);


static void birthday_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);

    if(code == LV_EVENT_FOCUSED) {
        if(lv_indev_get_type(lv_indev_get_act()) == LV_INDEV_TYPE_POINTER) {
            if(calendar == NULL) {
                lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);                         // 使能 lv_layer_top 点击
                calendar = lv_calendar_create(lv_layer_top());                                  // 在 lv_layer_top 层上创建日历对象
                lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);                          // 设置对象透明度
                lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0); // 设置对象颜色
                lv_obj_set_size(calendar, 300, 300);                                            // 设置对象大小
                lv_calendar_set_showed_date(calendar, 1990, 01);                                // 给日历的指定打开时显示的日期
                lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 30);                                 // 设置对象对齐、偏移
                lv_obj_add_event_cb(calendar, calendar_event_cb, LV_EVENT_VALUE_CHANGED, ta);   // 给对象分配事件

                calendar_header = lv_calendar_header_dropdown_create(calendar); // 创建一个包含 2 个下拉列表的标题：一个用于年份，另一个用于月份。
            }
        }
    }
}

static void calendar_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);    // 获取事件代码
    lv_obj_t * ta = lv_event_get_user_data(e);      // 获取在对象上注册事件时传递的 user_data
    LV_LOG_USER("%s,code:%d",__func__,code);
    // 判断事件类型
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_calendar_date_t d;

        lv_calendar_get_pressed_date(e->current_target, &d);        // 获取当前选中的日期
        lv_snprintf(buf, sizeof(buf), "%02d.%02d.%d", d.day, d.month, d.year);
        lv_textarea_set_text(ta, buf);  // 在文本区域展示日期信息
        lv_obj_del(calendar);           // 删除对象及其所有子对象
        // lv_obj_del(calendar_header);    // 删除对象及其所有子对象
        calendar = NULL;
        calendar_header = NULL;
        LV_LOG_USER("%s,2",__func__);
        lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);   // 清除标志
        lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);  // 设置透明度
    }
    else if (code == LV_EVENT_CLICKED)
    {
        lv_obj_del(calendar);           // 删除对象及其所有子对象
        // lv_obj_del(calendar_header);    // 删除对象及其所有子对象
        calendar = NULL;
        calendar_header = NULL;
        lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);   // 清除标志
        lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);  // 设置透明度
    }
}


static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);    // 获取事件代码
    lv_obj_t * ta = lv_event_get_target(e);         // 获取事件最初指向的对象
    lv_obj_t * kb = lv_event_get_user_data(e);      // 获取在对象上注册事件时传递的 user_data

    // 判断事件类型(当焦点在键盘上时)
    if(code == LV_EVENT_FOCUSED) {
        //获取输入设备的类型
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD) {
            lv_keyboard_set_textarea(kb, ta);                                   // 为键盘分配一个文本区域
            lv_obj_set_style_max_height(kb, LV_HOR_RES * 2 / 3, 0);             // 设置键盘的最高宽度为屏幕高度的一半
            lv_obj_update_layout(bottom_bg);                                    // 确保尺寸更新
            lv_obj_set_height(bottom_bg, LV_VER_RES - lv_obj_get_height(kb));   // 设置对象的高度，当唤出键盘时应该偏移背景
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);                          // 清除标志
            lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);                   // 滚动到一个对象，直到它在其父对象上可见。对父母的父母做同样的事情，依此类推。即使对象具有嵌套的可滚​​动父对象，它也会滚动到视图中
        }
    }
    // 判断事件类型(当焦点不在键盘上时)
    else if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_set_height(bottom_bg, LV_VER_RES);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
    // 判断事件类型(当键盘点击确定或取消键)
    else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_set_height(bottom_bg, LV_VER_RES);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);            // 标志为隐藏对象(使对象隐藏，就像它根本不存在一样)
        lv_obj_clear_state(e->target, LV_STATE_FOCUSED);    // 删除对象的一种或多种状态。其他状态位将保持不变。
        lv_indev_reset(NULL, e->target);                    // 忘记最后一次点击的对象，使其再次成为可关注的对象
    }
}

void lv_100ask_demo_layer(void)
{
    static lv_style_t style_title;
    static lv_style_t style_text_muted;

    bottom_bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(bottom_bg, LV_HOR_RES, LV_VER_RES);

    // 设置样式
    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, &lv_font_montserrat_20);   // 字体

    lv_style_init(&style_text_muted);
    lv_style_set_text_opa(&style_text_muted, LV_OPA_50);    // 文本透明度

    //  创建键盘
    lv_obj_t * kb = lv_keyboard_create(lv_scr_act());
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);    // 标志为隐藏对象(使对象隐藏，就像它根本不存在一样)

    // 创建对象并初始化
    lv_obj_t * title = lv_label_create(bottom_bg);
    lv_label_set_text(title, "Your profile");                       // 为标签设置新文本
    lv_obj_add_style(title, &style_title, 0);                       // 给对象添加样式
    lv_obj_align(bottom_bg, LV_ALIGN_TOP_LEFT, 0, 0);               // 更改对象的对齐方式并设置新坐标。

    lv_obj_t * user_name_label = lv_label_create(bottom_bg);
    lv_label_set_text(user_name_label, "User name");                // 为标签设置新文本
    lv_obj_add_style(user_name_label, &style_text_muted, 0);        // 给对象添加样式
    lv_obj_align(user_name_label, LV_ALIGN_TOP_LEFT, 0, 30);        // 更改对象的对齐方式并设置新坐标

    lv_obj_t * user_name = lv_textarea_create(bottom_bg);
    lv_textarea_set_one_line(user_name, true);                      // 单行模式
    lv_textarea_set_placeholder_text(user_name, "Your name");       // 设置文本区域的占位符文本(输入框提示文本内容)
    lv_obj_add_event_cb(user_name, ta_event_cb, LV_EVENT_ALL, kb);  // 为对象分配事件及其处理函数
    lv_obj_align(user_name, LV_ALIGN_TOP_LEFT, 0, 50);              // 更改对象的对齐方式并设置新坐标

    lv_obj_t * password_label = lv_label_create(bottom_bg);
    lv_label_set_text(password_label, "Password");                  // 为标签设置新文本
    lv_obj_align(password_label, LV_ALIGN_TOP_MID, -95, 30);        // 更改对象的对齐方式并设置新坐标。
    lv_obj_add_style(password_label, &style_text_muted, 0);         // 给对象添加样式

    lv_obj_t * password = lv_textarea_create(bottom_bg);
    lv_textarea_set_one_line(password, true);                       // 单行模式
    lv_textarea_set_password_mode(password, true);                  // 密码模式
    lv_textarea_set_placeholder_text(password, "Min. 8 chars.");    // 设置文本区域的占位符文本(输入框提示文本内容)
    lv_obj_align(password, LV_ALIGN_TOP_MID, 0, 50);                // 更改对象的对齐方式并设置新坐标
    lv_obj_add_event_cb(password, ta_event_cb, LV_EVENT_ALL, kb);   // 为对象分配事件及其处理函数

    lv_obj_t * gender_label = lv_label_create(bottom_bg);
    lv_label_set_text(gender_label, "Gender");                      // 为标签设置新文本
    lv_obj_add_style(gender_label, &style_text_muted, 0);           // 给对象添加样式
    lv_obj_align(gender_label, LV_ALIGN_TOP_LEFT, 0, 100);          // 更改对象的对齐方式并设置新坐标

    lv_obj_t * gender = lv_dropdown_create(bottom_bg);
    lv_dropdown_set_options_static(gender, "Male\nFemale\nOther");  // 设置下拉列表选项
    lv_obj_align(gender, LV_ALIGN_TOP_LEFT, 0, 120);                // 更改对象的对齐方式并设置新坐标

    lv_obj_t * birthday_label = lv_label_create(bottom_bg);
    lv_label_set_text(birthday_label, "Birthday");                  // 为标签设置新文本
    lv_obj_align(birthday_label, LV_ALIGN_TOP_MID, -95, 100);       // 更改对象的对齐方式并设置新坐标
    lv_obj_add_style(birthday_label, &style_text_muted, 0);         // 给对象添加样式

    lv_obj_t * birthdate = lv_textarea_create(bottom_bg);
    lv_textarea_set_one_line(birthdate, true);                      // 单行模式
    lv_obj_align(birthdate, LV_ALIGN_TOP_MID, 0, 120);              // 更改对象的对齐方式并设置新坐标
    lv_obj_add_event_cb(birthdate, birthday_event_cb, LV_EVENT_ALL, NULL);  // 为对象分配事件及其处理函数
}
