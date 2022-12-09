#ifndef _WIFI_LIST_H_
#define _WIFI_LIST_H_

#include <stdbool.h>
#include "list.h"

typedef struct
{
    char ssid[33];
    char bssid[33];
    char flags[33];
    char connected;
    int rssi;
    struct list_head node;
} wifi_node_t;
int signalLevel(int rssi);
int encrypType(const char *flags);
void wifi_list_add(wifi_node_t *cur);
void wifi_list_clear(void);
void wifi_list_each(int (*cb)(void *));
#endif