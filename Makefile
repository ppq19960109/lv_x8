#
# Makefile
#
CROSS_COMPILE=/home/ppq/git/robam_x2100_lvgl/out/product/robam.CQ9068A_nand_5.10-userdebug/host/bin/mips-linux-gnu-
STRIP = $(CROSS_COMPILE)strip
CC = @echo "GCC $@"; $(CROSS_COMPILE)gcc
CXX = @echo "G++ $@"; $(CROSS_COMPILE)g++
LVGL_DIR_NAME ?= lvgl
LVGL_DIR ?= ${shell pwd}

BIN = lv_X8app

LINKTOOL_PATH:=liblinktool
HV_PATH:=libhv

CFLAGS +=  -I$(LVGL_DIR)/ \
-I/home/ppq/git/robam_x2100_lvgl/out/product/robam.CQ9068A_nand_5.10-userdebug/host/mipsel-buildroot-linux-gnu/sysroot/usr/include/freetype2 \
-Wall -O3 -g0
ifdef DEBUG
CFLAGS += -g -rdynamic -funwind-tables -ffunction-sections -lmcheck -DDEBUG
endif

CFLAGS += -I$(LINKTOOL_PATH)/include
# CFLAGS += -I$(LINKTOOL_PATH)/include/base64
CFLAGS += -I$(LINKTOOL_PATH)/include/cJSON
# CFLAGS += -I$(LINKTOOL_PATH)/include/tcp
# CFLAGS += -I$(LINKTOOL_PATH)/include/signal
CFLAGS += -I$(LINKTOOL_PATH)/include/timer
# CFLAGS += -I$(LINKTOOL_PATH)/include/md5
CFLAGS += -I$(HV_PATH)/include/hv

LDFLAGS += -lm -latomic -Wl,-rpath=./
LDFLAGS += -L$(LINKTOOL_PATH)/lib
LDFLAGS += -L$(HV_PATH)/lib
LDFLAGS += -L. -ldl -lm -lpthread -lrt

LDFLAGS += -Wl,--start-group	\
		-Wl,-Bstatic -lhv -llinktool \
		-Wl,-Bdynamic -lfreetype -lvglfont \
		-Wl,--end-group

CXXFLAGS += $(CFLAGS) -std=c++11
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
COBJS = $(filter %.o,$(CSRCS:.c=$(OBJEXT)))
COBJS += $(filter %.o,$(CSRCS:.cpp=$(OBJEXT)))

MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS) 


all: default

%.o: %.c
#	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

default: $(AOBJS) $(COBJS) $(MAINOBJ)
	$(CXX) -o $(BIN) $(MAINOBJ) $(AOBJS) $(COBJS) $(LDFLAGS)
#	$(STRIP) $(BIN)
clean: 
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ)
distclean: 
	make clean
	rm -f libvglfont.so
lib:
	$(CC) -fPIC -shared -O3 -g0 $(LIBSRCS) -o libvglfont.so
	$(STRIP) libvglfont.so
