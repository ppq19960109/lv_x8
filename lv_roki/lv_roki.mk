LV_ROKI_DIR_NAME := lv_roki

CFLAGS += -I$(LVGL_DIR)/$(LV_ROKI_DIR_NAME)/src

CSRCS += $(wildcard $(LVGL_DIR)/$(LV_ROKI_DIR_NAME)/src/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(LV_ROKI_DIR_NAME)/src/tcp/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(LV_ROKI_DIR_NAME)/src/tcp/*.cpp)
CSRCS += $(wildcard $(LVGL_DIR)/$(LV_ROKI_DIR_NAME)/src/dialog/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(LV_ROKI_DIR_NAME)/src/custom_item/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(LV_ROKI_DIR_NAME)/src/page/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(LV_ROKI_DIR_NAME)/src/production_page/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(LV_ROKI_DIR_NAME)/lv_100ask_page_manager_test/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(LV_ROKI_DIR_NAME)/lv_100ask_pinyin_ime_test/*.c)
