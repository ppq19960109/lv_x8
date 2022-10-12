LV_TEST_DIR_NAME := lv_test

CSRCS += $(wildcard $(LVGL_DIR)/$(LV_TEST_DIR_NAME)/src/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(LV_TEST_DIR_NAME)/lv_100ask_page_manager_test/*.c)
CSRCS += $(wildcard $(LVGL_DIR)/$(LV_TEST_DIR_NAME)/lv_100ask_pinyin_ime_test/*.c)
