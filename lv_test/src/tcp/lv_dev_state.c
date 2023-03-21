#include "lv_test_main.h"
#include "main.h"
#include "lv_dev_state.h"
#include "uds_protocol.h"
#include "lv_backlight.h"
#include "hlog.h"

void mlog_hex(const void *buf, int len, const char *file, const int line, const char *func)
{
    unsigned char *data = (unsigned char *)buf;
    printf("[%s:%d:%s]", __FILENAME__, __LINE__, __FUNCTION__);
    for (int i = 0; i < len; ++i)
    {
        if (i % 16 == 0)
            printf("\n");
        printf("%02x ", data[i]);
    }
    printf("\n");
}
#define mlogHex(buf, buf_len) mlog_hex(buf, buf_len, __FILENAME__, __LINE__, __FUNCTION__)

recipe_t g_recipes[60];
save_settings_t g_save_settings;
char wifi_connecting = 0;
static pthread_mutex_t mutex;
static dev_state_t *g_dev_state = NULL;
const char *workStateChineseEnum[] = {"停止", "预约中", "预热中", "运行中", "烹饪完成", "暂停中", "预约暂停中", "预热暂停中"};
static char *workModeEnum[] = {"未设定", "经典蒸", "鲜嫩蒸", "高温蒸", "热风烧烤", "上下加热", "立体热风", "蒸汽嫩烤", "空气速炸", "解冻", "发酵", "保温"};
const char *workModeName(const char mode)
{
    char *name;
    switch (mode)
    {
    case 1:
        name = workModeEnum[1];
        break;
    case 3:
        name = workModeEnum[2];
        break;
    case 4:
        name = workModeEnum[3];
        break;
    case 35:
        name = workModeEnum[4];
        break;
    case 36:
        name = workModeEnum[5];
        break;
    case 38:
        name = workModeEnum[6];
        break;
    case 40:
        name = workModeEnum[7];
        break;
    case 42:
        name = workModeEnum[8];
        break;
    case 65:
        name = workModeEnum[9];
        break;
    case 66:
        name = workModeEnum[10];
        break;
    case 68:
        name = workModeEnum[11];
        break;
    default:
        name = workModeEnum[0];
        break;
    }
    return name;
}

void (*property_change_cb)(const char *key, void *value);
void register_property_change_cb(void (*cb)(const char *key, void *value))
{
    property_change_cb = cb;
}

dev_state_t *get_dev_state(void)
{
    return g_dev_state;
}

dev_attr_t *get_attr_ptr(const char *name)
{
    dev_state_t *dev_state = g_dev_state;
    dev_attr_t *attr = dev_state->attr;

    for (int i = 0; i < dev_state->attr_len; ++i)
    {
        if (strcmp(name, attr[i].key) == 0)
        {
            return &attr[i];
        }
    }
    return NULL;
}

