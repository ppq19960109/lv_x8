export CROSS_COMPILE = /home/ppq/git/robam_x2100_lvgl/out/product/robam.CQ9068A_nand_5.10-userdebug/host/bin/mips-linux-gnu-
CC = @echo "GCC $@"; $(CROSS_COMPILE)gcc
CXX = @echo "G++ $@"; $(CROSS_COMPILE)g++
STRIP = $(CROSS_COMPILE)strip
RM = rm -rf
AR = ar -rcs
CP = cp -r
MKDIR = mkdir -p

TOPDIR = .

HV_PATH:=libhv
SRC_DIRS := $(shell find roki -maxdepth 3 -type d)

LINKTOOL_PATH:=liblinktool
LIBLVGL_PATH:=liblvgl

ifdef DEBUG
export MCFLAGS = -g -rdynamic -funwind-tables -ffunction-sections -lmcheck -DDEBUG
else
export MCFLAGS = -Wall -O3 -g0
endif

CFLAGS += $(MCFLAGS)
CFLAGS += -I$(TOPDIR)
CFLAGS += $(addprefix -I , $(SRC_DIRS))
CFLAGS += $(addprefix -I , $(INC_DIRS))
CFLAGS += -I$(LINKTOOL_PATH)/include
CFLAGS += -I$(LINKTOOL_PATH)/include/timer
CFLAGS += -I$(LINKTOOL_PATH)/include/cJSON
# CFLAGS += -I$(LINKTOOL_PATH)/include/base64
# CFLAGS += -I$(LINKTOOL_PATH)/include/klib
# CFLAGS += -I$(LINKTOOL_PATH)/include/tcp
# CFLAGS += -I$(LINKTOOL_PATH)/include/signal
# CFLAGS += -I$(LINKTOOL_PATH)/include/md5
CFLAGS += -I$(HV_PATH)/include/hv
CFLAGS += -I$(LIBLVGL_PATH) -I$(LIBLVGL_PATH)/lvgl

CXXFLAGS += $(CFLAGS) -std=c++11

LDFLAGS += -L$(LINKTOOL_PATH)/lib
LDFLAGS += -L$(HV_PATH)/lib
LDFLAGS += -L$(TOPDIR)
LDFLAGS += -L$(LIBLVGL_PATH) 

LIBS += -Wl,--start-group	\
		-Wl,-Bstatic  -lhv -llinktool -llvgl \
		-Wl,-Bdynamic -lavcodec -lavformat -lavutil -lswresample -lswscale -lcurl -lfreetype -latomic -ldl -lm -lpthread -lrt \
		-Wl,--end-group
LIBS += -Wl,-rpath=./

SRC := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
INC := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.h))

include lv_roki/lv_roki.mk

OBJ += $(filter %.o,$(SRC:%.c=%.o))
OBJ += $(filter %.o,$(SRC:%.cpp=%.o))

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

TARGET := lv_rokiapp

.PHONY : all clean

all: $(TARGET)
#	@echo "MCFLAGS:$(MCFLAGS)"
#	@echo "CFLAGS:$(CFLAGS)"

$(TARGET) : liblvgl/liblvgl.a $(OBJ)
	$(CXX) $^  $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $@
#	$(STRIP) $@

liblvgl/liblvgl.a : 
	make -C liblvgl M=$(PWD) 

clean :
	$(RM) $(TARGET)
	$(RM) $(OBJ)
	
distclean :
	make clean
	make -C liblvgl clean
