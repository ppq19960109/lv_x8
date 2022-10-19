

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC VARIABLES
 **********************/
#if LV_100ASK_PAGE_MANAGER_COSTOM_ANIMARION
/*open page anim*/
static void open_page_anim(lv_obj_t * obj)
{
    /*Do something with LVGL*/
    // LV_LOG_USER("open page anim.");
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
}

/*close page anim*/
static void close_page_anim(lv_obj_t * obj)
{
    /*Do something with LVGL*/
    // LV_LOG_USER("close page anim.");
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
}
#endif
lv_obj_t * lv_custom_image_button_create(lv_obj_t * parent, const void * img_src, lv_coord_t width, lv_coord_t height,
                                         lv_coord_t x, lv_coord_t y)
{
    lv_obj_t * image_button = lv_obj_create(parent);
    lv_obj_set_size(image_button, width, height);

    lv_obj_t * img = lv_img_create(image_button);
    lv_img_set_src(img, img_src);
    lv_obj_align(img, LV_ALIGN_CENTER, x, y);
    return image_button;
}
static void set_angle(void * img, int32_t v)
{
    lv_img_set_angle(img, v);
}
lv_obj_t * lv_rotate_anim(lv_obj_t * obj, const int run)
{
    // LV_LOG_USER("%s,run:%d\n", __func__, run);
    if(run) {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_exec_cb(&a, set_angle);
        lv_anim_set_values(&a, 0, 3600);
        lv_anim_set_time(&a, 6000);
        lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
        lv_anim_start(&a);
    } else {
        lv_anim_del(obj, set_angle);
    }
    return NULL;
}
static void obj_event_cb(lv_event_t * e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t * target = lv_event_get_current_target(e); // lv_event_get_target(e);
    lv_obj_set_style_bg_color(target, lv_color_hex(0xff0000), LV_STATE_PRESSED);

    lv_obj_t * label = lv_event_get_user_data(e);
    lv_event_send(label, LV_EVENT_CLICKED, 0);
}

