#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/display/drm.h"
#include "lv_drivers/indev/evdev.h"
#include "lv_roki/lv_roki.h"

#define DISP_BUF_SIZE (480 * 854)
pthread_mutex_t g_mutex;

static void feedback_cb(struct _lv_indev_drv_t *lv_indev_drv, uint8_t event_code)
{
    if (LV_EVENT_CLICKED == event_code || LV_EVENT_RELEASED == event_code) //|| LV_EVENT_PRESSED == event_code
    {
        printf("%s,code:%d............\n", __func__, event_code);
        lv_sleep_wakeup();
    }
}

int lvgl_roki_main(void)
{
    mlog_init();

    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];
    // static lv_color_t buf2[DISP_BUF_SIZE];
    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = fbdev_flush; // fbdev_flush drm_flush
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 854;
    // disp_drv.direct_mode = 1;
    disp_drv.sw_rotate = 1;
    disp_drv.rotated = LV_DISP_ROT_90;
    lv_disp_drv_register(&disp_drv);

    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    indev_drv_1.feedback_cb = feedback_cb;
    lv_indev_drv_register(&indev_drv_1);
    // lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    /*Set a cursor for the mouse*/
    // LV_IMG_DECLARE(mouse_cursor_icon)
    // lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
    // lv_img_set_src(cursor_obj, &mouse_cursor_icon);      /*Set the image source*/
    // lv_indev_set_cursor(mouse_indev, cursor_obj);        /*Connect the image  object to the driver*/

    /*Handle LitlevGL tasks (tickless mode)*/
    pthread_mutex_init(&g_mutex, NULL);
    lv_dev_init();
    lv_roki_widgets();
    while (1)
    {
        pthread_mutex_lock(&g_mutex);
        lv_timer_handler();
        pthread_mutex_unlock(&g_mutex);
        usleep(3000);
    }
    pthread_mutex_destroy(&g_mutex);
    return 0;
}
/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    static uint64_t now_ms = 0;
    // struct timeval tv_now;
    struct timespec tv_now;
    if (start_ms == 0)
    {
        // gettimeofday(&tv_now, NULL);
        clock_gettime(CLOCK_MONOTONIC, &tv_now);
        start_ms = tv_now.tv_sec * 1000 + tv_now.tv_nsec * 0.000001;
        return 0;
    }
    // gettimeofday(&tv_now, NULL);
    clock_gettime(CLOCK_MONOTONIC, &tv_now);
    now_ms = tv_now.tv_sec * 1000 + tv_now.tv_nsec * 0.000001;

    return now_ms - start_ms;
}
