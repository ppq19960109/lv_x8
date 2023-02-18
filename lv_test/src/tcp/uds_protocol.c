#include "main.h"

#include "uds_protocol.h"

#include "hloop.h"
#include "hbase.h"
#include "hlog.h"
#include "nlog.h"
#include "hsocket.h"
#include "hssl.h"
#include "hmain.h"
#include "hthread.h"

static pthread_mutex_t mutex;
static char g_send_buf[3072];
static int g_seqid = 0;
static hio_t *mio = NULL;

int (*uds_json_recv_cb)(cJSON *);
void register_uds_json_recv_cb(int (*cb)(cJSON *))
{
    uds_json_recv_cb = cb;
}

int cJSON_Object_isNull(cJSON *object) // cJSON判断Object是否为空
{
    char *json = cJSON_PrintUnformatted(object);
    if (strlen(json) == 2 && strcmp(json, "{}") == 0)
    {
        cJSON_free(json);
        return 1;
    }
    cJSON_free(json);
    return 0;
}

static unsigned char CheckSum(unsigned char *buf, int len) // 和校验算法
{
    int i;
    unsigned char ret = 0;
    for (i = 0; i < len; i++)
    {
        ret += *(buf++);
    }
    return ret;
}

int send_to_uds(cJSON *root)
{
    if (mio == NULL)
    {
        LOGW("%s,mio NULL", __func__);
        cJSON_Delete(root);
        return -1;
    }
    char *json = cJSON_PrintUnformatted(root);
    if (json == NULL)
    {
        dzlog_error("%s,cJSON_PrintUnformatted error", __func__);
        cJSON_Delete(root);
        return -1;
    }
    int len = strlen(json);

    printf("send to comm---------------------- cJSON_PrintUnformatted json:%d,%s\n", len, json);
    pthread_mutex_lock(&mutex);
    char *send_buf;
    if (len + 10 > sizeof(g_send_buf))
    {
        send_buf = (char *)malloc(len + 10);
    }
    else
    {
        send_buf = g_send_buf;
    }
    send_buf[0] = FRAME_HEADER;
    send_buf[1] = FRAME_HEADER;
    send_buf[2] = 0;
    send_buf[3] = g_seqid / 256;
    send_buf[4] = g_seqid % 256;
    send_buf[5] = len >> 8;
    send_buf[6] = len;
    memcpy(&send_buf[7], json, len);
    send_buf[7 + len] = CheckSum((unsigned char *)&send_buf[2], len + 5);
    send_buf[8 + len] = FRAME_TAIL;
    send_buf[9 + len] = FRAME_TAIL;

    hio_write(mio, send_buf, len + 10);
    if (len + 10 > sizeof(g_send_buf))
    {
        free(send_buf);
    }
    pthread_mutex_unlock(&mutex);
    cJSON_free(json);
    cJSON_Delete(root);
    return 0;
}

int send_set_uds(cJSON *send)
{
    if (cJSON_Object_isNull(send))
    {
        cJSON_Delete(send);
        dzlog_warn("%s,send NULL", __func__);
        return -1;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, TYPE, TYPE_SET);
    cJSON_AddItemToObject(root, DATA, send);

    send_to_uds(root);
    return 0;
}
int send_get_uds(cJSON *send)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, TYPE, TYPE_GET);
    cJSON_AddItemToObject(root, DATA, send);

    send_to_uds(root);
    return 0;
}
int send_getall_uds()
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, TYPE, TYPE_GETALL);
    cJSON_AddNullToObject(root, DATA);

    send_to_uds(root);
    return 0;
}

static int uds_json_parse(char *value, unsigned int value_len) // uds接受的json数据解析
{
    cJSON *root = cJSON_Parse(value);
    if (root == NULL)
    {
        dzlog_error("JSON Parse Error");
        return -1;
    }

    // char * json = cJSON_PrintUnformatted(root);
    // dzlog_debug("recv from comm-------------------------- json:%s", json);
    // cJSON_free(json);
    dzlog_debug("recv from UI--------------------------:%.*s", value_len, value);

    cJSON *Type = cJSON_GetObjectItem(root, TYPE);
    if (Type == NULL)
    {
        dzlog_error("Type is NULL\n");
        goto fail;
    }
    cJSON *Data = cJSON_GetObjectItem(root, DATA);
    if (Data == NULL)
    {
        dzlog_error("Data is NULL\n");
        goto fail;
    }
    if (uds_json_recv_cb != NULL)
        uds_json_recv_cb(Data);
    else
        dzlog_error("uds_json_recv_cb is NULL\n");
fail:
    cJSON_Delete(root);
    return -1;
}

