

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static recipe_t *cur_recipe = NULL;
static int cook_pos = 0;
static lv_obj_t *smart_recipes[2];
static lv_obj_t *pos_btn[2];
/**********************
 *  STATIC VARIABLES
 **********************/
static void lv_get_recipes(lv_obj_t *parent, unsigned char recipeType, char cookPos);
static void scroll_change(lv_obj_t *cont, int cur_index);
void recipe_cook_start(recipe_t *recipe, const int reserve_time)
{
    if (recipe == NULL)
    {
        LV_LOG_ERROR("recipe data is NULL\n");
        return;
    }
    if (recipe->cookPos == 0)
    {
        steamoven_t steamoven = {0};
        steamoven.cookId = recipe->recipeid;
        strcpy(steamoven.cookName, recipe->dishName);

        cJSON *root = cJSON_Parse(recipe->cookSteps);
        if (root == NULL)
            return;
        int arraySize = cJSON_GetArraySize(root);
        if (arraySize == 0)
        {
            LV_LOG_ERROR("attr arraySize is 0\n");
            goto fail;
        }
        steamoven.attr_len = arraySize;
        cJSON *arraySub;
        for (int i = 0; i < arraySize; i++)
        {
            arraySub = cJSON_GetArrayItem(root, i);
            if (arraySub == NULL)
                continue;
            cJSON *mode = cJSON_GetObjectItem(arraySub, "mode");
            cJSON *temp = cJSON_GetObjectItem(arraySub, "temp");
            cJSON *time = cJSON_GetObjectItem(arraySub, "time");
            steamoven.attr[i].mode = mode->valueint;
            steamoven.attr[i].temp = temp->valueint;
            steamoven.attr[i].time = time->valueint;
        }
        steamoven.orderTime = reserve_time;
        set_cook_toServer(&steamoven);
    fail:
        cJSON_Delete(root);
    }
    else
    {
    }
}
static void recipe_click_checked(lv_obj_t *child, unsigned char checked)
{
    if (checked)
    {
        lv_obj_set_size(child, 184, 258);
        lv_obj_t *child0 = lv_obj_get_child(child, 0);
        lv_obj_t *child0_0 = lv_obj_get_child(child0, 0);
        lv_obj_t *child0_0_0 = lv_obj_get_child(child0_0, 0);
        lv_obj_add_state(child, LV_STATE_CHECKED);
        lv_obj_add_state(child0, LV_STATE_CHECKED);
        lv_obj_add_state(child0_0_0, LV_STATE_CHECKED);
        cur_recipe = child->user_data;
    }
    else
    {
        lv_obj_set_size(child, 164, 210);
        lv_obj_t *child0 = lv_obj_get_child(child, 0);
        lv_obj_t *child0_0 = lv_obj_get_child(child0, 0);
        lv_obj_t *child0_0_0 = lv_obj_get_child(child0_0, 0);
        lv_obj_clear_state(child, LV_STATE_CHECKED);
        lv_obj_clear_state(child0, LV_STATE_CHECKED);
        lv_obj_clear_state(child0_0_0, LV_STATE_CHECKED);
    }
}

static void recipe_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *current_target = lv_event_get_current_target(e);
    lv_obj_t *target = lv_event_get_target(e);
    recipe_t *user_data = (recipe_t *)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d current_target:%p target:%p\n", __func__, e->code, current_target, target);
    if (code == LV_EVENT_CLICKED)
    {
        cur_recipe = user_data;
        if (lv_obj_has_state(current_target, LV_STATE_CHECKED))
        {
            lv_100ask_page_manager_page_t *page = lv_page_get_page("page_cook_details");
            page->user_data = user_data;
            lv_100ask_page_manager_set_open_page(NULL, "page_cook_details");
            return;
        }
        return;
        lv_obj_t *parent = lv_obj_get_parent(current_target);

        int child_num = lv_obj_get_child_cnt(parent);
        for (int i = 0; i < child_num; ++i)
        {
            lv_obj_t *child = lv_obj_get_child(parent, i);
            if (child == current_target)
            {
                recipe_click_checked(child, 1);
            }
            else
            {
                if (lv_obj_has_state(child, LV_STATE_CHECKED))
                {
                    recipe_click_checked(child, 0);
                }
            }
        }
    }
}

