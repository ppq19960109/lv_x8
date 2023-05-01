#ifndef _ROKI_DEV_STATE_H_
#define _ROKI_DEV_STATE_H_

#define PROFILE_NAME "ROKIDevProfile.json"
#define ETH_NAME "wlan0"
#define SOFTER_VER "0.0.1"
// #define ATTR_UNION

typedef enum
{
    RK_WIFI_State_IDLE = 0,
    RK_WIFI_State_CONNECTING,
    RK_WIFI_State_CONNECTFAILED,
    RK_WIFI_State_CONNECTFAILED_WRONG_KEY,
    RK_WIFI_State_CONNECTED,
    RK_WIFI_State_DISCONNECTED,
    RK_WIFI_State_OPEN,
    RK_WIFI_State_OFF,
    RK_WIFI_State_LINK_CONNECTED
} RK_WIFI_RUNNING_State_e;

enum LINK_VALUE_TYPE
{
    LINK_VALUE_TYPE_NUM = 0x00,
    LINK_VALUE_TYPE_STRING,
    LINK_VALUE_TYPE_STRING_NUM,
    LINK_VALUE_TYPE_STRUCT,
    LINK_VALUE_TYPE_ARRAY,
};
enum LINK_FUN_TYPE
{
    LINK_FUN_TYPE_ATTR_REPORT_CTRL = 0x00,
    LINK_FUN_TYPE_ATTR_REPORT,
    LINK_FUN_TYPE_ATTR_CTRL,
    LINK_FUN_TYPE_EVENT,
    LINK_FUN_TYPE_SERVICE,
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
    WORK_OPERATION_STOP= 0x00,
    WORK_OPERATION_START ,
    WORK_OPERATION_PAUSE,
    WORK_OPERATION_RESERVE,
    WORK_OPERATION_CONTINUE,
    WORK_OPERATION_SELF_TEST,
};
enum WORK_STATE_ENUM
{
    WORK_STATE_STOP = 0x00,
    WORK_STATE_RESERVE,
    WORK_STATE_PREHEAT,
    WORK_STATE_PAUSE_PREHEAT,
    WORK_STATE_RUN,
    WORK_STATE_PAUSE,
    WORK_STATE_FINISH,
    WORK_STATE_SELF_TEST,
};
enum TIMING_OPERATION_ENUM
{
    TIMING_OPERATION_START = 0x01,
    TIMING_OPERATION_CANCEL,
};
enum TIMING_STATE_ENUM
{
    TIMING_STATE_STOP = 0x00,
    TIMING_STATE_RUN,
    TIMING_STATE_PAUSE,
    TIMING_STATE_CONFIRM,
};
enum OTA_STATE_ENUM
{
    OTA_STATE_IDLE = 0x00,
    OTA_STATE_NO_FIRMWARE,
    OTA_STATE_NEW_FIRMWARE,
    OTA_STATE_DOWNLOAD_START,
    OTA_STATE_DOWNLOAD_FAIL,
    OTA_STATE_DOWNLOAD_SUCCESS,
    OTA_STATE_INSTALL_START,
    OTA_STATE_INSTALL_FAIL,
    OTA_STATE_INSTALL_SUCCESS,
};
enum ROKI_CMD_ID_ENUM
{
    ROKI_CMD_ID_QUERY = 190,
    ROKI_CMD_ID_QUERY_RESPONSE,
    ROKI_CMD_ID_SET,
    ROKI_CMD_ID_SET_RESPONSE,
    ROKI_CMD_ID_EVENT,
    ROKI_CMD_ID_ERROR,
    ROKI_CMD_ID_HISTORY_ERROR,
};
typedef struct
{
    char key[18];
    unsigned char dev_type;
    unsigned char cmd;
    unsigned char value_len;
    enum LINK_VALUE_TYPE value_type;
    enum LINK_FUN_TYPE fun_type;
#ifndef ATTR_UNION
    char *value;
#else
    union
    {
        char *p;
        int n;
    } value;
#endif
} dev_attr_t;

typedef struct
{
    char device_category[33];
    char device_model[16];
    char mac[16];
    char software_ver[6];
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
    char **temp_model;
    unsigned short time;
    unsigned short maxtime;
    char name[24];
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
    int temp;
} recipe_t;

extern recipe_t g_recipes[];
extern const char *workStateChineseEnum[];
extern char wifi_connecting;

void register_property_change_cb(void (*cb)(const char *key, void *value));

int roki_dev_init(void);
void roki_dev_deinit(void);
dev_state_t *get_dev_state(void);
dev_attr_t *get_attr_ptr(const char *name);
int get_attr_value_int(const char *name);
int get_value_int(dev_attr_t *attr);
char *get_attr_value_string(const char *name);
char *get_value_string(dev_attr_t *attr);

void set_num_toServer(const char *key, int value);
void get_toServer(const char *key);
void set_cook_toServer(steamoven_t *steamoven);

const char *workModeName(const int mode);
#endif
