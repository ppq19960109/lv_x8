#ifndef _ROKI_UART_PARSE_H_
#define _ROKI_UART_PARSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

typedef enum
{
    UART_READ_VALID = 0,
    UART_READ_NO_HEADER,
    UART_READ_LEN_SMALL,
    UART_READ_LEN_ERR,
    UART_READ_CHECK_ERR,
    UART_READ_TAILER_ERR
} uart_read_status_t;

typedef int (*uart_cb)(unsigned char *, int);
typedef int(uart_parse_cb)(const unsigned char *, const int, int *, uart_cb);

void mlog_hex(const void *buf, int len, const char *file, const int line, const char *func);
#define mlogHex(buf, buf_len) mlog_hex(buf, buf_len, __FILE__, __LINE__, __FUNCTION__)

unsigned short crc16_maxim_single(const unsigned char *ptr, int len);

int roki_uart_send_msg(unsigned char *msg, const int msg_len, const unsigned char enc, uart_cb send_cb);
int roki_uart_send_msg2(unsigned char cmd_key, unsigned char cmd_id, unsigned char *msg, const int msg_len, const unsigned char enc, uart_cb send_cb);

int roki_uart_parse_func(const unsigned char *in, const int in_len, int *end, uart_cb recv_cb);
void data_cycle_parse(unsigned char *in, int *in_len, uart_parse_cb parse_cb, uart_cb recv_cb);
void roki_uart_recv_msg(unsigned char *in, int *in_len, uart_cb recv_cb);
#endif