int get_value_int(dev_attr_t *attr)
{
    // int val = 0;
    // for (int i = attr->value_len - 1; i >= 0; --i)
    // {
    //     val = (val << 8) + attr->value[i];
    // }
    // return val;
    return attr->value.n;
}
int get_attr_value_int(const char *name)
{
    dev_attr_t *attr = get_attr_ptr(name);
    if (attr == NULL)
    {
        LOGE("%s,attr name:%s not exits", __func__, name);
        return -1;
    }
    return get_value_int(attr);
}
char *get_value_string(dev_attr_t *attr)
{
    // return attr->value;
    return attr->value.p;
}
char *get_attr_value_string(const char *name)
{
    dev_attr_t *attr = get_attr_ptr(name);
    if (attr == NULL)
    {
        LOGE("%s,attr name:%s not exits", __func__, name);
        return NULL;
    }
    // return attr->value;
    return get_value_string(attr);
}
static void lv_dev_set_value(cJSON *Data, dev_attr_t *ptr)
{
    cJSON *value;
    char change;
    change = 0;
    value = cJSON_GetObjectItem(Data, ptr->key);
    if (value == NULL || ptr == NULL)
    {
        LOGE("%s,null key:%s", __func__, ptr->key);
        return;
    }
    if (LINK_VALUE_TYPE_NUM == ptr->value_type)
    {
        // if (memcmp(ptr->value, &value->valueint, ptr->value_len))
        // {
        //     memcpy(ptr->value, &value->valueint, ptr->value_len);
        //     change = 1;
        // }
        if (ptr->value.n != value->valueint || strcmp("HoodOffRemind", ptr->key) == 0 || strcmp("OTAState", ptr->key) == 0 || strcmp("OTAPowerState", ptr->key) == 0)
        {
            ptr->value.n = value->valueint;
            change = 1;
        }
    }
    else if (LINK_VALUE_TYPE_STRING == ptr->value_type)
    {
        // if (memcmp(ptr->value, value->valuestring, ptr->value_len))
        // {
        //     memcpy(ptr->value, value->valuestring, ptr->value_len);
        //     change = 1;
        // }
        if (ptr->value.p == NULL)
        {
            ptr->value.p = malloc(strlen(value->valuestring) + 1);
            strcpy(ptr->value.p, value->valuestring);
            change = 1;
        }
        else
        {
            if (strcmp(ptr->value.p, value->valuestring))
            {
                free(ptr->value.p);
                ptr->value.p = malloc(strlen(value->valuestring) + 1);
                strcpy(ptr->value.p, value->valuestring);
                change = 1;
            }
        }
    }
    else if (LINK_VALUE_TYPE_STRUCT == ptr->value_type)
    {
        if (strcmp("WifiCurConnected", ptr->key) == 0)
        {
            dev_attr_t *attr_ssid = get_attr_ptr("ssid");
            lv_dev_set_value(value, attr_ssid);
            dev_attr_t *attr_bssid = get_attr_ptr("bssid");
            lv_dev_set_value(value, attr_bssid);
        }
    }
    else if (LINK_VALUE_TYPE_ARRAY == ptr->value_type)
    {
    }
    else
    {
    }
    if (property_change_cb != NULL && change > 0)
        property_change_cb(ptr->key, ptr);
}
static int lv_dev_recv_event(cJSON *Data)
{
    dev_state_t *dev_state = g_dev_state;
    dev_attr_t *attr = dev_state->attr;
    dev_attr_t *ptr;
    pthread_mutex_lock(&g_mutex);
    for (int i = 0; i < dev_state->attr_len; ++i)
    {
        ptr = &attr[i];
        if (cJSON_HasObjectItem(Data, ptr->key))
        {
            lv_dev_set_value(Data, ptr);
        }
    }
    pthread_mutex_unlock(&g_mutex);
    return 0;
}

