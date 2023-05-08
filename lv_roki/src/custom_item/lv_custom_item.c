

/*********************
 *      INCLUDES
 *********************/
#include "lv_roki/lv_roki.h"

/*********************
 *      DEFINES
 *********************/
static char roller_options[1024] = {0};
/**********************
 *  STATIC VARIABLES
 **********************/
char *temp_roller_options(int min_temp, int max_temp)
{
    int i = 0, index = 0;
    for (i = min_temp; i < max_temp; ++i)
    {
        sprintf(&roller_options[index], "%d\n", i);
        if (i < 10)
            index += 2;
        else if (i < 100)
            index += 3;
        else
            index += 4;
    }
    sprintf(&roller_options[index], "%d", max_temp);
    return roller_options;
}
char *time_roller_options(int max_time)
{
    int i = 0, index = 0;
    if (max_time <= 0)
        max_time = 300;
    for (i = 1; i <= 120; ++i)
    {
        sprintf(&roller_options[index], "%d\n", i);
        if (i < 10)
            index += 2;
        else if (i < 100)
            index += 3;
        else
            index += 4;
    }
    for (i = 125; i < max_time; i += 5)
    {
        sprintf(&roller_options[index], "%d\n", i);
        index += 4;
    }
    sprintf(&roller_options[index], "%d", max_time);
    LV_LOG_USER("%s,index:%d", __func__, index);
    return roller_options;
}

lv_obj_t *lv_custom_image_button_create(lv_obj_t *parent, const void *img_src, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *image_button = lv_obj_create(parent);

    lv_obj_t *img = lv_img_create(image_button);
    lv_img_set_src(img, img_src);
    lv_obj_align(img, LV_ALIGN_CENTER, x, y);
    return image_button;
}
lv_obj_t *lv_custom_text_btn_create(lv_obj_t *parent, const char *text)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 140, 50);
    lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
    lv_obj_set_style_radius(btn, 25, 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    lv_label_set_recolor(label, true);
    return btn;
}

static void set_angle(void *img, int32_t v)
{
    lv_img_set_angle(img, v);
}
lv_obj_t *lv_rotate_anim(lv_obj_t *obj, const int run)
{
    lv_anim_t *anim = lv_anim_get(obj, NULL);
    LV_LOG_USER("%s,run:%d anim:%p\n", __func__, run, anim);
    if (run)
    {
        if (anim == NULL)
        {
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, obj);
            lv_anim_set_exec_cb(&a, set_angle);
            lv_anim_set_values(&a, 0, 3600);
            lv_anim_set_time(&a, 8000);
            lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
            lv_anim_start(&a);
        }
    }
    else
    {
        if (anim != NULL)
            lv_anim_del(obj, set_angle);
    }
    return NULL;
}

lv_obj_t *radiobutton_create(lv_obj_t *parent, const char *txt)
{
    lv_obj_t *obj = lv_checkbox_create(parent);
    lv_checkbox_set_text(obj, txt);
    lv_obj_set_style_text_font(obj, &lv_font_SiYuanHeiTi_Normal_30, 0);
    lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), 0);
    // lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);
    // lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_INDICATOR | LV_STATE_CHECKED);
    // lv_obj_set_style_bg_color(obj, lv_color_hex(themesTextColor), LV_PART_INDICATOR);
    // lv_obj_set_style_border_color(obj, lv_color_hex(themesTextColor), LV_PART_INDICATOR);
    // lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR);
    // lv_obj_set_style_pad_column(obj, 20, 0);

    lv_obj_set_style_pad_all(obj, 3, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(obj, lv_color_hex(themesPopupWindowColor), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xE68855), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR);
    lv_obj_set_style_border_color(obj, lv_color_hex(0xE68855), LV_PART_INDICATOR);
    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);
    // LV_SYMBOL_OK themesImagesPath"icon_checked.png"
    lv_obj_set_style_bg_img_src(obj, LV_SYMBOL_OK, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_pad_gap(obj, 10, 0);
    return obj;
}
lv_obj_t *lv_custom_btn_array_create(lv_obj_t *parent, const char *text[], const char count, lv_event_cb_t event_cb)
{
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 140, 50 * count + 55);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    for (long i = 0; i < count; ++i)
    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 140, 50);
        lv_obj_set_style_bg_color(btn, lv_color_hex(themesTextColor2), 0);
        lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
        lv_obj_set_style_radius(btn, 25, 0);
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, (void *)i);

        lv_obj_t *label = lv_label_create(btn);
        lv_obj_set_style_text_font(label, &lv_font_SiYuanHeiTi_Normal_30, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
        lv_label_set_text(label, text[i]);
        lv_obj_center(label);
    }
    return cont;
}

