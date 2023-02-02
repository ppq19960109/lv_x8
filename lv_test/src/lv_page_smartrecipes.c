

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/
static recipe_t *cur_recipe = NULL;
/**********************
 *  STATIC VARIABLES
 **********************/
static void lv_get_recipes(lv_obj_t *parent, unsigned char recipeType);
void recipe_cook_start(recipe_t *recipe, const int reserve_time)
{
    if (recipe == NULL)
    {
        dzlog_error("recipe data is NULL\n");
        return;
    }
    steamoven_t steamoven = {0};
    steamoven.cookId = recipe->recipeid;
    strcpy(steamoven.cookName, recipe->dishName);

    cJSON *root = cJSON_Parse(recipe->cookSteps);
    if (root == NULL)
        return;
    int arraySize = cJSON_GetArraySize(root);
    if (arraySize == 0)
    {
        dzlog_error("attr arraySize is 0\n");
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
static void recipe_click_checked(lv_obj_t *child, unsigned char checked)
{
    if (checked)
    {
        lv_obj_set_size(child, 180, 258);
        lv_obj_t *child0 = lv_obj_get_child(child, 0);
        lv_obj_t *child0_0 = lv_obj_get_child(child0, 0);
        lv_obj_t *child0_0_0 = lv_obj_get_child(child0_0, 0);
        lv_obj_add_state(child, LV_STATE_CHECKED);
        lv_obj_add_state(child0, LV_STATE_CHECKED);
        lv_obj_add_state(child0_0_0, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_set_size(child, 152, 210);
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
    lv_obj_t *user_data = lv_event_get_user_data(e);
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
                lv_get_recipes(user_data, i + 1);
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
static lv_obj_t *lv_recipe_create(lv_obj_t *parent, const char *img_src, const char *text, void *arg)
{
    char imgUrl[256];
    sprintf(imgUrl, "%s%s%s", recipesImagesPath, img_src, "/0.png");
    LV_LOG_USER("img_src:%s text:%s imgUrl:%s", img_src, text, imgUrl);

    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, 152, 210);
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
static void lv_get_recipes(lv_obj_t *parent, unsigned char recipeType)
{
    lv_obj_clean(parent);
    cur_recipe = NULL;
    for (int i = 0; i < 40; ++i)
    {
        if (recipeType == g_recipes[i].recipeType)
        {
            lv_recipe_create(parent, g_recipes[i].imgUrl, g_recipes[i].dishName, &g_recipes[i]);
            if (cur_recipe == NULL)
            {
                cur_recipe = &g_recipes[i];
            }
        }
    }
    lv_obj_t *child = lv_obj_get_child(parent, 0);
    recipe_click_checked(child, 1);
}
static void dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *obj = lv_event_get_current_target(e);
    int user_data = (int)lv_event_get_user_data(e);
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
    lv_obj_clean(lv_layer_top());
}
static void reserve_dialog_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t *obj = lv_event_get_current_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
    {
        lv_obj_t *reserve_dialog = lv_obj_get_child(lv_layer_top(), 0);
        int orderTime = lv_get_reserve_dialog_time(reserve_dialog);
        LV_LOG_USER("%s,orderTime:%d\n", __func__, orderTime);
        recipe_cook_start(cur_recipe, orderTime);
    }
    break;
    }
    lv_obj_clean(lv_layer_top());
}
static void btn_array_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    if (user_data == 0)
    {
        lv_manual_cook_dialog("请将食物放入左腔,水箱中加满水", dialog_event_cb);
    }
    else
    {
        lv_manual_reserve_dialog("左腔将在", reserve_dialog_event_cb);
    }
}
void lv_page_smartrecipes_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    // lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t *back_bar = lv_page_back_bar_init(page, "智慧菜谱", NULL, NULL);
    lv_page_top_bar_init(page, 2);

    lv_obj_t *left_content = lv_obj_create(page);
    lv_obj_t *right_content = lv_obj_create(page);
    lv_obj_clear_flag(right_content, LV_OBJ_FLAG_SCROLL_ELASTIC);

    lv_obj_set_size(left_content, 156, 345);
    lv_obj_align_to(left_content, back_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
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
    lv_get_recipes(right_content, 1);
    //------------------------------
    const char *text[] = {"启动", "预约"};
    lv_obj_t *btn_array = lv_custom_btn_array_create(page, text, 2, btn_array_event_cb);
    lv_obj_align_to(btn_array, back_bar, LV_ALIGN_OUT_BOTTOM_RIGHT, -40, 90);
}
