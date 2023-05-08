/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"

char *vapour_model[3] = {"小", "中", "大"};

static steamoven_mode_t steam_mode[] = {
    {
        mode_index : 0,
        mode : 40,
        temp : 150,
        mintemp : 50,
        maxtemp : 200,
        time : 60,
        maxtime : 0,
        name : "营养蒸"
    },
    {
        mode_index : 1,
        mode : 4,
        temp : 120,
        mintemp : 101,
        maxtemp : 120,
        time : 20,
        maxtime : 0,
        name : "高温蒸"
    },
    {
        mode_index : 2,
        mode : 35,
        temp : 2,
        mintemp : 1,
        maxtemp : 10,
        vapour : 1,
        time : 60,
        maxtime : 0,
        name : "澎湃蒸"
    },
};

steamoven_mode_t *get_steamoven_mode(unsigned char mode_index)
{
    for (int i = 0; i < sizeof(steam_mode) / sizeof(steam_mode[0]); ++i)
    {
        if (steam_mode[i].mode_index == mode_index)
            return &steam_mode[i];
    }
    return NULL;
}
lv_obj_t *mode_roller_scroll_child_create(lv_obj_t *parent, const char *text, const int num)
{
    lv_obj_t *btn = lv_obj_create(parent);
    lv_obj_set_size(btn, LV_PCT(100), LV_PCT(33));

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label, g_robam_font.FZLTHC_44.font, 0);
    lv_obj_set_style_text_font(label, g_robam_font.FZLTHC_60.font, LV_STATE_CHECKED);
    if (text == NULL)
        lv_label_set_text_fmt(label, "%d", num);
    else
        lv_label_set_text(label, text);

    lv_obj_center(label);
    return btn;
}
#if 1
void lv_custom_mode_change(steamoven_roller_t *steamoven_roller, unsigned char mode_index)
{
    steamoven_mode_t *steamoven_mode = get_steamoven_mode(mode_index);
    int i;
    lv_obj_t *child, *roller_parent;

    roller_parent = lv_obj_get_parent(steamoven_roller->temp_roller);
    if (steamoven_mode->maxtemp != 0)
    {
        lv_obj_clean(steamoven_roller->temp_roller);
        for (i = steamoven_mode->mintemp; i <= steamoven_mode->maxtemp; ++i)
        {
            child = mode_roller_scroll_child_create(steamoven_roller->temp_roller, NULL, i);
            child->user_data = (void *)i;
        }
        cycle_scroll_change(steamoven_roller->temp_roller, steamoven_mode->temp - steamoven_mode->mintemp);
        lv_obj_clear_flag(roller_parent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(roller_parent->user_data, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(roller_parent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(roller_parent->user_data, LV_OBJ_FLAG_HIDDEN);
    }

    roller_parent = lv_obj_get_parent(steamoven_roller->lower_temp_roller);
    if (steamoven_mode->maxlowertemp != 0)
    {
        lv_obj_clean(steamoven_roller->lower_temp_roller);
        for (i = steamoven_mode->minlowertemp; i <= steamoven_mode->maxlowertemp; ++i)
        {
            child = mode_roller_scroll_child_create(steamoven_roller->lower_temp_roller, NULL, i);
            child->user_data = (void *)i;
        }
        cycle_scroll_change(steamoven_roller->lower_temp_roller, steamoven_mode->lowertemp - steamoven_mode->minlowertemp);
        lv_obj_clear_flag(roller_parent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(roller_parent->user_data, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(roller_parent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(roller_parent->user_data, LV_OBJ_FLAG_HIDDEN);
    }

    roller_parent = lv_obj_get_parent(steamoven_roller->vapour_roller);
    if (steamoven_mode->vapour != 0)
    {
        cycle_scroll_change(steamoven_roller->vapour_roller, steamoven_mode->vapour);
        lv_obj_clear_flag(roller_parent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(roller_parent->user_data, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(roller_parent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(roller_parent->user_data, LV_OBJ_FLAG_HIDDEN);
    }

    lv_obj_clean(steamoven_roller->time_roller);
    int time = 0;
    if (steamoven_mode->maxtime <= 0)
        time = 200;
    for (i = 1; i <= time; ++i)
    {
        child = mode_roller_scroll_child_create(steamoven_roller->time_roller, NULL, i);
        child->user_data = (void *)i;
    }
    cycle_scroll_change(steamoven_roller->time_roller, steamoven_mode->time - 1);
}

lv_obj_t *lv_custom_mode_roller_create(lv_obj_t *parent, steamoven_roller_t *steamoven_roller, lv_cycle_scroll_t *lv_cycle_scroll)
{
    int i;
    lv_obj_t *child, *roller_parent, *label_unit;

    lv_obj_t *cont_row = lv_obj_create(parent);
    lv_obj_set_size(cont_row, 800, 242);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    //------------------------------
    steamoven_roller->mode_roller = lv_cycle_scroll_create(cont_row, 180, LV_PCT(100), LV_FLEX_FLOW_COLUMN, lv_cycle_scroll);
    lv_obj_clear_flag(steamoven_roller->mode_roller, LV_OBJ_FLAG_SCROLL_ELASTIC);
    // lv_obj_clear_flag(steamoven_roller->mode_roller, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    // lv_obj_add_flag(steamoven_roller->mode_roller, LV_OBJ_FLAG_SCROLL_ONE);
    // 子对象
    for (i = 0; i < sizeof(steam_mode) / sizeof(steam_mode[0]); ++i)
    {
        if (steamoven_roller->cooktype == steam_mode[i].cooktype)
        {
            child = mode_roller_scroll_child_create(steamoven_roller->mode_roller, steam_mode[i].name, 0);
            child->user_data = (void *)(steam_mode[i].mode_index);
        }
    }
    //----------------------------------------------------------------------------
    lv_obj_t *bg_line = lv_img_create(cont_row);
    lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
    lv_obj_set_size(bg_line, 2, LV_PCT(100));
    steamoven_roller->vapour_roller = lv_cycle_scroll_unit_create(cont_row, 160, LV_PCT(100), LV_FLEX_FLOW_COLUMN, lv_cycle_scroll);
    for (i = 0; i < sizeof(vapour_model) / sizeof(vapour_model[0]); ++i)
    {
        child = mode_roller_scroll_child_create(steamoven_roller->vapour_roller, vapour_model[i], 0);
        child->user_data = (void *)i;
    }
    roller_parent = lv_obj_get_parent(steamoven_roller->vapour_roller);
    roller_parent->user_data = bg_line;
    label_unit = lv_label_create(roller_parent);
    lv_obj_set_style_text_color(label_unit, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "蒸汽");
    lv_obj_align(label_unit, LV_ALIGN_CENTER, 80, -10);
    //----------------------------------------------------------------------------
    bg_line = lv_img_create(cont_row);
    lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
    lv_obj_set_size(bg_line, 2, LV_PCT(100));
    steamoven_roller->temp_roller = lv_cycle_scroll_unit_create(cont_row, 160, LV_PCT(100), LV_FLEX_FLOW_COLUMN, lv_cycle_scroll);
    roller_parent = lv_obj_get_parent(steamoven_roller->temp_roller);
    roller_parent->user_data = bg_line;
    label_unit = lv_label_create(roller_parent);
    lv_obj_set_style_text_color(label_unit, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align(label_unit, LV_ALIGN_CENTER, 80, -10);

    bg_line = lv_img_create(cont_row);
    lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
    lv_obj_set_size(bg_line, 2, LV_PCT(100));
    steamoven_roller->lower_temp_roller = lv_cycle_scroll_unit_create(cont_row, 160, LV_PCT(100), LV_FLEX_FLOW_COLUMN, lv_cycle_scroll);
    roller_parent = lv_obj_get_parent(steamoven_roller->temp_roller);
    roller_parent->user_data = bg_line;
    label_unit = lv_label_create(roller_parent);
    lv_obj_set_style_text_color(label_unit, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align(label_unit, LV_ALIGN_CENTER, 80, -10);
    // lv_obj_clear_flag(steamoven_roller->lower_temp_roller, LV_OBJ_FLAG_SCROLL_ELASTIC);
    //----------------------------------------------------------------------------
    bg_line = lv_img_create(cont_row);
    lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
    lv_obj_set_size(bg_line, 2, LV_PCT(100));
    steamoven_roller->time_roller = lv_cycle_scroll_unit_create(cont_row, 160, LV_PCT(100), LV_FLEX_FLOW_COLUMN, lv_cycle_scroll);
    roller_parent = lv_obj_get_parent(steamoven_roller->temp_roller);
    roller_parent->user_data = bg_line;
    label_unit = lv_label_create(roller_parent);
    lv_obj_set_style_text_color(label_unit, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "min");
    lv_obj_align(label_unit, LV_ALIGN_CENTER, 80, -10);
    // lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    //---------------------------------------------------------------------
    cycle_scroll_change(steamoven_roller->mode_roller, 0);

    return cont_row;
}
#endif
