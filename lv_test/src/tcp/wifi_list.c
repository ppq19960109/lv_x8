#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "wifi_list.h"
LIST_HEAD(WIFI_LIST);

int signalLevel(int rssi)
{
    if (rssi <= -100)
        return 0;
    else if (rssi < -75)
        return 1;
    else if (rssi < -55)
        return 2;
    else
        return 3;
}
int encrypType(const char *flags)
{
    if (strstr(flags, "WPA") != NULL)
        return 1;
    else if (strstr(flags, "WEP") != NULL)
        return 2;
    else
        return 0;
}

void wifi_list_del(wifi_node_t *ptr)
{
    list_del(&ptr->node);
    free(ptr);
    ptr = NULL;
}

void wifi_list_add(wifi_node_t *cur)
{
    wifi_node_t *ptr, *next;
    // printf("%s,ssid:%s,rssi:%d,flags:%d\n", __func__, cur->ssid, cur->rssi, cur->flags);
    list_for_each_entry_safe(ptr, next, &WIFI_LIST, node)
    {
        if (cur->rssi > ptr->rssi)
        {
            list_add_tail(&cur->node, &ptr->node);
            return;
        }
    }
    if (list_empty(&WIFI_LIST) == 0)
    {
        ptr = list_entry(WIFI_LIST.prev, wifi_node_t, node);
        // printf("%s,last ssid:%s,rssi:%d,flags:%d\n", __func__, ptr->ssid, ptr->rssi, ptr->flags);
    }
    list_add(&cur->node, &ptr->node);
}

void wifi_list_each(int (*cb)(void *))
{
    int i = 0;
    wifi_node_t *ptr;
    list_for_each_entry(ptr, &WIFI_LIST, node)
    {
        printf("%s,ssid:%s,rssi:%d,flags:%d\n", __func__, ptr->ssid, ptr->rssi, ptr->flags);
        if (cb != NULL)
            cb(ptr);
        // if (++i >= 15)
        //     break;
    }
}
void get_wifi_list_flags(const char *ssid)
{
    wifi_node_t *ptr, *next;
    list_for_each_entry_safe(ptr, next, &WIFI_LIST, node)
    {
        // printf("%s,ssid:%s,rssi:%d,flags:%d\n", __func__, ptr->ssid, ptr->rssi, ptr->flags);
        if (strcmp(ssid, ptr->ssid) == 0)
            return ptr->flags;
    }
    return 1;
}
void wifi_list_clear(void)
{
    wifi_node_t *ptr, *next;

    list_for_each_entry_safe(ptr, next, &WIFI_LIST, node)
    {
        wifi_list_del(ptr);
    }
}
