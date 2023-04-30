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
static hio_t *mio = NULL;

int (*uds_data_recv_cb)(void *, int);
void register_uds_recv_cb(int (*cb)(void *, int))
{
    uds_data_recv_cb = cb;
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

static int send_to_uds(unsigned char *data, int len)
{
    if (mio == NULL)
    {
        LOGW("%s,mio NULL", __func__);
        return -1;
    }
    mlogHex(data, len);
    pthread_mutex_lock(&mutex);

    int ret = hio_write(mio, data, len);
    pthread_mutex_unlock(&mutex);
    return ret;
}

int send_set_uds(unsigned char *payload, int payload_len)
{
    return roki_uart_send_msg2(32, 192, payload, payload_len, 0, send_to_uds);
}
int send_get_uds(unsigned char *payload, int payload_len)
{
    return roki_uart_send_msg2(32, 190, payload, payload_len, 0, send_to_uds);
}
int send_getall_uds(void)
{
    unsigned char payload = 0;
    return roki_uart_send_msg2(32, 190, &payload, 1, 0, send_to_uds);
}

static void uds_protocol_recv(unsigned char *data, int len)
{
    roki_uart_recv_msg(data, &len, uds_data_recv_cb);
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

    uds_protocol_recv((unsigned char *)buf, readbytes);
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
static void mlogger(int loglevel, const char *buf, int len)
{
    if (loglevel >= LOG_LEVEL_ERROR)
    {
        stderr_logger(loglevel, buf, len);
    }
    else
        stdout_logger(loglevel, buf, len);
    if (loglevel >= LOG_LEVEL_INFO)
    {
        file_logger(loglevel, buf, len);
    }
    // network_logger(loglevel, buf, len);
}
static void *uds_protocol_task(void *arg)
{
    hloop_t *loop = hloop_new(0);

    tcp_client_reconnect_create(loop);

    hloop_run(loop);
    hloop_free(&loop);
    return NULL;
}
void mlog_init(void)
{
    hlog_set_handler(mlogger);
    hlog_set_file("LVGL.log");
    hlog_set_max_filesize(1024 * 100);
    hlog_set_format(DEFAULT_LOG_FORMAT);
    hlog_set_level(LOG_LEVEL_DEBUG);
    hlog_set_remain_days(1);
    logger_enable_color(hlog, 1);
}
static void on_reload(void *userdata)
{
    hlogi("reload confile [%s]", g_main_ctx.confile);
    // parse_confile(g_main_ctx.confile);
}
int uds_protocol_init(void) // uds协议相关初始化
{
    pthread_mutex_init(&mutex, NULL);
    signal_init(on_reload, NULL);
    hthread_create(uds_protocol_task, NULL);
    return 0;
}
void uds_protocol_deinit(void) // uds协议相关反初始化
{
    pthread_mutex_destroy(&mutex);
}
