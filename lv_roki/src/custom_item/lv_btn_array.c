#include "lv_roki/lv_roki.h"

lv_obj_t *lv_btn_array_create(lv_obj_t *parent, const char count, lv_event_cb_t event_cb, lv_flex_flow_t flex_flow, int width, int height, int space)
{
    lv_obj_t *cont = lv_obj_create(parent);
    if (LV_FLEX_FLOW_COLUMN == (flex_flow & 0x01))
        lv_obj_set_size(cont, width, height * count + space);
    else
        lv_obj_set_size(cont, width * count + space, height);

    lv_obj_set_flex_flow(cont, flex_flow);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    for (int i = 0; i < count; ++i)
    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_size(btn, width, height);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0xffffff), 0);
        lv_obj_set_style_bg_opa(btn, LV_OPA_100, 0);
        lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, (void *)i);

        // lv_obj_t *label = lv_label_create(btn);
        // lv_obj_set_style_text_font(label, &lv_font_montserrat_30, 0);
        // lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
        // lv_obj_center(label);
    }
    return cont;
}

lv_obj_t *lv_btn_img_array_create(lv_obj_t *parent, const char count, lv_event_cb_t event_cb, lv_flex_flow_t flex_flow, int width, int height, int space)
{
    lv_obj_t *cont = lv_obj_create(parent);
    if (LV_FLEX_FLOW_COLUMN == (flex_flow & 0x01))
        lv_obj_set_size(cont, width, height * count + space);
    else
        lv_obj_set_size(cont, width * count + space, height);

    lv_obj_set_flex_flow(cont, flex_flow);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    for (int i = 0; i < count; ++i)
    {
        lv_obj_t *btn = lv_obj_create(cont);
        lv_obj_set_size(btn, width, height);
        lv_obj_t *img = lv_img_create(btn);
        lv_obj_center(img);
        // lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, (void *)i);
    }
    return cont;
}
