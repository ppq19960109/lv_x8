

/*********************
 *      INCLUDES
 *********************/
#include "lv_backlight.h"
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
int backlightEnable(void)
{
    printf("backlightEnable\n");

    int fd = -1;
    fd = open(SYSFS_BL_DIR "/bl_power", O_RDWR);
    if (fd < 0)
    {
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
int backlightDisable(void)
{
    printf("backlightDisable\n");

    int fd = -1;
    fd = open(SYSFS_BL_DIR "/bl_power", O_RDWR);
    if (fd < 0)
    {
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
    printf("backlightSet\n");

    int fd = -1;
    char buf[8] = {0};

    sprintf(buf, "%d", value);
    fd = open(SYSFS_BL_DIR "/brightness", O_WRONLY);
    if (fd < 0)
    {
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
int backlightGet(void)
{
    printf("backlightGet\n");
    int value = 200; // 200为背光驱动初始值

    int fd = -1;
    char buf[8] = {0};

    fd = open(SYSFS_BL_DIR "/brightness", O_RDONLY);
    if (fd < 0)
    {
        perror("Open Backlight brightness Fail");
        return -1;
    }
    read(fd, buf, sizeof(buf));
    close(fd);
    value = atoi(buf);

    return value;
}

void setClockTimestamp(long timestamp)
{
    printf("setClockTimestamp:%ld\n", timestamp);
    //pthread_mutex_lock(&g_mutex);
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);

    struct timeval tv;
    tv.tv_sec = timestamp;
    tv.tv_usec = tv_now.tv_usec;
    printf("settimeofday ret:%d\n", settimeofday(&tv, NULL));
    //pthread_mutex_unlock(&g_mutex);
}
void setClockTime(int hours, int minutes)
{
    printf("set hour:%d min:%d\n", hours, minutes);
    time_t t;
    time(&t);
    struct tm *local_tm = localtime(&t);
    printf("year:%d mon:%d day:%d\n", local_tm->tm_year, local_tm->tm_mon, local_tm->tm_mday);
    printf("hour:%d min:%d sec:%d\n", local_tm->tm_hour, local_tm->tm_min, local_tm->tm_sec);
    local_tm->tm_hour = hours;
    local_tm->tm_min = minutes;
    t = mktime(local_tm);
    if (t < 0)
    {
        local_tm->tm_mon = 1;
        t = mktime(local_tm);
    }
    setClockTimestamp(t);
}
