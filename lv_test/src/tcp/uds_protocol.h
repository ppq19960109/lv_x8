#ifndef _UDS_PROTLCOL_H_
#define _UDS_PROTLCOL_H_

#include "cJSON.h"

#define FRAME_HEADER (0xAA)
#define FRAME_TAIL (0xBB)

#define TYPE "Type"
#define TYPE_SET "SET"
#define TYPE_GET "GET"
#define TYPE_GETALL "GETALL"
#define TYPE_EVENT "EVENT"
#define TYPE_HEART "HEART"

#define DATA "Data"
void mlog_init(void);
int uds_protocol_init(void);
void uds_protocol_deinit(void);
int uds_protocol_recv(char *data, unsigned int len);

int cJSON_Object_isNull(cJSON *object);
int send_getall_uds(void);
int send_set_uds(cJSON *send);
int send_get_uds(cJSON *send);
void register_uds_json_recv_cb(int (*cb)(cJSON *));
#endif