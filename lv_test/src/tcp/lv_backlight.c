

/*********************
 *      INCLUDES
 *********************/
#include "lv_test_main.h"

#define SYSFS_BL_DIR "/sys/devices/platform/backlight/backlight/backlight"
/*********************
 *      DEFINES
 *********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * @brief backlightEnable屏背光-打开
 * 屏背光低有效
 */
int backlightEnable()
{
    LV_LOG_USER("backlightEnable");

    int fd = -1;
    fd     = open(SYSFS_BL_DIR "/bl_power", O_RDWR);
    if(fd < 0) {
        perror("Open Backlight bl_power Fail");
        return -1;
    }
    printf("Lcd Backlight Open\n");
    write(fd, "0", 2);
    close(fd);

    return 0;
}

/**
 * @brief backlightDisable屏背光－关闭
 * 屏背光低有效
 */
int backlightDisable()
{
    LV_LOG_USER("backlightDisable");

    int fd = -1;
    fd     = open(SYSFS_BL_DIR "/bl_power", O_RDWR);
    if(fd < 0) {
        perror("Open Backlight bl_power Fail");
        return -1;
    }
    printf("Lcd Backlight Close\n");
    write(fd, "1", 2);
    close(fd);

    return 0;
}

/**
 * @brief backlightConfig 设置背光亮度
 * @param value　亮度值0~255
 */
int backlightSet(unsigned char value)
{
    LV_LOG_USER("backlightSet");

    int fd      = -1;
    char buf[8] = {0};

    sprintf(buf, "%d", value);
    fd = open(SYSFS_BL_DIR "/brightness", O_WRONLY);
    if(fd < 0) {
        perror("Open Backlight brightness Fail");
        return -1;
    }
    write(fd, buf, strlen(buf));
    close(fd);

    return 0;
}

/**
 * @brief backlightGet 获取背光亮度
 * @return 　亮度值0~255
 */
int backlightGet()
{
    LV_LOG_USER("backlightGet");
    int value = 200; // 200为背光驱动初始值

    int fd      = -1;
    char buf[8] = {0};

    fd = open(SYSFS_BL_DIR "/brightness", O_RDONLY);
    if(fd < 0) {
        perror("Open Backlight brightness Fail");
        return -1;
    }
    read(fd, buf, sizeof(buf));
    close(fd);
    value = atoi(buf);

    return value;
}