void set_num_toServer(const char *key, int value)
{
    LOGI("%s,key:%s value:%d", __func__, key, value);
    cJSON *root = cJSON_CreateObject();
    if (value < 0)
        cJSON_AddNullToObject(root, key);
    else
        cJSON_AddNumberToObject(root, key, value);
    send_set_uds(root);
}
void get_toServer(const char *key)
{
    LOGI("%s,key:%s", __func__, key);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNullToObject(root, key);
    send_get_uds(root);
}
void connectWiFi(const char *ssid, const char *psk, int encryp)
{
    wifi_connecting = 1;
    wifi_connecting_change(wifi_connecting);

    strcpy(g_wifi_info.ssid, ssid);
    strcpy(g_wifi_info.psk, psk);
    g_wifi_info.encryp = encryp;

    cJSON *Data = cJSON_CreateObject();
    cJSON *wifiConnectInfo = cJSON_CreateObject();

    cJSON_AddStringToObject(wifiConnectInfo, "ssid", ssid);
    cJSON_AddStringToObject(wifiConnectInfo, "psk", psk);
    cJSON_AddNumberToObject(wifiConnectInfo, "encryp", encryp);

    cJSON_AddItemToObject(Data, "WifiConnect", wifiConnectInfo);
    send_set_uds(Data);
}
void set_stoveTiming_toServer(const int index, const int time)
{
    cJSON *root = cJSON_CreateObject();
    if (index == 0)
    {
        cJSON_AddNumberToObject(root, "LStoveTimingOpera", TIMING_OPERATION_START);
        cJSON_AddNumberToObject(root, "LStoveTimingSet", time);
    }
    else
    {
        cJSON_AddNumberToObject(root, "RStoveTimingOpera", TIMING_OPERATION_START);
        cJSON_AddNumberToObject(root, "RStoveTimingSet", time);
    }
    send_set_uds(root);
}
void set_cook_toServer(steamoven_t *steamoven)
{
    cJSON *root = cJSON_CreateObject();
    if (steamoven->attr_len == 0)
    {
        if (steamoven->cookPos == COOK_POS_LEFT)
        {
            cJSON_AddNumberToObject(root, "LMultiMode", MULTI_MODE_NONE);
            cJSON_AddNumberToObject(root, "LStOvMode", steamoven->attr[0].mode);
            cJSON_AddNumberToObject(root, "LStOvSetTimer", steamoven->attr[0].time);
            cJSON_AddNumberToObject(root, "LStOvSetTemp", steamoven->attr[0].temp);
            if (steamoven->orderTime > 0)
                cJSON_AddNumberToObject(root, "LStOvOrderTimer", steamoven->orderTime);
            cJSON_AddNumberToObject(root, "LStOvOperation", WORK_OPERATION_START);
        }
        else
        {
            cJSON_AddNumberToObject(root, "RMultiMode", MULTI_MODE_NONE);
            cJSON_AddNumberToObject(root, "RStOvMode", steamoven->attr[0].mode);
            cJSON_AddNumberToObject(root, "RStOvSetTimer", steamoven->attr[0].time);
            cJSON_AddNumberToObject(root, "RStOvSetTemp", steamoven->attr[0].temp);
            if (steamoven->orderTime > 0)
                cJSON_AddNumberToObject(root, "RStOvOrderTimer", steamoven->orderTime);
            cJSON_AddNumberToObject(root, "RStOvOperation", WORK_OPERATION_START);
        }
    }
    else
    {
        cJSON *array = cJSON_CreateArray();
        for (int i = 0; i < steamoven->attr_len; ++i)
        {
            cJSON *arraySub = cJSON_CreateObject();
            cJSON_AddNumberToObject(arraySub, "Mode", steamoven->attr[i].mode);
            cJSON_AddNumberToObject(arraySub, "Timer", steamoven->attr[i].time);
            cJSON_AddNumberToObject(arraySub, "Temp", steamoven->attr[i].temp);
            cJSON_AddItemToArray(array, arraySub);
        }
        if (steamoven->cookPos == COOK_POS_LEFT)
        {
            if (steamoven->cookId == 0)
            {
                cJSON_AddNumberToObject(root, "LMultiMode", MULTI_MODE_MULTISTAGE);
                cJSON_AddItemToObject(root, "LMultiStageContent", array);
            }
            else
            {
                cJSON_AddNumberToObject(root, "LMultiMode", MULTI_MODE_RECIPE);
                cJSON_AddItemToObject(root, "LCookbookParam", array);
                cJSON_AddNumberToObject(root, "LCookbookID", steamoven->cookId);
                cJSON_AddStringToObject(root, "LCookbookName", steamoven->cookName);
            }
            if (steamoven->orderTime > 0)
                cJSON_AddNumberToObject(root, "LStOvOrderTimer", steamoven->orderTime);
            cJSON_AddNumberToObject(root, "LStOvOperation", WORK_OPERATION_START);
        }
        else
        {
            if (steamoven->cookId == 0)
            {
                cJSON_AddNumberToObject(root, "RMultiMode", MULTI_MODE_MULTISTAGE);
                cJSON_AddItemToObject(root, "RMultiStageContent", array);
            }
            else
            {
                cJSON_AddNumberToObject(root, "RMultiMode", MULTI_MODE_RECIPE);
                cJSON_AddItemToObject(root, "RCookbookParam", array);
                cJSON_AddNumberToObject(root, "RCookbookID", steamoven->cookId);
                cJSON_AddStringToObject(root, "RCookbookName", steamoven->cookName);
            }
            if (steamoven->orderTime > 0)
                cJSON_AddNumberToObject(root, "RStOvOrderTimer", steamoven->orderTime);
            cJSON_AddNumberToObject(root, "RStOvOperation", WORK_OPERATION_START);
        }
    }
    send_set_uds(root);
    if (lv_page_exist_page("page_steaming"))
    {
        lv_page_back_page("page_steaming");
    }
}

