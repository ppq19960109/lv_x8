#include "main.h"
#include "roki_dev_state.h"
#include "uds_protocol.h"
#include "hlog.h"

recipe_t g_recipes[60];
char wifi_connecting = 0;
static pthread_mutex_t mutex;
static dev_state_t *g_dev_state = NULL;
const char *workStateChineseEnum[] = {"停止", "预约中", "预热中", "预热暂停中", "运行中", "暂停中", "烹饪完成", "自检中"};
static char *workModeEnum[] = {"无模式", "鲜嫩蒸", "营养蒸", "高温蒸", "微波蒸", "智控蒸", "快热", "风焙烤", "焙烤", "风扇烤", "强烤烧", "烤烧", "快速预热", "果蔬烘干", "专业烤", "微波烤", "煎烤", "底加热", "空气炸", "微波", "eco", "手动加湿烤", "加湿烤-焙烤", "加湿烤-风焙烤", "加湿烤-强烤烧", "加湿烤-烤烧", "干燥", "杀菌", "除垢", "保温", "解冻", "清洁"};
const char *workModeName(const int mode)
{
    return workModeEnum[mode];
}
int pointer_convert_num(unsigned char *data, int len)
{
    int val = 0;
    for (int i = 0; i < len; ++i)
    {
        val = (val << 8) + data[i];
    }
    return val;
}
void num_convert_pointer(unsigned char *src, unsigned char *data, int len)
{
    for (int i = 0; i < len; ++i)
    {
        src[i] = data[len - 1 - i];
    }
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
dev_attr_t *get_attr_cmd(const unsigned char cmd)
{
    dev_state_t *dev_state = g_dev_state;
    dev_attr_t *attr = dev_state->attr;

    for (int i = 0; i < dev_state->attr_len; ++i)
    {
        if (attr[i].cmd == cmd)
        {
            return &attr[i];
        }
    }
    return NULL;
}
int get_value_int(dev_attr_t *attr)
{
#ifndef ATTR_UNION
    return pointer_convert_num((unsigned char *)attr->value, attr->value_len);
#else
    return attr->value.n;
#endif
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
#ifndef ATTR_UNION
    return attr->value;
#else
    return attr->value.p;
#endif
}
char *get_attr_value_string(const char *name)
{
    dev_attr_t *attr = get_attr_ptr(name);
    if (attr == NULL)
    {
        LOGE("%s,attr name:%s not exits", __func__, name);
        return NULL;
    }
    return get_value_string(attr);
}

void roki_attr_set_key_value(const char *key, int value)
{
    LOGI("%s,key:%s value:%d", __func__, key, value);
    dev_attr_t *ptr = get_attr_ptr(key);
    if (ptr == NULL)
    {
        LOGE("%s,attr is NULL:%s", __func__, key);
        return;
    }
    if (LINK_VALUE_TYPE_NUM == ptr->value_type)
    {
#ifndef ATTR_UNION
        num_convert_pointer((unsigned char *)ptr->value, (unsigned char *)&value, ptr->value_len);
#else
        ptr->value.n = value;
#endif
        if (property_change_cb != NULL)
            property_change_cb(ptr->key, ptr);
    }
}
static void roki_dev_set_value(unsigned char *data, unsigned char len, dev_attr_t *ptr)
{
    char change = 0;

    if (LINK_VALUE_TYPE_NUM == ptr->value_type)
    {
#ifndef ATTR_UNION
        if (memcmp(ptr->value, data, ptr->value_len))
        {
            memcpy(ptr->value, data, ptr->value_len);
            change = 1;
        }
#else
        int num = pointer_convert_num(data, len);
        if (ptr->value.n != num)
        {
            ptr->value.n = num;
            change = 1;
        }
#endif
    }
    else if (LINK_VALUE_TYPE_STRING == ptr->value_type)
    {
        if (ptr->value == NULL)
        {
            ptr->value = malloc(len + 1);
            memcpy(ptr->value, data, len);
            ptr->value[len] = 0;
            change = 1;
        }
        else
        {
            int str_len = strlen(ptr->value);
            if (str_len != len || memcmp(ptr->value, data, len))
            {
                if (str_len != len)
                {
                    free(ptr->value);
                    ptr->value = malloc(len + 1);
                }
                memcpy(ptr->value, data, len);
                ptr->value[len] = 0;
                change = 1;
            }
        }
    }
    else if (LINK_VALUE_TYPE_STRUCT == ptr->value_type)
    {
        // if (1 == ptr->cmd)
        // {
        // }
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
static int roki_dev_recv_uds(void *value, int value_len)
{
    int i, j, index = 0;
    dev_state_t *dev_state = g_dev_state;
    dev_attr_t *attr = dev_state->attr;
    dev_attr_t *ptr = NULL;

    pthread_mutex_lock(&g_lvgl_mutex);
    unsigned char *data = (unsigned char *)value;
    unsigned char cmd_key = data[index++];
    unsigned char cmd_id = data[index++];
    unsigned char event = 0, attr_count = 0;
    if (ROKI_CMD_ID_QUERY_RESPONSE == cmd_id)
    {
    }
    else if (ROKI_CMD_ID_EVENT == cmd_id)
    {
        event = data[index++];
    }
    else
    {
        LOGE("%s,unknown cmd_id", __func__);
        goto fail;
    }
    attr_count = data[index++];

    LOGI("%s,cmd_key:%d cmd_id:%d event:%d attr_count:%d", __func__, cmd_key, cmd_id, event, attr_count);
    for (i = index; i < value_len - 2; ++i)
    {
        for (j = 0; j < dev_state->attr_len; ++j)
        {
            ptr = &attr[j];
            if (data[i] == ptr->cmd)
            {
                roki_dev_set_value(&data[i + 2], data[i + 1], ptr);
            }
        }
        i += data[i + 1] + 1;
    }
fail:
    pthread_mutex_unlock(&g_lvgl_mutex);
    return 0;
}
void set_devattr_num(dev_attr_t *attr, int value)
{
    unsigned char index = 0;
    unsigned char buf[6];
    buf[index++] = 1;
    buf[index++] = attr->cmd;
    for (int i = attr->value_len - 1; i >= 0; --i)
    {
        buf[index++] = value >> (8 * i);
    }
    send_set_uds(buf, index);
}
void set_num_toServer2(const unsigned char cmd, int value)
{
    LOGI("%s,cmd:%d value:%d", __func__, cmd, value);
    dev_attr_t *attr = get_attr_cmd(cmd);
    if (attr == NULL)
    {
        LOGE("%s,attr is NULL:%d", __func__, cmd);
        return;
    }
    set_devattr_num(attr, value);
}
void set_num_toServer(const char *key, int value)
{
    LOGI("%s,key:%s value:%d", __func__, key, value);
    dev_attr_t *attr = get_attr_ptr(key);
    if (attr == NULL)
    {
        LOGE("%s,attr is NULL:%s", __func__, key);
        return;
    }
    set_devattr_num(attr, value);
}
void get_devattr(dev_attr_t *attr)
{
    unsigned char buf[2];
    buf[0] = 1;
    buf[1] = attr->cmd;
    send_get_uds(buf, 2);
}
void get_toServer2(const unsigned char cmd)
{
    LOGI("%s,cmd:%d", __func__, cmd);
    dev_attr_t *attr = get_attr_cmd(cmd);
    if (attr == NULL)
    {
        LOGE("%s,attr is NULL:%d", __func__, cmd);
        return;
    }
    get_devattr(attr);
}
void get_toServer(const char *key)
{
    LOGI("%s,key:%s", __func__, key);
    dev_attr_t *attr = get_attr_ptr(key);
    if (attr == NULL)
    {
        LOGE("%s,attr is NULL:%s", __func__, key);
        return;
    }
    get_devattr(attr);
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
    // send_set_uds(root);
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

    cJSON *arraySub, *cloudKey, *valueType, *funType, *uartByteLen;
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

        if (cJSON_HasObjectItem(arraySub, "funType"))
        {
            funType = cJSON_GetObjectItem(arraySub, "funType");
            dev_state->attr[i].fun_type = funType->valueint;
        }
#ifndef ATTR_UNION
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
                LOGE("malloc error\n");
                goto fail;
            }
            memset(dev_state->attr[i].value, 0, dev_state->attr[i].value_len);
        }
#endif
    }
    cJSON_Delete(root);
    return dev_state;
fail:
    cJSON_Delete(root);
    return NULL;
}
static void recipes_parse_array(recipe_t *recipes, cJSON *array)
{
    cJSON *arraySub = NULL;
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

int roki_dev_init(void) // 初始化
{
    pthread_mutex_init(&mutex, NULL);

    g_dev_state = get_dev_profile(".", NULL, PROFILE_NAME, profile_parse_json);
    if (g_dev_state == NULL)
    {
        LOGE("cloud_init error\n");
        return -1;
    }
    get_dev_profile(".", NULL, "RecipesDetails.json", recipes_parse_json);

    register_uds_recv_cb(roki_dev_recv_uds);
    uds_protocol_init();

    return 0;
}

void roki_dev_deinit(void) // 反初始化
{
    for (int i = 0; i < g_dev_state->attr_len; ++i)
    {
#ifndef ATTR_UNION
        if (g_dev_state->attr[i].value_len > 0)
            free(g_dev_state->attr[i].value);
#else
        if (g_dev_state->attr[i].value.p != NULL)
            free(g_dev_state->attr[i].value.p);
#endif
    }
    free(g_dev_state->attr);
    free(g_dev_state);

    uds_protocol_deinit();
    pthread_mutex_destroy(&mutex);
}