int uds_protocol_recv(char *data, unsigned int len) // uds接受回调函数，初始化时注册
{
    int ret = 0;
    int msg_len, encry, seqid;
    unsigned char verify;

    for (int i = 0; i < len; ++i)
    {
        if (data[i] == FRAME_HEADER && data[i + 1] == FRAME_HEADER)
        {
            encry = data[i + 2];
            seqid = (data[i + 3] << 8) + data[i + 4];
            msg_len = (data[i + 5] << 8) + data[i + 6];
            if (data[i + 6 + msg_len + 2] != FRAME_TAIL || data[i + 6 + msg_len + 3] != FRAME_TAIL)
            {
                continue;
            }
            // hdzlog_info(&data[i], 6 + msg_len + 4);
            dzlog_debug("uds_recv encry:%d seqid:%d msg_len:%d", encry, seqid, msg_len);
            verify = data[i + 6 + msg_len + 1];
            unsigned char verify_check = CheckSum((unsigned char *)&data[i + 2], msg_len + 5);
            if (verify_check != verify)
            {
                dzlog_error("CheckSum error:%d,%d", verify_check, verify);
                // continue;
            }
            if (msg_len > 0)
            {
                ret = uds_json_parse(&data[i + 6 + 1], msg_len);
                if (ret == 0)
                {
                    i += 6 + msg_len + 3;
                }
            }
        }
    }
    return 0;
}
static void on_recv(hio_t *io, void *buf, int readbytes)
{
    printf("on_recv fd=%d readbytes=%d\n", hio_fd(io), readbytes);
    char localaddrstr[SOCKADDR_STRLEN] = {0};
    char peeraddrstr[SOCKADDR_STRLEN] = {0};
    printf("[%s] <=> [%s]\n",
           SOCKADDR_STR(hio_localaddr(io), localaddrstr),
           SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));
    printf("< %.*s\n", readbytes, (char *)buf);
    uds_protocol_recv((char *)buf, readbytes);
}
static int tcp_client_reconnect_create(hloop_t *loop);
static void reconnect_timer_cb(htimer_t *timer)
{
    tcp_client_reconnect_create(hevent_loop(timer));
}
static void on_close(hio_t *io)
{
    printf("on_close fd=%d error=%d\n", hio_fd(io), hio_error(io));
    mio = NULL;
    hloop_t *loop = hevent_loop(io);
    htimer_add(loop, reconnect_timer_cb, 2000, 1);
}
static void on_connect(hio_t *io)
{
    printf("on_connect fd=%d\n", hio_fd(io));

    char localaddrstr[SOCKADDR_STRLEN] = {0};
    char peeraddrstr[SOCKADDR_STRLEN] = {0};
    printf("connect connfd=%d [%s] => [%s]\n", hio_fd(io),
           SOCKADDR_STR(hio_localaddr(io), localaddrstr),
           SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));

    hio_read_start(io);
    // uncomment to test heartbeat
    // hio_set_heartbeat(sockio, 3000, send_heartbeat);
    send_getall_uds();
}
static int tcp_client_reconnect_create(hloop_t *loop)
{
    static char recvbuf[4096];
    mio = hloop_create_tcp_client(loop, "/tmp/unix_server.domain", -1, on_connect, on_close);
    if (mio == NULL)
    {
        return -20;
    }
    // hio_set_connect_timeout(mio, 20000);
    hio_setcb_read(mio, on_recv);
    hio_set_readbuf(mio, recvbuf, sizeof(recvbuf));
    return 0;
}
static void *uds_protocol_task(void *arg)
{
    hloop_t *loop = hloop_new(0);
    tcp_client_reconnect_create(loop);

    hloop_run(loop);
    hloop_free(&loop);
    return NULL;
}
int uds_protocol_init(void) // uds协议相关初始化
{
    pthread_mutex_init(&mutex, NULL);
    hthread_create(uds_protocol_task, NULL);
    return 0;
}
void uds_protocol_deinit(void) // uds协议相关反初始化
{
    pthread_mutex_destroy(&mutex);
}
