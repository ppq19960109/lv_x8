#include "main.h"
#include "lv_dev_state.h"
#include "uds_protocol.h"
#include "lv_backlight.h"

recipe_t g_recipes[40];
static pthread_mutex_t mutex;
static dev_state_t *g_dev_state = NULL;

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

int get_attr_value_int(const char *name)
{
    dev_attr_t *attr = get_attr_ptr(name);
    if (attr == NULL)
    {
        dzlog_error("%s,attr name:%s not exits", __func__, name);
        return -1;
    }
    int val = 0;
    for (int i = 0; i < attr->value_len; ++i)
    {
        val = (val << 8) + attr->value[i];
    }
    return val;
}

const char *get_attr_value_string(const char *name)
{
    dev_attr_t *attr = get_attr_ptr(name);
    if (attr == NULL)
    {
        dzlog_error("%s,attr name:%s not exits", __func__, name);
        return NULL;
    }
    return attr->value;
}

static int lv_dev_recv_event(cJSON *Data)
{
    dev_state_t *dev_state = g_dev_state;
    dev_attr_t *attr = dev_state->attr;
    dev_attr_t *ptr;
    cJSON *value;
    for (int i = 0; i < dev_state->attr_len; ++i)
    {
        ptr = &attr[i];
        if (cJSON_HasObjectItem(Data, ptr->key))
        {
            value = cJSON_GetObjectItem(Data, ptr->key);
            if (LINK_VALUE_TYPE_NUM == ptr->value_type)
            {
                memcpy(ptr->value, &value->valueint, ptr->value_len);
            }
            else if (LINK_VALUE_TYPE_STRING == ptr->value_type)
            {
                memcpy(ptr->value, &value->valuestring, ptr->value_len);
            }
            else if (LINK_VALUE_TYPE_STRUCT == ptr->value_type)
            {
            }
            else if (LINK_VALUE_TYPE_ARRAY == ptr->value_type)
            {
            }
            else
            {
            }
            if (property_change_cb != NULL)
                property_change_cb(ptr->key, ptr->value);
        }
    }
    return 0;
}

void set_num_toServer(const char *key, int value)
{
    dzlog_info("%s,key:%s value:%d", __func__, key, value);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, key, value);
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
}

static void *profile_parse_json(void *input, const char *str) //启动时解析转换配置文件
{
    cJSON *root = cJSON_Parse(str);
    if (root == NULL)
        return NULL;

    cJSON *DeviceCategory = cJSON_GetObjectItem(root, "DeviceCategory");
    if (DeviceCategory == NULL)
    {
        dzlog_error("DeviceCategory is NULL\n");
        goto fail;
    }
    cJSON *attr = cJSON_GetObjectItem(root, "attr");
    if (attr == NULL)
    {
        dzlog_error("attr is NULL\n");
        goto fail;
    }

    int arraySize = cJSON_GetArraySize(attr);
    if (arraySize == 0)
    {
        dzlog_error("attr arraySize is 0\n");
        goto fail;
    }
    int i;
    dev_state_t *dev_state = (dev_state_t *)malloc(sizeof(dev_state_t));
    if (dev_state == NULL)
    {
        dzlog_error("malloc error\n");
        goto fail;
    }
    memset(dev_state, 0, sizeof(dev_state_t));
    dev_state->attr_len = arraySize;
    dev_state->attr = (dev_attr_t *)malloc(sizeof(dev_attr_t) * dev_state->attr_len);
    if (dev_state->attr == NULL)
    {
        dzlog_error("malloc error\n");
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
        if (cJSON_HasObjectItem(arraySub, "uartByteLen"))
        {
            uartByteLen = cJSON_GetObjectItem(arraySub, "uartByteLen");
            dev_state->attr[i].value_len = uartByteLen->valueint;
        }
        if (dev_state->attr[i].value_len > 0)
        {
            dev_state->attr[i].value = (char *)malloc(dev_state->attr[i].value_len);
            if (dev_state->attr[i].value == NULL)
            {
                dzlog_error("malloc error\n");
                goto fail;
            }
            memset(dev_state->attr[i].value, 0, dev_state->attr[i].value_len);
        }
    }
    cJSON_Delete(root);
    return dev_state;
fail:
    cJSON_Delete(root);
    return NULL;
}

static void *recipes_parse_json(void *input, const char *str) //启动时解析
{
    cJSON *root = cJSON_Parse(str);
    if (root == NULL)
    {
        return NULL;
    }

    cJSON *recipes = cJSON_GetObjectItem(root, "recipes");
    if (recipes == NULL)
    {
        dzlog_info("recipes is NULL\n");
        goto fail;
    }
    cJSON *arraySub;
    int arraySize = cJSON_GetArraySize(recipes);
    for (int i = 0; i < arraySize; ++i)
    {
        arraySub = cJSON_GetArrayItem(recipes, i);
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

        strncpy(g_recipes[i].imgUrl, imgUrl->valuestring, sizeof(g_recipes[i].imgUrl) - 1);
        strncpy(g_recipes[i].ingredients, ingredients->valuestring, sizeof(g_recipes[i].ingredients) - 1);
        strncpy(g_recipes[i].cookSteps, cookSteps->valuestring, sizeof(g_recipes[i].cookSteps) - 1);
        strncpy(g_recipes[i].dishName, dishName->valuestring, sizeof(g_recipes[i].dishName) - 1);
        g_recipes[i].recipeid = recipeid->valueint;
        g_recipes[i].recipeType = recipeType->valueint;
        g_recipes[i].cookPos = cookPos->valueint;
    }
fail:
    cJSON_Delete(root);
    return NULL;
}

int lv_dev_init(void) //初始化
{
    pthread_mutex_init(&mutex, NULL);

    int rc = dzlog_init("zlog_lvgl.conf", "default"); // zlog初始化
    if (rc)
    {
        printf("dzlog_init failed\n");
        return -1;
    }

    backlightEnable();
    backlightSet(200);

    uds_protocol_init();
    uds_client_init();
    g_dev_state = get_dev_profile(".", NULL, PROFILE_NAME, profile_parse_json);
    if (g_dev_state == NULL)
    {
        dzlog_error("cloud_init error\n");
        return -1;
    }
    get_dev_profile(".", NULL, "RecipesDetails.json", recipes_parse_json);

    register_uds_json_recv_cb(lv_dev_recv_event);

    return 0;
}

void lv_dev_deinit(void) //反初始化
{
    for (int i = 0; i < g_dev_state->attr_len; ++i)
    {
        if (g_dev_state->attr[i].value_len > 0)
            free(g_dev_state->attr[i].value);
    }
    free(g_dev_state->attr);
    free(g_dev_state);

    uds_protocol_deinit();
    zlog_fini();
    dzlog_warn("%s...\n", __func__);
    pthread_mutex_destroy(&mutex);
}
