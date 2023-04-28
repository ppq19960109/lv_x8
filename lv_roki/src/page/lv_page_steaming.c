

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/
typedef struct
{
    lv_obj_t *arc;
    lv_obj_t *arc_label_temp;
    lv_obj_t *arc_label_top_temp;
    lv_obj_t *arc_label_bottom_temp;
    lv_obj_t *arc_label_time;
    lv_obj_t *arc_label_work_state;

    lv_obj_t *label_mode;
    lv_obj_t *label_temp;
    lv_obj_t *label_top_temp;
    lv_obj_t *label_bottom_temp;
    lv_obj_t *label_time;
    lv_obj_t *label_steam;

    lv_obj_t *btn_preheat;
    lv_obj_t *btn_vapour;
    lv_obj_t *btn_start_stop;
} lv_cooking_t;
static lv_cooking_t lv_cooking;
static lv_obj_t *left_steaming, *left_child, *right_child;
static lv_obj_t *btn_array[2];
static char work_state[2];
/**********************
 *  STATIC VARIABLES
 **********************/
static void btn_array_update(const int index, char *text[], const int count)
{
    lv_obj_t *child;
    int i;
    for (i = 0; i < count; ++i)
    {
        child = lv_obj_get_child(btn_array[index], i);
        lv_obj_clear_flag(child, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(lv_obj_get_child(child, 0), text[i]);
    }
    for (; i < 3; ++i)
    {
        child = lv_obj_get_child(btn_array[index], i);
        lv_obj_add_flag(child, LV_OBJ_FLAG_HIDDEN);
    }
}
static void page_update_StOvState(const int index, void *ptr)
{
    int value;
    lv_obj_t **child;
    if (index == 0)
    {
        if (ptr == NULL)
            value = get_attr_value_int("LStOvState");
        child = left_child;
    }
    else
    {
        if (ptr == NULL)
            value = get_attr_value_int("RStOvState");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);
    work_state[index] = value;
    if (value == WORK_STATE_STOP)
    {
        lv_obj_clear_flag(child[0], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[1], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[2], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[3], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[4], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[5], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[6], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[7], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[8], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[9], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[10], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn_array[index], LV_OBJ_FLAG_HIDDEN);
    }
    else if (value == WORK_STATE_FINISH)
    {
        lv_obj_add_flag(child[0], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[1], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[2], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[3], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[4], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[5], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[6], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[7], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[8], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[9], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[10], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(btn_array[index], LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(child[0], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[1], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[2], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[3], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[4], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(child[5], LV_OBJ_FLAG_HIDDEN);
        if (value == WORK_STATE_PREHEAT)
        {
            lv_obj_add_flag(child[6], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(child[7], LV_OBJ_FLAG_HIDDEN);
            lv_rotate_anim(child[7], 1);
        }
        else
        {
            lv_obj_clear_flag(child[6], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[7], LV_OBJ_FLAG_HIDDEN);
            lv_rotate_anim(child[7], 0);
        }
        lv_obj_add_flag(child[8], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[9], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child[10], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn_array[index], LV_OBJ_FLAG_HIDDEN);

        lv_label_set_text(child[2], workStateChineseEnum[value]);
    }
    char *btn_text[3];
    int btn_count = 0;
    if (value == WORK_STATE_STOP)
    {
        if (index == 0)
        {
            btn_text[btn_count++] = "左腔蒸烤";
            btn_text[btn_count++] = "多段烹饪";
            btn_text[btn_count++] = "智慧菜谱";
        }
        else
        {
            btn_text[btn_count++] = "右腔速蒸";
            btn_text[btn_count++] = "辅助烹饪";
        }
    }
    else if (value == WORK_STATE_PAUSE || value == WORK_STATE_PREHEAT_RESERVE)
    {
        btn_text[btn_count++] = "继续";
        btn_text[btn_count++] = "取消";
    }
    else if (value == WORK_STATE_PAUSE_RESERVE)
    {
        btn_text[btn_count++] = "继续";
        btn_text[btn_count++] = "取消";
        btn_text[btn_count++] = "立即烹饪";
    }
    else if (value == WORK_STATE_RESERVE)
    {
        btn_text[btn_count++] = "暂停";
        btn_text[btn_count++] = "取消";
        btn_text[btn_count++] = "立即烹饪";
    }
    else if (value == WORK_STATE_FINISH)
    {
        return;
    }
    else
    {
        btn_text[btn_count++] = "暂停";
        btn_text[btn_count++] = "取消";
    }
    btn_array_update(index, btn_text, btn_count);
}

static void page_update_StOvRealTemp(const int index, void *ptr)
{
    if (work_state[index] != WORK_STATE_PREHEAT && work_state[index] != WORK_STATE_PREHEAT_RESERVE)
        return;
    int value;
    lv_obj_t **child;
    if (index == 0)
    {
        if (ptr == NULL)
            value = get_attr_value_int("LStOvRealTemp");
        child = left_child;
    }
    else
    {
        if (ptr == NULL)
            value = get_attr_value_int("RStOvRealTemp");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);
    char buf[16];
    sprintf(buf, "%d℃", value);
    lv_label_set_text(child[3], buf);
}
static void page_update_StOvSetTimerLeft(const int index, void *ptr)
{
    int value;
    float setTime_value;
    lv_obj_t **child;
    if (index == 0)
    {
        setTime_value = get_attr_value_int("LStOvSetTimer");
        if (ptr == NULL)
            value = get_attr_value_int("LStOvSetTimerLeft");
        child = left_child;
    }
    else
    {
        setTime_value = get_attr_value_int("RStOvSetTimer");
        if (ptr == NULL)
            value = get_attr_value_int("RStOvSetTimerLeft");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);

    if (work_state[index] == WORK_STATE_RUN || work_state[index] == WORK_STATE_PAUSE)
    {
        char buf[16];
        sprintf(buf, "%d分钟", value);
        lv_label_set_text(child[3], buf);
    }

    lv_arc_set_value(child[6], (value / setTime_value) * 100);
}
static void page_update_StOvOrderTimerLeft(const int index, void *ptr)
{
    if (work_state[index] != WORK_STATE_RESERVE && work_state[index] != WORK_STATE_PAUSE_RESERVE)
        return;
    int value;
    float setTime_value;
    lv_obj_t **child;
    if (index == 0)
    {
        setTime_value = get_attr_value_int("LStOvOrderTimer");
        if (ptr == NULL)
            value = get_attr_value_int("LStOvOrderTimerLeft");
        child = left_child;
    }
    else
    {
        setTime_value = get_attr_value_int("RStOvOrderTimer");
        if (ptr == NULL)
            value = get_attr_value_int("RStOvOrderTimerLeft");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);
    char buf[16];
    sprintf(buf, "%d分钟", value);
    lv_label_set_text(child[3], buf);

    lv_arc_set_value(child[6], (value / setTime_value) * 100);
}
static void page_update_StOvMode(const int index, void *ptr)
{
    int value;
    lv_obj_t **child;
    if (index == 0)
    {
        value = get_attr_value_int("LMultiMode");
        if (value == MULTI_MODE_RECIPE)
            return;
        if (ptr == NULL)
            value = get_attr_value_int("LStOvMode");
        child = left_child;
    }
    else
    {
        value = get_attr_value_int("RMultiMode");
        if (value == MULTI_MODE_RECIPE)
            return;
        if (ptr == NULL)
            value = get_attr_value_int("RStOvMode");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_int(ptr);
    lv_label_set_text(child[4], workModeName(value));
}
static void page_update_CookName(const int index, void *ptr)
{
    int multiMode_value;
    char *value;
    lv_obj_t **child;

    if (index == 0)
    {
        multiMode_value = get_attr_value_int("LMultiMode");
        if (multiMode_value != MULTI_MODE_RECIPE)
            return;
        if (ptr == NULL)
            value = get_attr_value_string("LCookbookName");

        child = left_child;
    }
    else
    {
        multiMode_value = get_attr_value_int("RMultiMode");
        if (multiMode_value != MULTI_MODE_RECIPE)
            return;
        if (ptr == NULL)
            value = get_attr_value_string("RCookbookName");
        child = right_child;
    }
    if (ptr != NULL)
        value = get_value_string(ptr);
    lv_label_set_text(child[4], value);
}
static void page_update_StOvSetTemp_StOvSetTimer(const int index, void *ptr)
{
    int temp_value, time_value;
    lv_obj_t **child;
    if (index == 0)
    {
        temp_value = get_attr_value_int("LStOvSetTemp");
        time_value = get_attr_value_int("LStOvSetTimer");
        child = left_child;
    }
    else
    {
        temp_value = get_attr_value_int("RStOvSetTemp");
        time_value = get_attr_value_int("RStOvSetTimer");
        child = right_child;
    }
    char buf[28];
    sprintf(buf, "%d℃  %d分钟", temp_value, time_value);
    lv_label_set_text(child[5], buf);
}
static void property_change_cb(const char *key, void *value)
{
    LV_LOG_USER("key:%s\n", key);
    if (strcmp("LStOvState", key) == 0)
    {
        page_update_StOvState(0, value);
    }
    else if (strcmp("RStOvState", key) == 0)
    {
        page_update_StOvState(1, value);
    }
    else if (strcmp("LStOvRealTemp", key) == 0)
    {
        page_update_StOvRealTemp(0, value);
    }
    else if (strcmp("RStOvRealTemp", key) == 0)
    {
        page_update_StOvRealTemp(1, value);
    }
    else if (strcmp("LStOvSetTimerLeft", key) == 0)
    {
        page_update_StOvSetTimerLeft(0, value);
    }
    else if (strcmp("RStOvSetTimerLeft", key) == 0)
    {
        page_update_StOvSetTimerLeft(1, value);
    }
    else if (strcmp("LStOvOrderTimerLeft", key) == 0)
    {
        page_update_StOvOrderTimerLeft(0, value);
    }
    else if (strcmp("RStOvOrderTimerLeft", key) == 0)
    {
        page_update_StOvOrderTimerLeft(1, value);
    }
    else if (strcmp("LStOvMode", key) == 0)
    {
        page_update_StOvMode(0, value);
    }
    else if (strcmp("RStOvMode", key) == 0)
    {
        page_update_StOvMode(1, value);
    }
    else if (strcmp("LCookbookName", key) == 0)
    {
        page_update_CookName(0, value);
    }
    else if (strcmp("RCookbookName", key) == 0)
    {
        page_update_CookName(1, value);
    }
    else if (strcmp("LStOvSetTemp", key) == 0 || strcmp("LStOvSetTimer", key) == 0)
    {
        page_update_StOvSetTemp_StOvSetTimer(0, NULL);
    }
    else if (strcmp("RStOvSetTemp", key) == 0 || strcmp("RStOvSetTimer", key) == 0)
    {
        page_update_StOvSetTemp_StOvSetTimer(1, NULL);
    }
}
static void page_update_cb(void *arg)
{
    LV_LOG_USER("%s\n", __func__);
    lv_page_top_hide(1);
    // page_update_StOvState(0, NULL);
    // page_update_StOvState(1, NULL);
    // page_update_StOvRealTemp(0, NULL);
    // page_update_StOvRealTemp(1, NULL);
    // page_update_StOvSetTimerLeft(0, NULL);
    // page_update_StOvSetTimerLeft(1, NULL);
    // page_update_StOvOrderTimerLeft(0, NULL);
    // page_update_StOvOrderTimerLeft(1, NULL);
    // page_update_StOvMode(0, NULL);
    // page_update_StOvMode(1, NULL);
    // page_update_CookName(0, NULL);
    // page_update_CookName(1, NULL);
    // page_update_StOvSetTemp_StOvSetTimer(0, NULL);
    // page_update_StOvSetTemp_StOvSetTimer(1, NULL);
}

static void finish_back_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    if (user_data == 0)
    {
        set_num_toServer("LStOvOperation", WORK_OPERATION_CONFIRM);
    }
    else
    {
        set_num_toServer("RStOvOperation", WORK_OPERATION_CONFIRM);
    }
}
static lv_obj_t *lv_steam_item_create(lv_obj_t *parent, const int index, lv_cooking_t *cooking)
{
    lv_obj_t *obj = lv_img_create(parent);
    lv_img_set_src(obj, getThemesPath("bg_work_arc_circle.png"));
    // lv_obj_set_size(obj, 290, 290);
    lv_obj_t *img = lv_img_create(obj);
    lv_img_set_src(img, getThemesPath("bg_work_arc_bg.png"));
    // lv_obj_align_to(img, obj, LV_ALIGN_CENTER, 0, 0);
    //------------------------------
    lv_obj_t *temp_obj = lv_obj_create(obj);
    lv_obj_set_size(temp_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(temp_obj, LV_ALIGN_TOP_MID, 0, 140);
    lv_obj_t *label_temp = lv_label_create(temp_obj);
    lv_obj_set_style_text_font(label_temp, g_robam_font.FZLTHC_66.font, 0);
    lv_obj_set_style_text_color(label_temp, getThemesFontColor1(), 0);
    lv_label_set_text(label_temp, "180");
    lv_obj_t *label_unit = lv_label_create(temp_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align_to(label_unit, label_temp, LV_ALIGN_OUT_RIGHT_MID, 0, -10);
    cooking->arc_label_temp = label_temp;

    temp_obj = lv_obj_create(obj);
    lv_obj_set_size(temp_obj, 300, LV_SIZE_CONTENT);
    lv_obj_align(temp_obj, LV_ALIGN_TOP_MID, 0, 90);
    label_temp = lv_label_create(temp_obj);
    lv_obj_set_style_text_font(label_temp, g_robam_font.FZLTHC_66.font, 0);
    lv_obj_set_style_text_color(label_temp, getThemesFontColor1(), 0);
    lv_label_set_text(label_temp, "150");
    lv_obj_align(label_temp, LV_ALIGN_CENTER, 0, 0);
    label_unit = lv_label_create(temp_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align_to(label_unit, label_temp, LV_ALIGN_OUT_RIGHT_MID, 0, -10);
    label_unit = lv_label_create(temp_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "上温");
    lv_obj_align_to(label_unit, label_temp, LV_ALIGN_OUT_LEFT_MID, 0, 10);
    cooking->arc_label_top_temp = label_temp;

    temp_obj = lv_obj_create(obj);
    lv_obj_set_size(temp_obj, 300, LV_SIZE_CONTENT);
    lv_obj_align(temp_obj, LV_ALIGN_TOP_MID, 0, 170);
    label_temp = lv_label_create(temp_obj);
    lv_obj_set_style_text_font(label_temp, g_robam_font.FZLTHC_66.font, 0);
    lv_obj_set_style_text_color(label_temp, getThemesFontColor1(), 0);
    lv_label_set_text(label_temp, "160");
    lv_obj_align(label_temp, LV_ALIGN_CENTER, 0, 0);
    label_unit = lv_label_create(temp_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align_to(label_unit, label_temp, LV_ALIGN_OUT_RIGHT_MID, 0, -10);
    label_unit = lv_label_create(temp_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "下温");
    lv_obj_align_to(label_unit, label_temp, LV_ALIGN_OUT_LEFT_MID, 0, 10);
    cooking->arc_label_bottom_temp = label_temp;

    lv_obj_t *time_obj = lv_obj_create(obj);
    lv_obj_set_size(time_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(time_obj, LV_ALIGN_TOP_MID, 0, 140);
    lv_obj_t *label_time = lv_label_create(time_obj);
    lv_obj_set_style_text_font(label_time, g_robam_font.FZLTHC_66.font, 0);
    lv_obj_set_style_text_color(label_time, getThemesFontColor1(), 0);
    lv_label_set_text(label_time, "30");
    label_unit = lv_label_create(time_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "min");
    lv_obj_align_to(label_unit, label_time, LV_ALIGN_OUT_RIGHT_MID, 0, 10);
    cooking->arc_label_time = label_time;

    lv_obj_t *label_state = lv_label_create(obj);
    lv_obj_set_style_text_font(label_state, g_robam_font.FZLTHJW_36.font, 0);
    lv_obj_set_style_text_color(label_state, getThemesFontColor1(), 0);
    lv_obj_align(label_state, LV_ALIGN_TOP_MID, 0, 242);
    lv_label_set_text(label_state, "预热中");
    cooking->arc_label_work_state = label_state;

    lv_obj_t *arc = lv_arc_create(obj);
    lv_obj_set_size(arc, 315, 315);
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    // lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(arc);
    lv_obj_set_style_arc_color(arc, lv_color_hex(themesTextColor), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(arc, 20, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(arc, true, LV_PART_INDICATOR);
    lv_obj_set_style_bg_img_src(arc, publicImagesPath "knob.png", LV_PART_KNOB);
    lv_obj_set_style_pad_all(arc, 10, LV_PART_KNOB);
    lv_arc_set_value(arc, 30);
    cooking->arc = arc;
    //------------------------------
    // lv_obj_t *run_img = lv_img_create(obj); //
    // lv_img_set_src(run_img, themesImagesPath "icon_steam_runing.png");
    // lv_obj_align(run_img, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_update_layout(run_img);
    // lv_img_set_pivot(run_img, lv_obj_get_width(run_img) / 2, lv_obj_get_height(run_img) / 2);

    return obj;
}
static void left_dialog1_event_cb(lv_event_t *e)
{
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
        set_num_toServer("LStOvOperation", WORK_OPERATION_CANCEL);
        break;
    }
    clean_manual_layer();
}
static void right_dialog1_event_cb(lv_event_t *e)
{
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
    case 1:
        break;
    case 2:
        set_num_toServer("RStOvOperation", WORK_OPERATION_CANCEL);
        break;
    }
    clean_manual_layer();
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
    case 2:

        break;
    }
}
static void right_btn_event_cb(lv_event_t *e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    switch (user_data)
    {
    case 0:
        if (work_state[1] == WORK_STATE_STOP)
        {
            lv_100ask_page_manager_set_open_page(NULL, "page_steam_right");
        }
        else if (work_state[1] == WORK_STATE_PAUSE || work_state[1] == WORK_STATE_PAUSE_RESERVE || work_state[1] == WORK_STATE_PREHEAT_RESERVE)
        {
            set_num_toServer("RStOvOperation", WORK_OPERATION_START);
        }
        else
        {
            set_num_toServer("RStOvOperation", WORK_OPERATION_PAUSE);
        }
        break;
    case 1:
        if (work_state[1] == WORK_STATE_STOP)
        {
        }
        else
        {
            lv_manual_dialog1("是否取消右腔烹饪?", "否", "是", right_dialog1_event_cb);
        }
        break;
    case 2:
        if (work_state[1] == WORK_STATE_STOP)
        {
        }
        else
        {
            set_num_toServer("RStOvOperation", WORK_OPERATION_RUN_NOW);
        }
        break;
    }
}
static void back_bar_event_cb(lv_event_t *e)
{
    lv_page_back_top_page();
}
void lv_page_steaming_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_property_change_cb = property_change_cb;
    manager_page->page_update_cb = page_update_cb;

    lv_page_back_bar_init(page, "返回", NULL, NULL);

    lv_obj_t *obj = lv_steam_item_create(page, 0, &lv_cooking);
    lv_obj_align(obj, LV_ALIGN_TOP_LEFT, 60, 80);

    lv_obj_t *cont = lv_obj_create(page);
    lv_obj_set_size(cont, 240, 270);
    lv_obj_align(cont, LV_ALIGN_LEFT_MID, 450, 15);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    lv_obj_t *label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, g_robam_font.FZLTHC_60.font, 0);
    lv_obj_set_style_text_color(label, getThemesFontColor1(), 0);
    lv_label_set_text(label, "焙烤");
    lv_cooking.label_mode = label;

    lv_obj_t *steam_obj = lv_obj_create(cont);
    lv_obj_set_size(steam_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    label = lv_label_create(steam_obj);
    lv_obj_set_style_text_font(label, g_robam_font.FZLTHC_66.font, 0);
    lv_obj_set_style_text_color(label, getThemesFontColor1(), 0);
    lv_label_set_text(label, "中");
    lv_obj_t *label_unit = lv_label_create(steam_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "蒸汽");
    lv_obj_align_to(label_unit, label, LV_ALIGN_OUT_RIGHT_MID, 5, 15);
    lv_cooking.label_steam = label;
    lv_obj_add_flag(lv_obj_get_parent(lv_cooking.label_steam), LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *time_obj = lv_obj_create(cont);
    lv_obj_set_size(time_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_t *label_time = lv_label_create(time_obj);
    lv_obj_set_style_text_font(label_time, g_robam_font.FZLTHC_66.font, 0);
    lv_obj_set_style_text_color(label_time, getThemesFontColor1(), 0);
    lv_label_set_text(label_time, "30");
    label_unit = lv_label_create(time_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "min");
    lv_obj_align_to(label_unit, label_time, LV_ALIGN_OUT_RIGHT_MID, 0, 10);
    lv_cooking.label_time = label_time;
    lv_obj_add_flag(lv_obj_get_parent(lv_cooking.label_time), LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *temp_obj = lv_obj_create(cont);
    lv_obj_set_size(temp_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_t *label_temp = lv_label_create(temp_obj);
    lv_obj_set_style_text_font(label_temp, g_robam_font.FZLTHC_66.font, 0);
    lv_obj_set_style_text_color(label_temp, getThemesFontColor1(), 0);
    lv_label_set_text(label_temp, "180");
    label_unit = lv_label_create(temp_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align_to(label_unit, label_temp, LV_ALIGN_OUT_RIGHT_MID, 0, -10);
    lv_cooking.label_temp = label_temp;
    lv_obj_add_flag(lv_obj_get_parent(lv_cooking.label_temp), LV_OBJ_FLAG_HIDDEN);

    temp_obj = lv_obj_create(cont);
    lv_obj_set_size(temp_obj, LV_PCT(100), LV_SIZE_CONTENT);
    label_temp = lv_label_create(temp_obj);
    lv_obj_set_style_text_font(label_temp, g_robam_font.FZLTHC_66.font, 0);
    lv_obj_set_style_text_color(label_temp, getThemesFontColor1(), 0);
    lv_label_set_text(label_temp, "150");
    lv_obj_align(label_temp, LV_ALIGN_CENTER, 0, 0);
    label_unit = lv_label_create(temp_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align_to(label_unit, label_temp, LV_ALIGN_OUT_RIGHT_MID, 0, -10);
    label_unit = lv_label_create(temp_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "上温");
    lv_obj_align_to(label_unit, label_temp, LV_ALIGN_OUT_LEFT_MID, 0, 10);
    lv_cooking.label_top_temp = label_temp;

    temp_obj = lv_obj_create(cont);
    lv_obj_set_size(temp_obj, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_align(temp_obj, LV_ALIGN_TOP_MID, 0, 170);
    label_temp = lv_label_create(temp_obj);
    lv_obj_set_style_text_font(label_temp, g_robam_font.FZLTHC_66.font, 0);
    lv_obj_set_style_text_color(label_temp, getThemesFontColor1(), 0);
    lv_label_set_text(label_temp, "160");
    lv_obj_align(label_temp, LV_ALIGN_CENTER, 0, 0);
    label_unit = lv_label_create(temp_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align_to(label_unit, label_temp, LV_ALIGN_OUT_RIGHT_MID, 0, -10);
    label_unit = lv_label_create(temp_obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "下温");
    lv_obj_align_to(label_unit, label_temp, LV_ALIGN_OUT_LEFT_MID, 0, 10);
    lv_cooking.label_bottom_temp = label_temp;

    cont = lv_obj_create(page);
    lv_obj_set_size(cont, 80, 270);
    lv_obj_align(cont, LV_ALIGN_LEFT_MID, 730, 15);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN_REVERSE);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

    lv_obj_t *img = lv_img_create(cont);
    lv_img_set_src(img, getThemesPath("bg_stop.png"));
    lv_obj_add_event_cb(img, btn_event_cb, LV_EVENT_CLICKED, (void *)0);
    lv_cooking.btn_start_stop = img;
    img = lv_img_create(cont);
    lv_img_set_src(img, getThemesPath("bg_preheat.png"));
    lv_obj_add_event_cb(img, btn_event_cb, LV_EVENT_CLICKED, (void *)1);
    lv_cooking.btn_preheat = img;
    img = lv_img_create(cont);
    lv_img_set_src(img, getThemesPath("bg_vapour.png"));
    lv_obj_add_event_cb(img, btn_event_cb, LV_EVENT_CLICKED, (void *)2);
    lv_cooking.btn_vapour = img;

    lv_obj_add_flag(lv_cooking.btn_preheat, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(lv_cooking.btn_vapour, LV_OBJ_FLAG_HIDDEN);
}
