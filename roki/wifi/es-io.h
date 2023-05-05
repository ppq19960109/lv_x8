#ifndef __ES_IO_H__
#define __ES_IO_H__

typedef struct
{
   unsigned int status;
   unsigned int intFlag;
   unsigned int intMask;
   unsigned int intClr;
   unsigned int ipv4Addr;
   unsigned int ipv4Mask;
   unsigned int macAddr0;
   unsigned int macAddr1;
   unsigned int gw0;
   unsigned int gw1;
   unsigned int gw2;
   unsigned int gw3;
   unsigned int dns0;
   unsigned int dns1;
   unsigned int dns2;
   unsigned int dns3;
   unsigned int fwVersion;
   unsigned int powerOff;
}vnet_reg_t;

#define SPI_IOC_MAGIC           'k'
#define SPI_IOC_HD_STATUS       _IOR(SPI_IOC_MAGIC, 0, __u8)
#define SPI_IOC_HD_INTFLAG      _IOW(SPI_IOC_MAGIC, 1, __u8)
#define SPI_IOC_HD_INTMASK      _IOW(SPI_IOC_MAGIC, 2, __u8)
#define SPI_IOC_HD_INTCLR       _IOW(SPI_IOC_MAGIC, 3, __u8)
#define SPI_IOC_HD_IPADDR       _IOR(SPI_IOC_MAGIC, 4, __u8)
#define SPI_IOC_HD_IPMASK       _IOR(SPI_IOC_MAGIC, 5, __u8)
#define SPI_IOC_HD_MACADDR0     _IOR(SPI_IOC_MAGIC, 6, __u8)
#define SPI_IOC_HD_MACADDR1     _IOR(SPI_IOC_MAGIC, 7, __u8)
#define SPI_IOC_HD_GW0          _IOR(SPI_IOC_MAGIC, 8, __u8)
#define SPI_IOC_HD_GW1          _IOR(SPI_IOC_MAGIC, 9, __u8)
#define SPI_IOC_HD_GW2          _IOR(SPI_IOC_MAGIC, 10, __u8)
#define SPI_IOC_HD_GW3          _IOR(SPI_IOC_MAGIC, 11, __u8)
#define SPI_IOC_HD_DNS0         _IOR(SPI_IOC_MAGIC, 12, __u8)
#define SPI_IOC_HD_DNS1         _IOR(SPI_IOC_MAGIC, 13, __u8)
#define SPI_IOC_HD_DNS2         _IOR(SPI_IOC_MAGIC, 14, __u8)
#define SPI_IOC_HD_DNS3         _IOR(SPI_IOC_MAGIC, 15, __u8)
#define SPI_IOC_HD_FWVERSION    _IOR(SPI_IOC_MAGIC, 16, __u8)
#define SPI_IOC_HD_POWEROFF     _IOW(SPI_IOC_MAGIC, 17, __u8)
#define SPI_IOC_OTA(N)          _IOW(SPI_IOC_MAGIC, 18, char[N])
#define SPI_IOC_REG_INFO        _IOR(SPI_IOC_MAGIC, 19, vnet_reg_t)

#endif
