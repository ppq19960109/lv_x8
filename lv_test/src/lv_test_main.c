

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lv_test_main.h"

LV_FONT_DECLARE(lv_font_SiYuanHeiTi_Normal_30);
/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static void obj_event_cb(lv_event_t * e)
{
    LV_LOG_USER("%s,code:%d\n", __func__, e->code);
    lv_obj_t * target = lv_event_get_current_target(e); // lv_event_get_target(e);
    lv_obj_set_style_bg_color(target, lv_color_hex(0xff0000), LV_STATE_PRESSED);

    lv_obj_t * label = lv_event_get_user_data(e);
    lv_event_send(label, LV_EVENT_CLICKED, 0);
}

void lv_test_widgets(void)
{
    LV_LOG_USER("lv_test_widgets...");
    // lv_100ask_pinyin_ime_simple_test();
    // lv_100ask_page_manager_simple_test();
    // lv_100ask_demo_layer();
    // return 0;
    lv_obj_t * scr = lv_scr_act();

    lv_obj_t * win_bg = lv_img_create(scr);
    lv_img_set_src(win_bg, themesImagesPath "window-background.png");

    static lv_style_t style_bg;
    // lv_style_set_bg_color(&style_bg, lv_color_hex(0xe47833));
    lv_style_set_bg_opa(&style_bg, LV_OPA_TRANSP);

    lv_obj_t * home_bar = lv_obj_create(scr);
    lv_obj_remove_style_all(home_bar);

    lv_obj_set_size(home_bar, 120, LV_PCT(100));
    lv_obj_set_align(home_bar, LV_ALIGN_RIGHT_MID);

    lv_obj_t * clock_text = lv_label_create(home_bar);
    lv_obj_align(clock_text, LV_ALIGN_TOP_MID, 0, 17);
    lv_obj_set_style_text_color(clock_text, lv_color_hex(themesTextColor2), 0);
    lv_label_set_text(clock_text, "12:45");

    lv_obj_t * newline2 = lv_img_create(home_bar);
    lv_img_set_src(newline2, themesImagesPath "icon_newline2.png");
    lv_obj_set_size(newline2, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_align(newline2, LV_ALIGN_LEFT_MID);

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
    lv_obj_t * home = lv_obj_create(scr);
    lv_obj_remove_style_all(home);
    // lv_obj_add_style(home, &style_bg, LV_PART_MAIN);

    lv_obj_set_size(home, 1160, LV_PCT(100));
    lv_obj_set_align(home, LV_ALIGN_LEFT_MID);
    // lv_obj_set_style_bg_color(home, lv_color_hex(0x00ff00), LV_PART_MAIN);

    // static const char * map[] = {LV_SYMBOL_LEFT, "1", "2", "3", "4", "5", LV_SYMBOL_RIGHT, ""};
    // lv_obj_t * btnm = lv_btnmatrix_create(lv_scr_act());
    // lv_btnmatrix_set_map(btnm, map);

    // lv_obj_t * imgbtn1 = lv_imgbtn_create(lv_scr_act());
    // lv_imgbtn_set_src(imgbtn1, LV_IMGBTN_STATE_RELEASED, themesImagesPath "icon_wifi_connected.png", NULL, NULL);
    // lv_imgbtn_set_src(imgbtn1, LV_IMGBTN_STATE_PRESSED, themesImagesPath "icon_wifi_disconnect.png", NULL, NULL);
    // lv_obj_update_layout(lv_scr_act());
    // printf("imgbtn1 width:%d height:%d\n", lv_obj_get_width(imgbtn1), lv_obj_get_height(imgbtn1));
    // printf("icon_wifi width:%d height:%d\n", lv_obj_get_width(icon_wifi), lv_obj_get_height(icon_wifi));
}
