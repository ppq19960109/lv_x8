
CC = @echo "GCC $@"; $(CROSS_COMPILE)gcc
CXX = @echo "G++ $@"; $(CROSS_COMPILE)g++
STRIP = $(CROSS_COMPILE)strip
AR = ar -rcs

LVGL_DIR_NAME ?= lvgl
LVGL_DIR ?= ${shell pwd}

CFLAGS += $(MCFLAGS)
CFLAGS += -I/home/ppq/git/robam_x2100_lvgl/out/product/robam.CQ9068A_nand_5.10-userdebug/host/mipsel-buildroot-linux-gnu/sysroot/usr/include/freetype2
CFLAGS += -I$(LVGL_DIR)
include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk

CSRCS +=$(LVGL_DIR)/mouse_cursor_icon.c
CSRCS +=$(wildcard lv_font_SiYuanHeiTi_Normal_*.c) 
OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(filter %.o,$(CSRCS:.c=$(OBJEXT)))
COBJS += $(filter %.o,$(CSRCS:.cpp=$(OBJEXT)))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS) 

all: liblvgl.a
#	@echo "CFLAGS:$(CFLAGS)"

%.o: %.c
#	@echo "$(CC) $@"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

liblvgl.a: $(AOBJS) $(COBJS)
	$(AR) $@ $^
clean: 
	rm -f  $(AOBJS) $(COBJS) liblvgl.a
distclean: 
	make clean
