LV_ROKI_DIR_NAME := lv_roki

CFLAGS += -I$(LV_ROKI_DIR_NAME)/src

SRC += $(wildcard $(LV_ROKI_DIR_NAME)/*.c)
SRC += $(wildcard $(LV_ROKI_DIR_NAME)/src/*.c)
SRC += $(wildcard $(LV_ROKI_DIR_NAME)/src/general/*.c)
SRC += $(wildcard $(LV_ROKI_DIR_NAME)/src/dialog/*.c)
SRC += $(wildcard $(LV_ROKI_DIR_NAME)/src/custom_item/*.c)
SRC += $(wildcard $(LV_ROKI_DIR_NAME)/src/page/*.c)
SRC += $(wildcard $(LV_ROKI_DIR_NAME)/src/production_page/*.c)
SRC += $(wildcard $(LV_ROKI_DIR_NAME)/lv_100ask_page_manager/*.c)
# SRC += $(wildcard $(LV_ROKI_DIR_NAME)/lv_100ask_pinyin_ime/*.c)
# SRC += $(wildcard $(LV_ROKI_DIR_NAME)/lv_100ask_pinyin_ime_test/*.c)
