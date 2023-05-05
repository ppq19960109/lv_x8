#include "main.h"
#include "es-io.h"

static const char *g_spiDevice = "/dev/spidev1.0";
vnet_reg_t g_spiReg;

static void wifi_link_listen_ioctl(unsigned long request, unsigned int *info)
{
    int fd = -1;
    int ret;

    fd = open(g_spiDevice, O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "open error:%s\n", g_spiDevice);
        return;
    }
    ret = ioctl(fd, request, info);
    if (ret != 0)
    {
        fprintf(stderr, "ioctl error\n");
    }
    close(fd);
}
