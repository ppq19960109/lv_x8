#include "main.h"
#include "thread2Client.h"
#include "uds_protocol.h"
#include "uds_client_task.h"

#define UNIX_DOMAIN "/tmp/unix_server.domain"
static ThreadTcp threadTcp;

int uds_send(unsigned char *data, unsigned int len)
{
    return thread2ClientSend(&threadTcp, data, len);
}
static int uds_recv(char *data, unsigned int uart_read_len)
{
    return uds_protocol_recv(data, uart_read_len);
}
static int uds_disconnect(void)
{
    printf("---disconnect...\n");
    return 0;
}
static int uds_connect(void)
{
    printf("---connect...\n");
    send_getall_uds();
    return 0;
}

int uds_client_init(void)
{
    printf("%s UNIX_DOMAIN:%s\r\n", __func__, UNIX_DOMAIN);

    static struct sockaddr_un server_addr = {0};
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, UNIX_DOMAIN);

    tcpEvent2Set(&threadTcp, (struct sockaddr *)&server_addr, AF_UNIX, uds_recv, uds_disconnect, uds_connect, 0);
    threadTcp.status = 1;
    return 0;
}
void uds_client_close(void)
{
    thread2ClientClose(&threadTcp);
}
void uds_client_task(void)
{
    thread2ManualHander(&threadTcp);
    // thread2ClientOpen(&threadTcp);
}