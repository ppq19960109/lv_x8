#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "list.h"
#include "tcp.h"
#include "select_tcp_server.h"

struct Select_Server_Event select_server_event;
static struct App_Select_Client_Tcp app_select_client_Tcp_Server = {
    .select_client_event.fd = -1};
static struct App_Select_Client_Tcp app_select_client_Tcp_Client[SELECT_TCP_MAX_CLIENT];

void app_select_client_tcp_server_send(char *data, unsigned short len)
{
    pthread_mutex_lock(&app_select_client_Tcp_Server.mutex);
    int i;
    for (i = 0; i < SELECT_TCP_MAX_CLIENT; ++i)
    {
        if (app_select_client_Tcp_Client[i].select_client_event.fd >= 0)
            send(app_select_client_Tcp_Client[i].select_client_event.fd, data, len, 0);
    }
    pthread_mutex_unlock(&app_select_client_Tcp_Server.mutex);
}

static int app_select_server_recv_cb(void *arg)
{
    struct Select_Client_Event *client_event = (struct Select_Client_Event *)arg;
    struct App_Select_Client_Tcp *app_select_tcp = container_of(client_event, struct App_Select_Client_Tcp, select_client_event);
    app_select_tcp->recv_len = recv(client_event->fd, app_select_tcp->recv_buf, sizeof(app_select_tcp->recv_buf), 0); //读取客户端发过来的数据

    if (app_select_tcp->recv_len <= 0)
    {
        printf("recv[fd=%d] error[%d]:%s\n", client_event->fd, errno, strerror(errno));

        delete_select_client_event(&select_server_event, client_event);
        close(client_event->fd);
        client_event->fd = -1;
        if (app_select_tcp->disconnect_cb != NULL)
            app_select_tcp->disconnect_cb();

        return -1;
    }
    else
    {
        app_select_tcp->recv_buf[app_select_tcp->recv_len] = '\0'; //手动添加字符串结束标记
        printf("%s,[%d]: %d\n", __func__, client_event->fd, app_select_tcp->recv_len);
        if (app_select_tcp->recv_cb != NULL)
            app_select_tcp->recv_cb(app_select_tcp->recv_buf, app_select_tcp->recv_len);
    }
    return 0;
}

static int app_add_select_tcp_client_event(int cfd)
{
    int i;
    for (i = 0; i < SELECT_TCP_MAX_CLIENT; ++i)
    {
        if (app_select_client_Tcp_Client[i].select_client_event.fd < 0)
        {
            app_select_client_Tcp_Client[i].select_client_event.fd = cfd;
            app_select_client_Tcp_Client[i].select_client_event.read_cb = app_select_server_recv_cb;
            app_select_client_Tcp_Client[i].recv_cb = app_select_client_Tcp_Server.recv_cb;
            app_select_client_Tcp_Client[i].connect_cb = app_select_client_Tcp_Server.connect_cb;
            app_select_client_Tcp_Client[i].disconnect_cb = app_select_client_Tcp_Server.disconnect_cb;
            if (add_select_server_event(&app_select_client_Tcp_Client[i].select_client_event) < 0)
            {
                app_select_client_Tcp_Client[i].select_client_event.fd = -1;
                return -1;
            }
            if (app_select_client_Tcp_Client[i].connect_cb != NULL)
                app_select_client_Tcp_Client[i].connect_cb();
        }
    }
    if (i == SELECT_MAX_CLIENT)
    {
        return -1;
    }
    return 0;
}

static int app_select_server_accetp_cb(void *arg)
{
    struct Select_Client_Event *client_event = (struct Select_Client_Event *)arg;
    // struct App_Select_Client_Tcp *app_select_tcp = container_of(client_event, struct App_Select_Client_Tcp, select_client_event);
    int res;
    struct sockaddr cin;
    socklen_t clen = sizeof(cin);

    int cfd;
    if ((cfd = accept(client_event->fd, (struct sockaddr *)&cin, &clen)) == -1)
    {
        if (errno != EAGAIN && errno != EINTR)
        {
            sleep(1);
        }
        printf("%s:accept,%s\n", __func__, strerror(errno));
        return -1;
    }

    res = app_add_select_tcp_client_event(cfd);
    if (res < 0)
    {
        close(cfd);
    }
    return res;
}

void app_select_tcp_server_init(void)
{
    pthread_mutex_init(&app_select_client_Tcp_Server.mutex, NULL);
    for (int i = 0; i < SELECT_TCP_MAX_CLIENT; ++i)
    {
        pthread_mutex_init(&app_select_client_Tcp_Client[i].mutex, NULL);
    }
}

void app_select_tcp_server_deinit(void)
{
    for (int i = 0; i < SELECT_TCP_MAX_CLIENT; ++i)
    {
        pthread_mutex_destroy(&app_select_client_Tcp_Client[i].mutex);
    }
    pthread_mutex_destroy(&app_select_client_Tcp_Server.mutex);
}

void app_select_tcp_server_task()
{
    for (int i = 0; i < SELECT_TCP_MAX_CLIENT; ++i)
    {
        app_select_client_Tcp_Client[i].select_client_event.fd = -1;
    }
    select_server_init(&select_server_event);
    app_select_tcp_server_init();
    int fd;
    app_select_client_Tcp_Server.select_client_event.fd = tcpServerListen(&fd, "127.0.0.1", 8888, SELECT_TCP_MAX_CLIENT);
    app_select_client_Tcp_Server.select_client_event.read_cb = app_select_server_accetp_cb;
    app_select_client_Tcp_Server.recv_cb = NULL;
    app_select_client_Tcp_Server.connect_cb = NULL;
    app_select_client_Tcp_Server.disconnect_cb = NULL;
    add_select_server_event(&select_server_event, &app_select_client_Tcp_Server.select_client_event);
    select_server_task(&select_server_event, 200);
    select_server_deinit(&select_server_event);
    app_select_tcp_server_deinit();
}