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
enum COOK_POS_ENUM
{
    COOK_POS_LEFT = 0x00,
    COOK_POS_RIGHT,
    COOK_POS_ALL,
};
enum MULTI_MODE_ENUM
{
    MULTI_MODE_NONE = 0x00,
    MULTI_MODE_RECIPE,
    MULTI_MODE_MULTISTAGE,
};
enum WORK_OPERATION_ENUM
{
    WORK_OPERATION_START = 0x00,
    WORK_OPERATION_PAUSE,
    WORK_OPERATION_CANCEL,
    WORK_OPERATION_CONFIRM,
    WORK_OPERATION_RUN_NOW,
};
enum WORK_STATE_ENUM
{
    WORK_STATE_STOP = 0x00,
    WORK_STATE_RESERVE,
    WORK_STATE_PREHEAT,
    WORK_STATE_RUN,
    WORK_STATE_FINISH,
    WORK_STATE_PAUSE,
    WORK_STATE_PAUSE_RESERVE,
};
typedef struct
{
    char key[28];
    enum LINK_VALUE_TYPE value_type;
    // unsigned char value_len;
    // char *value;
    union
    {
        char *p;
        int n;
    } value;
} dev_attr_t;

typedef struct
{
    dev_attr_t *attr;
    int attr_len;
} dev_state_t;

typedef struct
{
    unsigned char mode;
    unsigned short temp;
    unsigned short time;
} steamoven_attr_t;

typedef struct
{
    unsigned char cookPos;
    unsigned short orderTime;
    int cookId;
    char cookName[64];
    steamoven_attr_t attr[3];
    int attr_len;
} steamoven_t;

typedef struct
{
    unsigned char mode_index;
    unsigned char mode;
    unsigned short temp;
    unsigned short mintemp;
    unsigned short maxtemp;
    unsigned short time;
    unsigned short maxtime;
    char name[32];
} steamoven_mode_t;

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
extern const char *workStateChineseEnum[];

int lv_dev_init(void);
void lv_dev_deinit(void);
dev_state_t *get_dev_state(void);
dev_attr_t *get_attr_ptr(const char *name);
int get_attr_value_int(const char *name);
int get_value_int(dev_attr_t *attr);
const char *get_attr_value_string(const char *name);
const char *get_value_string(dev_attr_t *attr);
const char *workModeName(const char mode);

void register_property_change_cb(void (*cb)(const char *key, void *value));

void set_num_toServer(const char *key, int value);
void get_toServer(const char *key);
void set_cook_toServer(steamoven_t *steamoven);
#endif