static void *profile_parse_json(void *input, const char *str) // 启动时解析转换配置文件
{
    cJSON *root = cJSON_Parse(str);
    if (root == NULL)
        return NULL;

    cJSON *DeviceCategory = cJSON_GetObjectItem(root, "DeviceCategory");
    if (DeviceCategory == NULL)
    {
        LOGE("DeviceCategory is NULL\n");
        goto fail;
    }
    cJSON *attr = cJSON_GetObjectItem(root, "attr");
    if (attr == NULL)
    {
        LOGE("attr is NULL\n");
        goto fail;
    }

    int arraySize = cJSON_GetArraySize(attr);
    if (arraySize == 0)
    {
        LOGE("attr arraySize is 0\n");
        goto fail;
    }
    int i;
    dev_state_t *dev_state = (dev_state_t *)malloc(sizeof(dev_state_t));
    if (dev_state == NULL)
    {
        LOGE("malloc error\n");
        goto fail;
    }
    memset(dev_state, 0, sizeof(dev_state_t));
    dev_state->attr_len = arraySize;
    dev_state->attr = (dev_attr_t *)malloc(sizeof(dev_attr_t) * dev_state->attr_len);
    if (dev_state->attr == NULL)
    {
        LOGE("malloc error\n");
        goto fail;
    }
    memset(dev_state->attr, 0, sizeof(dev_attr_t) * dev_state->attr_len);

    cJSON *arraySub, *cloudKey, *valueType, *uartByteLen;
    for (i = 0; i < arraySize; i++)
    {
        arraySub = cJSON_GetArrayItem(attr, i);
        if (arraySub == NULL)
            continue;

        if (cJSON_HasObjectItem(arraySub, "cloudKey"))
        {
            cloudKey = cJSON_GetObjectItem(arraySub, "cloudKey");
            strcpy(dev_state->attr[i].key, cloudKey->valuestring);
        }
        valueType = cJSON_GetObjectItem(arraySub, "valueType");
        dev_state->attr[i].value_type = valueType->valueint;
        LV_LOG_USER("cloudKey:%s\n", dev_state->attr[i].key);
        // if (cJSON_HasObjectItem(arraySub, "uartByteLen"))
        // {
        //     uartByteLen = cJSON_GetObjectItem(arraySub, "uartByteLen");
        //     dev_state->attr[i].value_len = uartByteLen->valueint;
        // }
        // if (dev_state->attr[i].value_len > 0)
        // {
        //     dev_state->attr[i].value = (char *)malloc(dev_state->attr[i].value_len);
        //     if (dev_state->attr[i].value == NULL)
        //     {
        //         LOGE("malloc error\n");
        //         goto fail;
        //     }
        //     memset(dev_state->attr[i].value, 0, dev_state->attr[i].value_len);
        // }
    }
    cJSON_Delete(root);
    return dev_state;
fail:
    cJSON_Delete(root);
    return NULL;
}
static void recipes_parse_array(recipe_t *recipes, cJSON *array)
{
    cJSON *arraySub;
    int arraySize = cJSON_GetArraySize(array);
    LOGI("%s,arraySize:%d\n", __func__, arraySize);
    for (int i = 0; i < arraySize; ++i)
    {
        arraySub = cJSON_GetArrayItem(array, i);
        if (arraySub == NULL)
            continue;
        cJSON *imgUrl = cJSON_GetObjectItem(arraySub, "imgUrl");
        cJSON *ingredients = cJSON_GetObjectItem(arraySub, "ingredients");
        cJSON *details = cJSON_GetObjectItem(arraySub, "details");
        cJSON *recipeid = cJSON_GetObjectItem(arraySub, "recipeid");
        cJSON *dishName = cJSON_GetObjectItem(arraySub, "dishName");
        cJSON *cookSteps = cJSON_GetObjectItem(arraySub, "cookSteps");
        cJSON *recipeType = cJSON_GetObjectItem(arraySub, "recipeType");
        cJSON *cookPos = cJSON_GetObjectItem(arraySub, "cookPos");
        cJSON *temp = cJSON_GetObjectItem(arraySub, "temp");

        strncpy(recipes[i].imgUrl, imgUrl->valuestring, sizeof(recipes[i].imgUrl) - 1);
        strncpy(recipes[i].ingredients, ingredients->valuestring, sizeof(recipes[i].ingredients) - 1);
        if (cookSteps != NULL)
            strncpy(recipes[i].cookSteps, cookSteps->valuestring, sizeof(recipes[i].cookSteps) - 1);
        strncpy(recipes[i].dishName, dishName->valuestring, sizeof(recipes[i].dishName) - 1);
        if (recipeid != NULL)
            recipes[i].recipeid = recipeid->valueint;
        recipes[i].recipeType = recipeType->valueint;
        recipes[i].cookPos = cookPos->valueint;
        if (temp != NULL)
            recipes[i].temp = temp->valueint;

        cJSON *detailSub;
        recipes[i].details_count = cJSON_GetArraySize(details);
        recipes[i].details = malloc(recipes[i].details_count * sizeof(char *));
        for (int j = 0; j < recipes[i].details_count; ++j)
        {
            detailSub = cJSON_GetArrayItem(details, j);
            if (detailSub == NULL)
                continue;
            // LOGI("recipes step:%d,value%s\n", j, detailSub->valuestring);
            recipes[i].details[j] = malloc(strlen(detailSub->valuestring) + 1);
            strcpy(recipes[i].details[j], detailSub->valuestring);
        }
    }
}
static void *recipes_parse_json(void *input, const char *str) // 启动时解析
{
    cJSON *root = cJSON_Parse(str);
    if (root == NULL)
    {
        return NULL;
    }

    cJSON *recipes = cJSON_GetObjectItem(root, "recipes");
    if (recipes == NULL)
    {
        LOGE("recipes is NULL\n");
        goto fail;
    }
    recipes_parse_array(g_recipes, recipes);
    cJSON *tempRecipes = cJSON_GetObjectItem(root, "tempRecipes");
    if (tempRecipes == NULL)
    {
        LOGE("tempRecipes is NULL\n");
        goto fail;
    }
    recipes_parse_array(&g_recipes[40], tempRecipes);
fail:
    cJSON_Delete(root);
    return NULL;
}