lv_obj_t *lv_divider_create(lv_obj_t *parent)
{
    lv_obj_t *divider = lv_obj_create(parent);
    lv_obj_set_size(divider, LV_PCT(100), 2);
    lv_obj_set_style_bg_opa(divider, 15, 0);
    lv_obj_set_style_bg_color(divider, lv_color_hex(0xE7E7E7), 0);
    return divider;
}

lv_obj_t *lv_custom_vapour_create(lv_obj_t *parent, int vapour, const lv_font_t *font, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_t *label = lv_label_create(obj);
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, getThemesFontColor1(), 0);

    lv_label_set_text(label, vapour_model[vapour]);
    lv_obj_t *label_unit = lv_label_create(obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "蒸汽");
    lv_obj_align_to(label_unit, label, LV_ALIGN_OUT_RIGHT_MID, x_ofs, y_ofs);
    return obj;
}

lv_obj_t *lv_custom_temp_create(lv_obj_t *parent, int temp, const lv_font_t *font, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_t *label = lv_label_create(obj);
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, getThemesFontColor1(), 0);
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", temp);
    lv_label_set_text(label, buf);
    lv_obj_t *label_unit = lv_label_create(obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align_to(label_unit, label, LV_ALIGN_OUT_RIGHT_MID, x_ofs, y_ofs);
    return obj;
}

lv_obj_t *lv_custom_temp_fix_width_create(lv_obj_t *parent, int temp, const lv_font_t *font, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_coord_t w, const char *unit_before)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, w, LV_SIZE_CONTENT);
    lv_obj_t *label = lv_label_create(obj);
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, getThemesFontColor1(), 0);
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", temp);
    lv_label_set_text(label, buf);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t *label_unit = lv_label_create(obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "℃");
    lv_obj_align_to(label_unit, label, LV_ALIGN_OUT_RIGHT_MID, x_ofs, y_ofs);
    label_unit = lv_label_create(obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, unit_before);
    lv_obj_align_to(label_unit, label, LV_ALIGN_OUT_LEFT_MID, 0, -y_ofs);
    return obj;
}

lv_obj_t *lv_custom_time_create(lv_obj_t *parent, int time, const lv_font_t *font, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_t *label = lv_label_create(obj);
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, getThemesFontColor1(), 0);
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", time);
    lv_label_set_text(label, buf);
    lv_obj_t *label_unit = lv_label_create(obj);
    lv_obj_set_style_text_color(label_unit, getThemesFontColor1(), 0);
    lv_obj_set_style_text_font(label_unit, g_robam_font.FZLTHC_30.font, 0);
    lv_label_set_text(label_unit, "min");
    lv_obj_align_to(label_unit, label, LV_ALIGN_OUT_RIGHT_MID, x_ofs, y_ofs);
    return obj;
}

lv_obj_t *lv_page_indicator_create(lv_obj_t *parent, int count)
{
    lv_obj_t *cont_row = lv_obj_create(parent);
    lv_obj_set_size(cont_row, 15 * count + 15 * (count - 1), 15);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *element;
    for (int i = 0; i < count; ++i)
    {
        element = lv_btn_create(cont_row);
        lv_obj_set_size(element, 15, 15);
        lv_obj_set_style_bg_color(element, lv_color_hex(0x6F6F6F), 0);
        lv_obj_set_style_bg_color(element, lv_color_hex(0xffffff), LV_STATE_CHECKED);
        lv_obj_set_style_bg_opa(element, LV_OPA_100, 0);
        lv_obj_set_style_radius(element, LV_RADIUS_CIRCLE, 0);
    }
    return cont_row;
}
void lv_page_indicator_check(lv_obj_t *page_indicator, int index)
{
    int count = lv_obj_get_child_cnt(page_indicator);
    lv_obj_t *child;
    for (int i = 0; i < count; ++i)
    {
        child = lv_obj_get_child(page_indicator, i);
        if (i == index)
        {
            lv_obj_add_state(child, LV_STATE_CHECKED);
        }
        else if (lv_obj_has_state(child, LV_STATE_CHECKED))
        {
            lv_obj_clear_state(child, LV_STATE_CHECKED);
        }
    }
}
