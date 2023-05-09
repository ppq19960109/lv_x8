/*********************
 *      INCLUDES
 *********************/
#include "lv_custom_mode_roller.h"
#include "lv_roki/lv_roki.h"

char *vapour_model[3] = {"小", "中", "大"};

static steamoven_mode_t steam_mode[] = {
    {
        cooktype : COOK_TYPE_STEAMED,
        mode_index : 0,
        mode : 2,
        vapour : VAPOUR_NULL,
        temp : 100,
        mintemp : 30,
        maxtemp : 100,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 20,
        maxtime : 180,
        name : "营养蒸"
    },
    {
        cooktype : COOK_TYPE_STEAMED,
        mode_index : 1,
        mode : 3,
        vapour : VAPOUR_NULL,
        temp : 120,
        mintemp : 101,
        maxtemp : 150,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 180,
        name : "高温蒸"
    },
    {
        cooktype : COOK_TYPE_STEAMED,
        mode_index : 2,
        mode : 5,
        vapour : VAPOUR_BIG,
        temp : 100,
        mintemp : 0,
        maxtemp : 0,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 60,
        maxtime : 180,
        name : "澎湃蒸"
    },
    {
        cooktype : COOK_TYPE_BAKE,
        mode_index : 3,
        mode : 8,
        vapour : VAPOUR_NULL,
        temp : 160,
        mintemp : 35,
        maxtemp : 230,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "焙烤"
    },
    {
        cooktype : COOK_TYPE_BAKE,
        mode_index : 4,
        mode : 7,
        vapour : VAPOUR_NULL,
        temp : 200,
        mintemp : 35,
        maxtemp : 250,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "风焙烤"
    },
    {
        cooktype : COOK_TYPE_BAKE,
        mode_index : 5,
        mode : 11,
        vapour : VAPOUR_NULL,
        temp : 200,
        mintemp : 35,
        maxtemp : 230,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "烤烧"
    },
    {
        cooktype : COOK_TYPE_BAKE,
        mode_index : 6,
        mode : 10,
        vapour : VAPOUR_NULL,
        temp : 200,
        mintemp : 35,
        maxtemp : 230,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "强烤烧"
    },
    {
        cooktype : COOK_TYPE_BAKE,
        mode_index : 7,
        mode : 6,
        vapour : VAPOUR_NULL,
        temp : 200,
        mintemp : 35,
        maxtemp : 250,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "快热"
    },
    {
        cooktype : COOK_TYPE_BAKE,
        mode_index : 8,
        mode : 9,
        vapour : VAPOUR_NULL,
        temp : 160,
        mintemp : 35,
        maxtemp : 250,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "风扇烤"
    },
    {
        cooktype : COOK_TYPE_BAKE,
        mode_index : 9,
        mode : 14,
        vapour : VAPOUR_NULL,
        temp : 180,
        mintemp : 100,
        maxtemp : 200,
        lowertemp : 160,
        minlowertemp : 100,
        maxlowertemp : 200,
        time : 30,
        maxtime : 120,
        name : "EXP"
    },
    {
        cooktype : COOK_TYPE_FRIED,
        mode_index : 10,
        mode : 18,
        vapour : VAPOUR_NULL,
        temp : 200,
        mintemp : 35,
        maxtemp : 230,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "空气炸"
    },
    {
        cooktype : COOK_TYPE_FRIED,
        mode_index : 11,
        mode : 26,
        vapour : VAPOUR_NULL,
        temp : 200,
        mintemp : 35,
        maxtemp : 230,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "蒸汽炸"
    },
    {
        cooktype : COOK_TYPE_VAPOUR_BAKE,
        mode_index : 12,
        mode : 7,
        vapour : VAPOUR_BIG,
        temp : 200,
        mintemp : 35,
        maxtemp : 250,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "风焙烤"
    },
    {
        cooktype : COOK_TYPE_VAPOUR_BAKE,
        mode_index : 13,
        mode : 8,
        vapour : VAPOUR_BIG,
        temp : 160,
        mintemp : 35,
        maxtemp : 230,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "焙烤"
    },
    {
        cooktype : COOK_TYPE_VAPOUR_BAKE,
        mode_index : 14,
        mode : 10,
        vapour : VAPOUR_BIG,
        temp : 200,
        mintemp : 35,
        maxtemp : 230,
        lowertemp : 0,
        minlowertemp : 0,
        maxlowertemp : 0,
        time : 30,
        maxtime : 120,
        name : "强烤烧"
    },
};