static void save_settings_init(void)
{
    int len = 1;
    H_Kv_Get("firstStartup", &g_save_settings.firstStartup, &len);
    H_Kv_Get("sleepSwitch", &g_save_settings.sleepSwitch, &len);
    H_Kv_Get("sleepTime", &g_save_settings.sleepTime, &len);
    H_Kv_Get("screenSaverIndex", &g_save_settings.screenSaverIndex, &len);
    H_Kv_Get("brightness", &g_save_settings.brightness, &len);
    H_Kv_Get("wifiEnable", &g_save_settings.wifiEnable, &len);
    if (g_save_settings.brightness == 0)
    {
        g_save_settings.brightness = 200;
        H_Kv_Set("brightness", &g_save_settings.brightness, 1, 0);
    }
    backlightSet(g_save_settings.brightness);
}

int lv_dev_init(void) // 初始化
{
    pthread_mutex_init(&mutex, NULL);

    backlightEnable();
    save_settings_init();

    g_dev_state = get_dev_profile(".", NULL, PROFILE_NAME, profile_parse_json);
    if (g_dev_state == NULL)
    {
        LOGE("cloud_init error\n");
        return -1;
    }
    get_dev_profile(".", NULL, "RecipesDetails.json", recipes_parse_json);

    register_uds_json_recv_cb(lv_dev_recv_event);
    uds_protocol_init();

    return 0;
}

void lv_dev_deinit(void) // 反初始化
{
    for (int i = 0; i < g_dev_state->attr_len; ++i)
    {
        // if (g_dev_state->attr[i].value_len > 0)
        //     free(g_dev_state->attr[i].value);
        if (g_dev_state->attr[i].value.p != NULL)
            free(g_dev_state->attr[i].value.p);
    }
    free(g_dev_state->attr);
    free(g_dev_state);

    uds_protocol_deinit();
    pthread_mutex_destroy(&mutex);
}
