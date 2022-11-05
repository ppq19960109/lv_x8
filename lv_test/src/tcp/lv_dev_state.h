#ifndef _LV_DEV_STATE_H_
#define _LV_DEV_STATE_H_

#include "cJSON.h"

#define PROFILE_NAME "LvDevProfile.json"

enum LINK_VALUE_TYPE
{
    LINK_VALUE_TYPE_NUM = 0x00,
    LINK_VALUE_TYPE_STRING_NUM,
    LINK_VALUE_TYPE_STRING,
    LINK_VALUE_TYPE_STRUCT,
    LINK_VALUE_TYPE_ARRAY,
};

typedef struct
{
    char key[28];
    enum LINK_VALUE_TYPE value_type;
    unsigned char value_len;
    char *value;
} dev_attr_t;

typedef struct
{
    dev_attr_t *attr;
    int attr_len;
} dev_state_t;

typedef struct
{
    char imgUrl[128];
    char ingredients[960];
    char dishName[80];
    char cookSteps[180];
    char **details;
    unsigned char details_count;
    int recipeid;
    int recipeType;
    int cookPos;
} recipe_t;

extern recipe_t g_recipes[40];

int lv_dev_init(void);
void lv_dev_deinit(void);
dev_state_t *get_dev_state(void);
dev_attr_t *get_attr_ptr(const char *name);
int get_attr_value_int(const char *name);
const char *get_attr_value_string(const char *name);

void register_property_change_cb(void (*cb)(const char *key, void *value));

void set_num_toServer(const char *key, int value);
#endif