steamoven_mode_t *get_steamoven_mode(int mode_index)
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
void lv_custom_get_roller_attr(steamoven_roller_t *steamoven_roller, steamoven_t *steamoven)
{
    int mode_index = (int)lv_cycle_scroll_get_selected(steamoven_roller->mode_roller)->user_data;
    int time = (int)lv_cycle_scroll_get_selected(steamoven_roller->time_roller)->user_data;

    steamoven_mode_t *steamoven_mode = get_steamoven_mode(mode_index);
    steamoven->attr[0].mode = steamoven_mode->mode;
    steamoven->attr[0].time = time;
    LV_LOG_USER("%s,mode_index:%d,mode:%d\n", __func__, mode_index, steamoven_mode->mode);

    lv_obj_t *roller_parent;
    roller_parent = lv_obj_get_parent(steamoven_roller->temp_roller);
    if (lv_obj_has_flag(roller_parent, LV_OBJ_FLAG_HIDDEN))
        steamoven->attr[0].temp = steamoven_mode->temp;
    else
    {
        int temp = (int)lv_cycle_scroll_get_selected(steamoven_roller->temp_roller)->user_data;
        steamoven->attr[0].temp = temp;
    }

    roller_parent = lv_obj_get_parent(steamoven_roller->lower_temp_roller);
    if (lv_obj_has_flag(roller_parent, LV_OBJ_FLAG_HIDDEN))
        steamoven->attr[0].lowertemp = steamoven_mode->lowertemp;
    else
    {
        int lowertemp = (int)lv_cycle_scroll_get_selected(steamoven_roller->lower_temp_roller)->user_data;
        steamoven->attr[0].lowertemp = lowertemp;
    }

    roller_parent = lv_obj_get_parent(steamoven_roller->vapour_roller);
    if (lv_obj_has_flag(roller_parent, LV_OBJ_FLAG_HIDDEN))
        steamoven->attr[0].vapour = steamoven_mode->vapour;
    else
    {
        int vapour = (int)lv_cycle_scroll_get_selected(steamoven_roller->vapour_roller)->user_data;
        steamoven->attr[0].vapour = vapour + 1;
    }
}

