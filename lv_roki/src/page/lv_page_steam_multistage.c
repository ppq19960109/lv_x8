

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/

static multistage_para_t multistage_para[3] = {0};
static int listLastIndex = 0;
static int listResetIndex = -1;
static lv_obj_t *roller1, *roller2, *roller3;
static lv_obj_t *page_cont_row, *bottom_bar;
/**********************
 *  STATIC VARIABLES
 **********************/
static void multistage_cook_start(const int reserve_time)
{
    steamoven_t steamoven = {0};
    for (int i = 0; i < 3; ++i)
    {
        steamoven.attr[i].mode = multistage_para[i].mode;
        steamoven.attr[i].temp = multistage_para[i].temp;
        steamoven.attr[i].time = multistage_para[i].time;
        steamoven.attr[i].lowertemp = multistage_para[i].lowertemp;
        steamoven.attr[i].vapour = multistage_para[i].vapour;
    }
    steamoven.orderTime = reserve_time;
    set_cook_toServer(&steamoven);
}
static void multistage_update(void)
{
    lv_obj_t *parent;
    lv_obj_t *child[7];
    char buf[16];
    int i, j;
    LV_LOG_USER("%s,page_cont_row:%p listLastIndex:%d\n", __func__, page_cont_row, listLastIndex);
    for (i = 0; i < 3; ++i)
    {
        parent = lv_obj_get_child(page_cont_row, i * 2);
        for (j = 0; j < 7; ++j)
        {
            child[j] = lv_obj_get_child(parent, j);
        }

        if (listLastIndex > i)
        {
            lv_obj_add_flag(child[0], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[1], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(child[2], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(child[4], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(child[6], LV_OBJ_FLAG_HIDDEN);

            lv_label_set_text(child[2], workModeName(multistage_para[i].mode));

            if (multistage_para[i].vapour == 0)
                lv_obj_add_flag(child[3], LV_OBJ_FLAG_HIDDEN);
            else
            {
                lv_obj_clear_flag(child[3], LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text(lv_obj_get_child(child[3], 0), vapour_model[multistage_para[i].vapour - 1]);
            }

            sprintf(buf, "%d", multistage_para[i].temp);
            lv_label_set_text(lv_obj_get_child(child[4], 0), buf);

            if (multistage_para[i].lowertemp == 0)
                lv_obj_add_flag(child[5], LV_OBJ_FLAG_HIDDEN);
            else
            {
                lv_obj_clear_flag(child[5], LV_OBJ_FLAG_HIDDEN);
                sprintf(buf, "%d", multistage_para[i].lowertemp);
                lv_label_set_text(lv_obj_get_child(child[5], 0), buf);
            }

            sprintf(buf, "%d", multistage_para[i].time);
            lv_label_set_text(lv_obj_get_child(child[6], 0), buf);
        }
        else
        {
            lv_obj_clear_flag(child[0], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(child[1], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[2], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[3], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[4], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[5], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(child[6], LV_OBJ_FLAG_HIDDEN);
        }
        if (listLastIndex >= i)
        {
            lv_obj_set_style_opa(parent, LV_OPA_100, 0);
        }
        else
        {
            lv_obj_set_style_opa(parent, LV_OPA_40, 0);
        }
    }
}

void multistage_update_from_select(steamoven_t *steamoven)
{
    if (listResetIndex >= 0)
    {
        multistage_para[listResetIndex].mode = steamoven->attr[0].mode;
        multistage_para[listResetIndex].temp = steamoven->attr[0].temp;
        multistage_para[listResetIndex].time = steamoven->attr[0].time;
        multistage_para[listResetIndex].lowertemp = steamoven->attr[0].lowertemp;
        multistage_para[listResetIndex].vapour = steamoven->attr[0].vapour;
    }
    else
    {
        multistage_para[listLastIndex].mode = steamoven->attr[0].mode;
        multistage_para[listLastIndex].temp = steamoven->attr[0].temp;
        multistage_para[listLastIndex].time = steamoven->attr[0].time;
        multistage_para[listLastIndex].lowertemp = steamoven->attr[0].lowertemp;
        multistage_para[listLastIndex].vapour = steamoven->attr[0].vapour;
        ++listLastIndex;
    }
    multistage_update();
}

static void page_update_cb(void *arg)
{
}
static void add_click_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    if (user_data == listLastIndex)
    {
        listResetIndex = -1;
    }
    else
    {
        listResetIndex = user_data;
    }
    lv_100ask_page_manager_set_open_page(NULL, "page_multistage_select");
}
static void btn_click_event_cb(lv_event_t *e)
{
    // lv_obj_t *target = lv_event_get_target(e);
    long user_data = (long)lv_event_get_user_data(e);
    int index = user_data / 2;
    LV_LOG_USER("%s,code:%d listLastIndex:%d index:%d\n", __func__, e->code, listLastIndex, index);
    if (user_data % 2 == 0)
    {
        if (index < 2)
            memcpy(&multistage_para[index], &multistage_para[index + 1], sizeof(multistage_para_t) * (2 - index));
        --listLastIndex;
        memset(&multistage_para[listLastIndex], 0, sizeof(multistage_para_t) * (3 - index));
        multistage_update();
    }
    else
    {
    }
}
static void bottom_bar_event_cb(lv_event_t *e)
{
    int user_data = (int)lv_event_get_user_data(e);
    LV_LOG_USER("%s,code:%d user_data:%d\n", __func__, e->code, user_data);
    if (user_data == 0)
    {
    }
    else
    {
        multistage_cook_start(0);
        // lv_100ask_page_manager_set_open_page(NULL, "page_cook_tab");
    }
}
void lv_page_multistage_init(lv_obj_t *page)
{
    LV_LOG_USER("%s...", __func__);
    lv_100ask_page_manager_page_t *manager_page = (lv_100ask_page_manager_page_t *)page;
    manager_page->page_update_cb = page_update_cb;

    lv_page_back_bar_init(page, "返回", NULL, NULL);
    bottom_bar = lv_page_bottom_bar_init(page, NULL, "开始烹饪", bottom_bar_event_cb);
    lv_obj_t *label_total_time = lv_label_create(bottom_bar);
    lv_obj_set_style_text_font(label_total_time, g_robam_font.FZLTHJW_32.font, 0);
    lv_obj_set_style_text_color(label_total_time, getThemesFontColor1(), 0);
    lv_label_set_text(label_total_time, "总时长");
    lv_obj_align(label_total_time, LV_ALIGN_LEFT_MID, 40, 0);
    label_total_time = lv_custom_time_create(bottom_bar, 332, g_robam_font.FZLTHC_48.font);
    lv_obj_align(label_total_time, LV_ALIGN_LEFT_MID, 150, 0);

    page_cont_row = lv_obj_create(page);
    lv_obj_set_size(page_cont_row, 750, 250);
    lv_obj_align(page_cont_row, LV_ALIGN_TOP_MID, 0, 120);
    lv_obj_set_flex_flow(page_cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(page_cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    //------------------------------
    char buf[10];
    for (int i = 0; i < 3; ++i)
    {
        if (i > 0)
        {
            lv_obj_t *bg_line = lv_img_create(page_cont_row);
            lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
            lv_obj_set_size(bg_line, 2, LV_PCT(100));
        }
        lv_obj_t *item_obj = lv_obj_create(page_cont_row);
        lv_obj_set_flex_flow(item_obj, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(item_obj, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        // lv_obj_set_style_bg_opa(item_obj, LV_OPA_100, 0);
        lv_obj_set_size(item_obj, 200, LV_PCT(100));
        // lv_obj_set_style_bg_color(item_obj, lv_color_hex(0xff0000), 0);
        // lv_obj_set_style_bg_opa(item_obj, LV_OPA_100, 0);

        lv_obj_t *label = lv_label_create(item_obj); // 0
        lv_obj_set_style_text_font(label, g_robam_font.FZLTHJW_48.font, 0);
        lv_obj_set_style_text_color(label, getThemesFontColor1(), 0);
        sprintf(buf, "第%d段", i + 1);
        lv_label_set_text(label, buf);

        lv_obj_t *img = lv_img_create(item_obj); // 1
        lv_img_set_src(img, getThemesPath("bg_multi_add.png"));
        //-------------------------------------------------------
        label = lv_label_create(item_obj); // 2
        lv_obj_set_style_text_font(label, g_robam_font.FZLTHJW_44.font, 0);
        lv_obj_set_style_text_color(label, getThemesFontColor1(), 0);
        lv_label_set_text(label, workModeName(4));

        label = lv_custom_vapour_create(item_obj, 2, g_robam_font.FZLTHC_44.font); // 3
        label = lv_custom_temp_create(item_obj, 180, g_robam_font.FZLTHC_44.font); // 4 temp
        label = lv_custom_temp_create(item_obj, 280, g_robam_font.FZLTHC_44.font); // 5 lowertemp

        label = lv_custom_time_create(item_obj, 333, g_robam_font.FZLTHC_44.font); // 6

        lv_obj_add_event_cb(item_obj, add_click_event_cb, LV_EVENT_CLICKED, (void *)i);
    }
    //------------------------------------------------------------------
    memset(multistage_para, 0, sizeof(multistage_para));
    listLastIndex = 0;
    multistage_update();
}
