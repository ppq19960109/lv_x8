#
# Makefile
#
# CC ?= gcc
CROSS_COMPILE=/home/ppq/git/robam_x2100_lvgl/out/product/robam.CQ9068A_nand_5.10-userdebug/host/bin/mips-linux-gnu-
STRIP = $(CROSS_COMPILE)strip
CC = @echo "GCC $@"; $(CROSS_COMPILE)gcc
CXX = @echo "G++ $@"; $(CROSS_COMPILE)g++
LVGL_DIR_NAME ?= lvgl
LVGL_DIR ?= ${shell pwd}
#CFLAGS ?= -O3 -g0 -I$(LVGL_DIR)/ -Wall -Wshadow -Wundef -Wmissing-prototypes -Wno-discarded-qualifiers -Wall -Wextra -Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith -fno-strict-aliasing -Wno-error=cpp -Wuninitialized -Wmaybe-uninitialized -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wformat-security -Wno-ignored-qualifiers -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated -Wempty-body -Wtype-limits -Wshift-negative-value -Wstack-usage=2048 -Wno-unused-value -Wno-unused-parameter -Wno-missing-field-initializers -Wuninitialized -Wmaybe-uninitialized -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wpointer-arith -Wno-cast-qual -Wmissing-prototypes -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wno-discarded-qualifiers -Wformat-security -Wno-ignored-qualifiers -Wno-sign-compare 
CFLAGS +=  -I$(LVGL_DIR)/ \
-I/home/ppq/git/robam_x2100_lvgl/out/product/robam.CQ9068A_nand_5.10-userdebug/host/mipsel-buildroot-linux-gnu/sysroot/usr/include/freetype2 \
-Wall -O3 -g0
ifdef DEBUG
CFLAGS += -g -rdynamic -funwind-tables -ffunction-sections -lmcheck -DDEBUG
endif
LDFLAGS ?= -lm -latomic -Wl,-rpath=./
BIN = lv_X8app

LINKTOOL_PATH:=liblinktool
HV_PATH:=libhv
CFLAGS += -I$(LINKTOOL_PATH)/include
# CFLAGS += -I$(LINKTOOL_PATH)/include/base64
CFLAGS += -I$(LINKTOOL_PATH)/include/cJSON
# CFLAGS += -I$(LINKTOOL_PATH)/include/klib
# CFLAGS += -I$(LINKTOOL_PATH)/include/tcp
# CFLAGS += -I$(LINKTOOL_PATH)/include/signal
CFLAGS += -I$(LINKTOOL_PATH)/include/timer
# CFLAGS += -I$(LINKTOOL_PATH)/include/md5
CFLAGS += -I$(HV_PATH)/include/hv
LDFLAGS += -L$(LINKTOOL_PATH)/lib
LDFLAGS += -L$(HV_PATH)/lib
LDFLAGS += -L. -lvglfont -llinktool -ldl -lm -lpthread -lrt

LDFLAGS += -Wl,--start-group	\
		-Wl,-Bstatic -lhv \
		-Wl,-Bdynamic -lfreetype \
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

## MAINOBJ -> OBJFILES

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
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ) obj/*
distclean: 
	make clean
	rm -f libvglfont.so
lib:
	$(CC) -fPIC -shared -O3 -g0 $(LIBSRCS) -o libvglfont.so
	$(STRIP) libvglfont.so