void lv_custom_mode_change(steamoven_roller_t *steamoven_roller, int mode_index)
{
    steamoven_mode_t *steamoven_mode = get_steamoven_mode(mode_index);
    int i;
    lv_obj_t *child, *roller_parent;
    LV_LOG_USER("%s,%d,%d,%d\n", __func__, steamoven_mode->maxtemp, steamoven_mode->maxlowertemp, steamoven_mode->vapour);
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
        lv_obj_clean(steamoven_roller->vapour_roller);
        for (i = 0; i < sizeof(vapour_model) / sizeof(vapour_model[0]); ++i)
        {
            child = mode_roller_scroll_child_create(steamoven_roller->vapour_roller, vapour_model[i], 0);
            child->user_data = (void *)i;
        }
        cycle_scroll_change(steamoven_roller->vapour_roller, steamoven_mode->vapour - 1);
        lv_obj_clear_flag(roller_parent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(roller_parent->user_data, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(roller_parent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(roller_parent->user_data, LV_OBJ_FLAG_HIDDEN);
    }

    lv_obj_clean(steamoven_roller->time_roller);

    for (i = 1; i <= steamoven_mode->maxtime; ++i)
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
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    //------------------------------
    steamoven_roller->mode_roller = lv_cycle_scroll_create(cont_row, 190, LV_PCT(100), LV_FLEX_FLOW_COLUMN, lv_cycle_scroll);
    // lv_obj_clear_flag(steamoven_roller->mode_roller, LV_OBJ_FLAG_SCROLL_ELASTIC);
    // lv_obj_clear_flag(steamoven_roller->mode_roller, LV_OBJ_FLAG_SCROLL_MOMENTUM);

    // 子对象
    for (i = 0; i < sizeof(steam_mode) / sizeof(steam_mode[0]); ++i)
    {
        if (steamoven_roller->cooktype == COOK_TYPE_MULTISTAGE || steamoven_roller->cooktype == steam_mode[i].cooktype)
        {
            child = mode_roller_scroll_child_create(steamoven_roller->mode_roller, steam_mode[i].name, 0);
            child->user_data = (void *)(steam_mode[i].mode_index);
            LV_LOG_USER("%s,name:%s mode_index:%d,mode:%d\n", __func__, steam_mode[i].name, steam_mode[i].mode_index, steam_mode[i].mode);
        }
    }
    //----------------------------------------------------------------------------
    lv_obj_t *bg_line = lv_img_create(cont_row);
    lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
    lv_obj_set_size(bg_line, 2, LV_PCT(100));
    steamoven_roller->vapour_roller = lv_cycle_scroll_unit_create(cont_row, 190, LV_PCT(100), LV_FLEX_FLOW_COLUMN, lv_cycle_scroll);

    roller_parent = lv_obj_get_parent(steamoven_roller->vapour_roller);
    roller_parent->user_data = bg_line;
    label_unit = lv_label_create(roller_parent);
    lv_obj_set_style_text_color(label_unit, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "蒸汽");
    lv_obj_align(label_unit, LV_ALIGN_CENTER, 50, 10);
    //----------------------------------------------------------------------------
    bg_line = lv_img_create(cont_row);
    lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
    lv_obj_set_size(bg_line, 2, LV_PCT(100));
    steamoven_roller->temp_roller = lv_cycle_scroll_unit_create(cont_row, 190, LV_PCT(100), LV_FLEX_FLOW_COLUMN, lv_cycle_scroll);
    roller_parent = lv_obj_get_parent(steamoven_roller->temp_roller);
    roller_parent->user_data = bg_line;
    label_unit = lv_label_create(roller_parent);
    lv_obj_set_style_text_color(label_unit, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align(label_unit, LV_ALIGN_CENTER, 60, -10);

    bg_line = lv_img_create(cont_row);
    lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
    lv_obj_set_size(bg_line, 2, LV_PCT(100));
    steamoven_roller->lower_temp_roller = lv_cycle_scroll_unit_create(cont_row, 190, LV_PCT(100), LV_FLEX_FLOW_COLUMN, lv_cycle_scroll);
    roller_parent = lv_obj_get_parent(steamoven_roller->lower_temp_roller);
    roller_parent->user_data = bg_line;
    label_unit = lv_label_create(roller_parent);
    lv_obj_set_style_text_color(label_unit, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align(label_unit, LV_ALIGN_CENTER, 60, -10);
    // lv_obj_clear_flag(steamoven_roller->lower_temp_roller, LV_OBJ_FLAG_SCROLL_ELASTIC);
    //----------------------------------------------------------------------------
    bg_line = lv_img_create(cont_row);
    lv_img_set_src(bg_line, getThemesPath("bg_line.png"));
    lv_obj_set_size(bg_line, 2, LV_PCT(100));
    steamoven_roller->time_roller = lv_cycle_scroll_unit_create(cont_row, 190, LV_PCT(100), LV_FLEX_FLOW_COLUMN, lv_cycle_scroll);
    roller_parent = lv_obj_get_parent(steamoven_roller->time_roller);
    roller_parent->user_data = bg_line;
    label_unit = lv_label_create(roller_parent);
    lv_obj_set_style_text_color(label_unit, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "min");
    lv_obj_align(label_unit, LV_ALIGN_CENTER, 65, 10);
    // lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    //---------------------------------------------------------------------
    cycle_scroll_change(steamoven_roller->mode_roller, 0);

    return cont_row;
}
#endif