static void init_main_page(lv_obj_t * page)
{
    lv_obj_t * cont_row = lv_obj_create(page);

    lv_obj_set_size(cont_row, 292 * 3 + 100, 360);
    lv_obj_align(cont_row, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * home_hood = lv_img_create(cont_row);
    lv_img_set_src(home_hood, themesImagesPath "home_hood_background.png");
    lv_obj_t * label_title1 = lv_label_create(home_hood);
    lv_obj_set_style_text_font(label_title1, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_label_set_text(label_title1, "烟机灶具");
    lv_obj_set_style_text_color(label_title1, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_title1, LV_ALIGN_TOP_MID, 0, 45);
    lv_100ask_page_manager_set_load_page_event(home_hood, NULL, "page_hood");

    lv_obj_t * home_steamoven = lv_img_create(cont_row);
    lv_img_set_src(home_steamoven, themesImagesPath "home_steamoven_background.png");
    lv_obj_t * label_title2 = lv_label_create(home_steamoven);
    lv_obj_set_style_text_font(label_title2, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_label_set_text(label_title2, "蒸烤箱");
    lv_obj_set_style_text_color(label_title2, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_title2, LV_ALIGN_TOP_MID, 0, 45);
    lv_100ask_page_manager_set_load_page_event(home_steamoven, NULL, "page_steamoven");

    lv_obj_t * home_smartrecipes = lv_img_create(cont_row);
    lv_img_set_src(home_smartrecipes, themesImagesPath "home_smartrecipes_background.png");
    lv_obj_t * label_title3 = lv_label_create(home_smartrecipes);
    lv_obj_set_style_text_font(label_title3, &lv_font_SiYuanHeiTi_Normal_40, 0);
    lv_label_set_text(label_title3, "智慧菜谱");
    lv_obj_set_style_text_color(label_title3, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_title3, LV_ALIGN_TOP_MID, 0, 45);
}
void lv_test_widgets(void)
{
    LV_LOG_USER("lv_test_widgets...");
    // lv_100ask_pinyin_ime_simple_test();
    // lv_100ask_page_manager_simple_test();
    // lv_100ask_demo_layer();
    // return 0;

    lv_obj_t * win_bg = lv_img_create(lv_scr_act());
    lv_img_set_src(win_bg, themesImagesPath "window-background.png");

    lv_obj_t * home_bar = lv_obj_create(lv_scr_act());
    lv_obj_set_size(home_bar, 120, LV_PCT(100));
    lv_obj_set_align(home_bar, LV_ALIGN_RIGHT_MID);

    lv_obj_t * newline2 = lv_img_create(home_bar);
    lv_img_set_src(newline2, themesImagesPath "icon_newline2.png");
    lv_obj_set_size(newline2, 2, LV_SIZE_CONTENT);
    lv_obj_set_align(newline2, LV_ALIGN_LEFT_MID);

    lv_obj_t * clock_text = lv_label_create(home_bar);
    lv_obj_align(clock_text, LV_ALIGN_TOP_MID, 0, 17);
    lv_obj_set_style_text_color(clock_text, lv_color_hex(themesTextColor2), 0);
    lv_label_set_text(clock_text, "12:45");

    lv_obj_t * icon_newline1 = lv_img_create(home_bar);
    lv_img_set_src(icon_newline1, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline1, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline1, LV_ALIGN_TOP_MID, 0, 54);
    lv_obj_t * icon_wifi = lv_img_create(home_bar);
    lv_img_set_src(icon_wifi, themesImagesPath "icon_wifi_connected.png");
    lv_obj_align(icon_wifi, LV_ALIGN_TOP_MID, 0, 80);

    lv_obj_t * icon_newline2 = lv_img_create(home_bar);
    lv_img_set_src(icon_newline2, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline2, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline2, LV_ALIGN_TOP_MID, 0, 133);
    lv_obj_t * icon_set = lv_img_create(home_bar);
    lv_img_set_src(icon_set, themesImagesPath "icon_set.png");
    lv_obj_align(icon_set, LV_ALIGN_TOP_MID, 0, 163);

    lv_obj_t * icon_newline3 = lv_img_create(home_bar);
    lv_img_set_src(icon_newline3, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline3, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline3, LV_ALIGN_TOP_MID, 0, 223);
    lv_obj_t * icon_alarm = lv_img_create(home_bar);
    lv_img_set_src(icon_alarm, themesImagesPath "icon_alarm.png");
    lv_obj_align(icon_alarm, LV_ALIGN_TOP_MID, 0, 248);

    lv_obj_t * icon_newline4 = lv_img_create(home_bar);
    lv_img_set_src(icon_newline4, themesImagesPath "icon_newline.png");
    lv_obj_set_size(icon_newline4, LV_SIZE_CONTENT, 2);
    lv_obj_align(icon_newline4, LV_ALIGN_TOP_MID, 0, 314);
    lv_obj_t * icon_standby = lv_img_create(home_bar);
    lv_img_set_src(icon_standby, themesImagesPath "icon_standby.png");
    lv_obj_align(icon_standby, LV_ALIGN_TOP_MID, 0, 339);
    //****************************************************
    lv_obj_t * home = lv_obj_create(lv_scr_act());
    // lv_obj_refresh_style(home, LV_PART_ANY, LV_STYLE_PROP_ANY);
    // lv_obj_invalidate(home);
    // lv_obj_update_layout(home);
    // static lv_style_t style_clear;
    // lv_style_init(&style_clear);
    // lv_style_set_bg_opa(&style_clear, LV_OPA_0);
    // lv_style_set_border_opa(&style_clear, LV_OPA_0);
    // lv_obj_add_style(home, &style_clear, 0);

    lv_obj_set_size(home, 1160, LV_PCT(100));
    lv_obj_set_align(home, LV_ALIGN_LEFT_MID);
    //****************************************************
    lv_obj_t * page_manager = lv_100ask_page_manager_create(home);

    lv_obj_t * main_page = lv_100ask_page_manager_page_create(page_manager, "main_page");
    lv_obj_t * page_hood = lv_100ask_page_manager_page_create(page_manager, "page_hood");
    lv_obj_t * page_steamoven = lv_100ask_page_manager_page_create(page_manager, "page_steamoven");

    lv_100ask_page_manager_set_page_init(main_page, init_main_page);
    lv_100ask_page_manager_set_page_init(page_hood, lv_page_hood_init);
    lv_100ask_page_manager_set_page_init(page_steamoven, lv_page_steamoven_init);
#if LV_100ASK_PAGE_MANAGER_COSTOM_ANIMARION
    lv_100ask_page_manager_set_open_page_anim(main_page, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(main_page, close_page_anim);
    lv_100ask_page_manager_set_open_page_anim(page_hood, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_hood, close_page_anim);
        lv_100ask_page_manager_set_open_page_anim(page_steamoven, open_page_anim);
    lv_100ask_page_manager_set_close_page_anim(page_steamoven, close_page_anim);
#endif
    lv_100ask_page_manager_set_main_page(page_manager, main_page);
    lv_100ask_page_manager_set_open_page(NULL, "main_page");
}