static void list_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *current_target = lv_event_get_current_target(e);
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *user_data = lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d current_target:%p target:%p\n", __func__, e->code, current_target, target);
    if (code == LV_EVENT_CLICKED)
    {
        if (lv_obj_has_state(current_target, LV_STATE_CHECKED))
        {
            return;
        }
        lv_obj_t *parent = lv_obj_get_parent(current_target);
        LV_LOG_USER("Clicked: %s", lv_list_get_btn_text(parent, current_target));

        int child_num = lv_obj_get_child_cnt(parent);
        for (int i = 0; i < child_num; ++i)
        {
            lv_obj_t *child = lv_obj_get_child(parent, i);
            lv_obj_t *child0 = lv_obj_get_child(child, 0);
            if (child == current_target)
            {
                lv_obj_add_state(child, LV_STATE_CHECKED);
                lv_obj_add_state(child0, LV_STATE_CHECKED);
                lv_get_recipes(user_data, i + 1, cook_pos);
            }
            else
            {
                if (lv_obj_has_state(child, LV_STATE_CHECKED))
                {
                    lv_obj_clear_state(child, LV_STATE_CHECKED);
                    lv_obj_clear_state(child0, LV_STATE_CHECKED);
                }
            }
            lv_obj_center(child0);
        }
    }
}

