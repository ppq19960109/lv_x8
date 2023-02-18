#
# Makefile
#
# CC ?= gcc
CROSS_COMPILE=/home/ppq/rk3308sdk/rk3308_linux_v1.00_220210/buildroot/output/rockchip_rk3308_bs_release/host/bin/aarch64-rockchip-linux-gnu-
STRIP = $(CROSS_COMPILE)strip
CC = @echo "GCC $@"; $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LVGL_DIR_NAME ?= lvgl
LVGL_DIR ?= ${shell pwd}
CFLAGS ?= -O3 -g0 -I$(LVGL_DIR)/ -Wall -Wshadow -Wundef -Wmissing-prototypes -Wno-discarded-qualifiers -Wall -Wextra -Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith -fno-strict-aliasing -Wno-error=cpp -Wuninitialized -Wmaybe-uninitialized -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wformat-security -Wno-ignored-qualifiers -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated -Wempty-body -Wtype-limits -Wshift-negative-value -Wstack-usage=2048 -Wno-unused-value -Wno-unused-parameter -Wno-missing-field-initializers -Wuninitialized -Wmaybe-uninitialized -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wpointer-arith -Wno-cast-qual -Wmissing-prototypes -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wno-discarded-qualifiers -Wformat-security -Wno-ignored-qualifiers -Wno-sign-compare -I/home/ppq/rk3308sdk/rk3308_linux_v1.00_220210/buildroot/output/rockchip_rk3308_bs_release/host/aarch64-rockchip-linux-gnu/sysroot/usr/include/libdrm/

LDFLAGS ?= -lm -ldrm
BIN = lv_X8app

LINKTOOL_PATH:=liblinktool
HV_PATH:=libhv
CFLAGS += -I$(LINKTOOL_PATH)/include
CFLAGS += -I$(LINKTOOL_PATH)/include/base64
CFLAGS += -I$(LINKTOOL_PATH)/include/cJSON
CFLAGS += -I$(LINKTOOL_PATH)/include/klib
CFLAGS += -I$(LINKTOOL_PATH)/include/tcp
CFLAGS += -I$(LINKTOOL_PATH)/include/signal
CFLAGS += -I$(LINKTOOL_PATH)/include/timer
CFLAGS += -I$(LINKTOOL_PATH)/include/md5
CFLAGS += -I$(HV_PATH)/include/hv
LDFLAGS += -L$(LINKTOOL_PATH)/lib
LDFLAGS += -L$(HV_PATH)/lib
LDFLAGS += -L. -lvglfont -llinktool -lzlog -ldl -lm -lpthread -lrt

LDFLAGS += -Wl,--start-group	\
		-Wl,-Bstatic -lhv \
		-Wl,-Bdynamic \
		-Wl,--end-group
#Collect the files to compile
MAINSRC = ./main.c

include $(LVGL_DIR)/lv_test/lv_test.mk
include $(LVGL_DIR)/lv_100ask_page_manager/lv_page_manager.mk
include $(LVGL_DIR)/lv_100ask_pinyin_ime/lv_pinyin_ime.mk
include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk

CSRCS +=$(LVGL_DIR)/mouse_cursor_icon.c
LIBSRCS :=$(wildcard lv_font_SiYuanHeiTi_Normal_*.c) 
OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS) 

## MAINOBJ -> OBJFILES

all: default

%.o: %.c
#	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

default: $(AOBJS) $(COBJS) $(MAINOBJ)
	$(CC) -o $(BIN) $(MAINOBJ) $(AOBJS) $(COBJS) $(LDFLAGS)
	$(STRIP) $(BIN)
clean: 
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ) obj/*
distclean: 
	make clean
	rm -f libvglfont.so
lib:
	$(CC) -fPIC -shared -O3 -g0 $(LIBSRCS) -o libvglfont.so
	$(STRIP) libvglfont.so