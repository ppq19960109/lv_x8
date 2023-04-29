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

int cJSON_Object_isNull(cJSON *object);
int send_getall_uds(void);
int send_get_uds(unsigned char *payload, int payload_len);
int send_set_uds(unsigned char *payload, int payload_len);

void register_uds_recv_cb(int (*cb)(void *, int));
#endif