static lv_obj_t *lv_text_img_btn_create(lv_obj_t *parent, const char *text, void *user_data)
{
    lv_obj_t *btn1 = lv_btn_create(parent);
    lv_obj_set_size(btn1, LV_PCT(100), 62);

    lv_obj_add_event_cb(btn1, list_event_handler, LV_EVENT_CLICKED, user_data);
    lv_obj_set_style_bg_img_src(btn1, themesImagesPath "menulist_item_background.png", LV_STATE_CHECKED);

    lv_obj_t *label = lv_label_create(btn1);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_24, 0);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, LV_STATE_CHECKED);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    // lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return btn1;
}
static lv_obj_t *lv_recipe_create(lv_obj_t *parent, const char *img_src, const char *text, const char cookPos, void *arg)
{
    char imgUrl[256];
    if (cookPos == 0)
        sprintf(imgUrl, "%s%s%s", recipesImagesPath, img_src, "/0.png");
    else
        sprintf(imgUrl, "%s%s%s", recipesImagesPath, img_src, ".png");
    LV_LOG_USER("img_src:%s text:%s imgUrl:%s", img_src, text, imgUrl);

    lv_obj_t *obj = lv_btn_create(parent);
    lv_obj_set_size(obj, 164, 210);
    obj->user_data = arg;
    lv_obj_add_event_cb(obj, recipe_event_cb, LV_EVENT_CLICKED, arg);

    lv_obj_t *img = lv_img_create(obj);
    lv_obj_set_size(img, LV_PCT(100) - 10, LV_PCT(100));
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_img_set_src(img, imgUrl);
    lv_obj_set_style_border_color(img, lv_color_hex(themesTextColor), LV_STATE_CHECKED);
    lv_obj_set_style_border_width(img, 3, LV_STATE_CHECKED);
    lv_obj_set_style_radius(img, 4, LV_STATE_CHECKED);

    lv_obj_t *bottom_img = lv_img_create(img);
    lv_obj_set_size(bottom_img, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_align(bottom_img, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_img_set_src(bottom_img, themesImagesPath "recipename_checked_background.png");

    lv_obj_t *label = lv_label_create(bottom_img);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);
    lv_obj_set_size(label, LV_PCT(100) - 20, 24);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_20, 0);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_24, LV_STATE_CHECKED);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label, text);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    return NULL;
}
static void lv_get_recipes(lv_obj_t *parent, unsigned char recipeType, char cookPos)
{
    lv_obj_clean(parent);
    cur_recipe = NULL;
    if (cookPos == 0)
    {
        for (int i = 0; i < 40; ++i)
        {
            // LV_LOG_USER("%s,recipeType:%d\n", __func__, g_recipes[i].recipeType);
            if (recipeType == g_recipes[i].recipeType)
            {
                lv_recipe_create(parent, g_recipes[i].imgUrl, g_recipes[i].dishName, g_recipes[i].cookPos, &g_recipes[i]);
            }
        }
        scroll_change(parent, 0);
        return;
    }
    else
    {
        for (int i = 40; i < 60; ++i)
        {
            if (recipeType == g_recipes[i].recipeType)
            {
                lv_recipe_create(parent, g_recipes[i].imgUrl, g_recipes[i].dishName, g_recipes[i].cookPos, &g_recipes[i]);
            }
        }
    }
    lv_obj_t *child = lv_obj_get_child(parent, 0);
    recipe_click_checked(child, 1);
}
static void dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *obj = lv_event_get_current_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
        recipe_cook_start(cur_recipe, 0);
    }
    break;
    }
    clean_manual_layer();
}
static void reserve_dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *obj = lv_event_get_current_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
        int orderTime = lv_get_reserve_dialog_time();
        LV_LOG_USER("%s,orderTime:%d\n", __func__, orderTime);
        recipe_cook_start(cur_recipe, orderTime);
    }
    break;
    }
    clean_manual_layer();
}
static void btn_array_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%ld\n", __func__, e->code, user_data);
    if (user_data == 0)
    {
        lv_manual_cook_dialog("请将食物放入左腔,水箱中加满水", dialog_event_cb);
    }
    else
    {
        lv_manual_reserve_dialog("左腔将在", "后启动", "预约", 12, reserve_dialog_event_cb);
    }
}
static void cookPos_btn_array_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%ld\n", __func__, e->code, user_data);
    cook_pos = user_data;
    lv_obj_t *parent;
    if (user_data == 0)
    {
        lv_obj_add_flag(smart_recipes[1], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(smart_recipes[0], LV_OBJ_FLAG_HIDDEN);
        // lv_obj_set_style_border_opa(pos_btn[0], LV_OPA_100, 0);
        // lv_obj_set_style_border_opa(pos_btn[1], LV_OPA_0, 0);
        lv_obj_set_style_text_color(lv_obj_get_child(pos_btn[0], 0), lv_color_hex(themesTextColor), 0);
        lv_obj_set_style_text_color(lv_obj_get_child(pos_btn[1], 0), lv_color_hex(0xffffff), 0);
        lv_obj_set_style_bg_opa(pos_btn[0], LV_OPA_100, 0);
        lv_obj_set_style_bg_opa(pos_btn[1], LV_OPA_0, 0);
        parent = smart_recipes[0]->user_data;
    }
    else
    {
        lv_obj_add_flag(smart_recipes[0], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(smart_recipes[1], LV_OBJ_FLAG_HIDDEN);
        // lv_obj_set_style_border_opa(pos_btn[0], LV_OPA_0, 0);
        // lv_obj_set_style_border_opa(pos_btn[1], LV_OPA_100, 0);
        lv_obj_set_style_text_color(lv_obj_get_child(pos_btn[1], 0), lv_color_hex(themesTextColor), 0);
        lv_obj_set_style_text_color(lv_obj_get_child(pos_btn[0], 0), lv_color_hex(0xffffff), 0);
        lv_obj_set_style_bg_opa(pos_btn[0], LV_OPA_0, 0);
        lv_obj_set_style_bg_opa(pos_btn[1], LV_OPA_100, 0);
        parent = smart_recipes[1]->user_data;
    }
    int child_num = lv_obj_get_child_cnt(parent);
    for (int i = 0; i < child_num; ++i)
    {
        lv_obj_t *child = lv_obj_get_child(parent, i);
        if (lv_obj_has_state(child, LV_STATE_CHECKED))
        {
            recipe_click_checked(child, 1);
        }
    }
}
static void scroll_change(lv_obj_t *cont, int cur_index)
{
    uint32_t child_cnt = lv_obj_get_child_cnt(cont); // 获取子界面的数量
    int mid_btn_index = (child_cnt - 1) / 2;         // 如果界面为偶数，将中间数向下取整的界面设置为中间界面

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
    
    if (e->code != LV_EVENT_SCROLL_END)
        return;

    LV_LOG_USER("%s,code:%d", __func__, e->code);
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
    lv_obj_get_coords(cont, &cont_a);                                      // 将cont对象的坐标复制到cont_a
    lv_coord_t cont_x_center = cont_a.x1 + lv_area_get_width(&cont_a) / 2; // 获取界面的宽像素大小/2

    /* 遍历子界面 */
    for (lv_coord_t i = 0; i < child_cnt; i++)
    {
        lv_obj_t *child = lv_obj_get_child(cont, i); // 通过索引获取子对象

        /* 获取子对象y轴的中心坐标值 */
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);
        lv_coord_t child_x_center = child_a.x1 + lv_area_get_width(&child_a) / 2; // 获取界面中按钮宽像素值的大小/2
        /* 子界面的坐标与父界面的坐标相等时，说明当前界面在父界面中显示 */
        if (child_x_center == cont_x_center)
        {

            /* 当前显示界面的索引 */
            lv_coord_t current_btn_index = lv_obj_get_index(child);
            LV_LOG_USER("%s,code:%d current_btn_index:%d mid_btn_index:%d\n", __func__, e->code, current_btn_index, mid_btn_index);
            /* 判断界面移动的数数据，并将当前界面的索引改为中间位置 */
            /* 因为是在滑动结束后实现的，建议界面较多的情况下使用此方式，当界面较少，一次滑动太多界面时，容易滑倒边界出现卡顿现象 */

            scroll_change(cont, current_btn_index);
            /* 保证界面居中显示 */
            recipe_click_checked(child, 1);
        }
        else
        {
            recipe_click_checked(child, 0);
        }
    }
}
static void left_recipes_create(lv_obj_t *parent)
{
    lv_obj_t *left_content = lv_obj_create(parent);
    lv_obj_t *right_content = lv_obj_create(parent);
    lv_obj_clear_flag(right_content, LV_OBJ_FLAG_SCROLL_ELASTIC);
    parent->user_data = right_content;

    lv_obj_set_size(left_content, 156, 345);
    lv_obj_align(left_content, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
    lv_obj_t *menulist = lv_img_create(left_content);
    lv_obj_set_size(menulist, LV_PCT(100), LV_PCT(100));
    lv_img_set_src(menulist, themesImagesPath "menulist_background.png");

    lv_obj_t *list = lv_list_create(left_content);

    lv_obj_set_size(list, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_t *first_btn = lv_text_img_btn_create(list, "蔬菜杂粮", right_content);
    lv_text_img_btn_create(list, "家禽肉类", right_content);
    lv_text_img_btn_create(list, "河海鲜类", right_content);
    lv_text_img_btn_create(list, "烘焙甜点", right_content);
    lv_text_img_btn_create(list, "其他食材", right_content);

    lv_obj_add_state(first_btn, LV_STATE_CHECKED);
    lv_obj_add_state(lv_obj_get_child(first_btn, 0), LV_STATE_CHECKED);

    lv_obj_set_size(right_content, 152 * 4 + 180 + 30, 258);
    lv_obj_align_to(right_content, left_content, LV_ALIGN_OUT_RIGHT_TOP, 0, 26);

    lv_obj_set_flex_flow(right_content, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(right_content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(right_content, LV_DIR_HOR);
    lv_obj_set_scroll_snap_x(right_content, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(right_content, LV_SCROLLBAR_MODE_OFF);
    // lv_obj_clear_flag(right_content, LV_OBJ_FLAG_SCROLL_ELASTIC);
    // lv_obj_clear_flag(right_content, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    lv_obj_clear_flag(right_content, LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_add_event_cb(right_content, scroll_end_event, LV_EVENT_ALL, NULL);
    lv_obj_set_style_pad_column(right_content, 100,0);
    lv_get_recipes(right_content, 1, 0);
}

static void right_recipes_create(lv_obj_t *parent)
{
    lv_obj_t *left_content = lv_obj_create(parent);
    lv_obj_t *right_content = lv_obj_create(parent);
    lv_obj_clear_flag(right_content, LV_OBJ_FLAG_SCROLL_ELASTIC);
    parent->user_data = right_content;

    lv_obj_set_size(left_content, 156, 345);
    lv_obj_align(left_content, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
    lv_obj_t *menulist = lv_img_create(left_content);
    lv_obj_set_size(menulist, LV_PCT(100), LV_PCT(100));
    lv_img_set_src(menulist, themesImagesPath "menulist_background.png");

    lv_obj_t *list = lv_list_create(left_content);

    lv_obj_set_size(list, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_t *first_btn = lv_text_img_btn_create(list, "油炸菜", right_content);
    lv_text_img_btn_create(list, "水煮菜", right_content);
    lv_text_img_btn_create(list, "香煎菜", right_content);
    lv_text_img_btn_create(list, "清炒菜", right_content);

    lv_obj_add_state(first_btn, LV_STATE_CHECKED);
    lv_obj_add_state(lv_obj_get_child(first_btn, 0), LV_STATE_CHECKED);

    lv_obj_set_size(right_content, 152 * 4 + 180 + 30, 258);
    lv_obj_align_to(right_content, left_content, LV_ALIGN_OUT_RIGHT_TOP, 0, 26);

    lv_obj_set_flex_flow(right_content, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(right_content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_get_recipes(right_content, 1, 1);
}
void lv_page_smartrecipes_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    // lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t *back_bar = lv_page_back_bar_init(page, "智慧菜谱", NULL, NULL);
    lv_page_top_bar_init(page, 2);

    smart_recipes[0] = lv_obj_create(page);
    lv_obj_set_size(smart_recipes[0], LV_PCT(100), 345);
    lv_obj_align_to(smart_recipes[0], back_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    left_recipes_create(smart_recipes[0]);

    smart_recipes[1] = lv_obj_create(page);
    lv_obj_set_size(smart_recipes[1], LV_PCT(100), 345);
    lv_obj_align_to(smart_recipes[1], back_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    right_recipes_create(smart_recipes[1]);
    lv_obj_add_flag(smart_recipes[1], LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *cont = lv_obj_create(page);
    lv_obj_set_size(cont, 200 * 2, 40);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(cont, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_100, 0);
    lv_obj_set_style_radius(cont, 4, 0);

    const char *pos_text[] = {"蒸烤菜(左腔)", "右灶菜"};

    for (long i = 0; i < 2; ++i)
    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 200, LV_PCT(100));
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x3E2B21), 0);
        lv_obj_set_style_bg_opa(btn, LV_OPA_0, 0);
        lv_obj_set_style_radius(btn, 4, 0);
        // lv_obj_set_style_border_color(btn, lv_color_hex(themesTextColor), 0);
        // lv_obj_set_style_border_width(btn, 4, 0);
        // lv_obj_set_style_border_opa(btn, LV_OPA_0, 0);
        lv_obj_set_style_outline_color(btn, lv_color_hex(themesTextColor), 0);
        lv_obj_set_style_outline_width(btn, 4, 0);
        lv_obj_set_style_outline_opa(btn, LV_OPA_0, 0);
        lv_obj_add_event_cb(btn, cookPos_btn_array_event_cb, LV_EVENT_CLICKED, (void *)i);
        pos_btn[i] = btn;

        lv_obj_t *label = lv_label_create(btn);
        lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_24, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
        lv_label_set_text(label, pos_text[i]);
        lv_obj_center(label);
    }
    // lv_obj_set_style_outline_opa(pos_btn[0], LV_OPA_100, 0);
    lv_obj_set_style_bg_opa(pos_btn[0], LV_OPA_100, 0);
    // lv_obj_set_style_border_opa(pos_btn[0], LV_OPA_100, 0);
    lv_obj_set_style_text_color(lv_obj_get_child(pos_btn[0], 0), lv_color_hex(themesTextColor), 0);
    lv_obj_align_to(cont, back_bar, LV_ALIGN_BOTTOM_MID, 0, 10);
    //------------------------------
    const char *text[] = {"启动", "预约"};
    lv_obj_t *btn_array = lv_custom_btn_array_create(page, text, 2, btn_array_event_cb);
    lv_obj_align_to(btn_array, back_bar, LV_ALIGN_OUT_BOTTOM_RIGHT, -40, 90);
}
