#include "roki_uart_parse_msg.h"

#define MSG_MIN_LEN (9)

void mlog_hex(const void *buf, int len, const char *file, const int line, const char *func)
{
    unsigned char *data = (unsigned char *)buf;
    // printf("[%s:%d:%s]", __FILE__, __LINE__, __FUNCTION__);
    printf("[%s:%d:%s]", file, line, func);
    for (int i = 0; i < len; ++i)
    {
        if (i % 16 == 0)
            printf("\n");
        printf("%02x ", data[i]);
    }
    printf("\n");
}

unsigned short crc16_maxim_single(const unsigned char *ptr, int len)
{
    unsigned int i;
    unsigned short crc = 0x0000;

    while (len--)
    {
        crc ^= *ptr++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc = (crc >> 1);
        }
    }

    return ~crc;
}

int roki_uart_send_msg(unsigned char *msg, const int msg_len, const unsigned char enc, uart_cb send_cb)
{
    int index = 0;
    unsigned char *send_msg = (unsigned char *)malloc(MSG_MIN_LEN + msg_len + 1);
    if (send_msg == NULL)
    {
        fprintf(stderr, "malloc error\n");
        return -1;
    }
    send_msg[index++] = 0xfe;
    send_msg[index++] = 0x5c;
    if (enc > 0)
        send_msg[index++] = 0x03;
    else
        send_msg[index++] = 0x02;
    send_msg[index++] = msg_len >> 8;
    send_msg[index++] = msg_len & 0xff;
    if (enc > 0)
        send_msg[index++] = enc;
    if (msg_len > 0 && msg != NULL)
    {
        memcpy(&send_msg[index], msg, msg_len);
        index += msg_len;
    }
    unsigned short crc16 = crc16_maxim_single((const unsigned char *)(send_msg - msg_len), msg_len);
    send_msg[index++] = crc16 >> 8;
    send_msg[index++] = crc16 & 0xff;

    int ret = -1;
    if (send_cb != NULL)
        ret = send_cb(send_msg, index);
    free(send_msg);

    return ret;
}

int roki_uart_send_msg2(unsigned char cmd_key, unsigned char cmd_id, unsigned char *msg, const int msg_len, const unsigned char enc, uart_cb send_cb)
{
    int index = 0;
    int len = msg_len + 2;
    unsigned char *send_msg = (unsigned char *)malloc(MSG_MIN_LEN + len + 1);
    if (send_msg == NULL)
    {
        fprintf(stderr, "malloc error\n");
        return -1;
    }
    send_msg[index++] = 0xfe;
    send_msg[index++] = 0x5c;
    if (enc > 0)
        send_msg[index++] = 0x03;
    else
        send_msg[index++] = 0x02;
    send_msg[index++] = len >> 8;
    send_msg[index++] = len & 0xff;
    if (enc > 0)
        send_msg[index++] = enc;
    send_msg[index++] = cmd_key;
    send_msg[index++] = cmd_id;
    if (msg_len > 0 && msg != NULL)
    {
        memcpy(&send_msg[index], msg, msg_len);
        index += msg_len;
    }
    unsigned short crc16 = crc16_maxim_single((const unsigned char *)(send_msg - len), len);
    send_msg[index++] = crc16 >> 8;
    send_msg[index++] = crc16 & 0xff;

    int ret = -1;
    if (send_cb != NULL)
        ret = send_cb(send_msg, index);
    free(send_msg);

    return ret;
}

int roki_uart_parse_func(const unsigned char *in, const int in_len, int *end, uart_cb recv_cb)
{
    int index = 0, i;
    if (in_len < 2)
    {
        *end = 0;
        fprintf(stderr, "input len too small\n");
        return UART_READ_LEN_SMALL;
    }

    for (i = 0; i < in_len - 1; ++i)
    {
        if (in[i] == 0xFE && in[i + 1] == 0x5C)
            break;
    }
    if (i >= in_len - 1)
    {
        fprintf(stderr, "no header was detected\n");
        return UART_READ_NO_HEADER;
    }
    index = i;

    if (in_len - index < MSG_MIN_LEN)
    {
        *end = index;
        fprintf(stderr, "input len too small\n");
        return UART_READ_LEN_SMALL;
    }
    int msg_index = 2;

    const unsigned char encrypt = in[index + msg_index];
    msg_index += 1;
    unsigned short data_len = in[index + msg_index] * 256 + in[index + msg_index + 1];
    msg_index += 2;
    if (data_len > 256)
    {
        *end = index + 1;
        fprintf(stderr, "input data len error\n");
        return UART_READ_LEN_ERR;
    }
    unsigned char encrypt_key = 0;
    if (encrypt & 0x01)
    {
        encrypt_key = in[index + msg_index];
        msg_index += 1;
    }
    const unsigned char *data = &in[index + msg_index];
    msg_index += data_len;
    if (index + msg_index + 2 > in_len)
    {
        *end = index;
        fprintf(stderr, "input data len too small\n");
        return UART_READ_LEN_SMALL;
    }

    unsigned short crc16 = crc16_maxim_single(&in[index + msg_index - data_len], data_len);
    unsigned short check_sum = in[index + msg_index] * 256 + in[index + msg_index + 1];
    printf("crc16:%x,check_sum:%x\n", crc16, check_sum);
    msg_index += 2;

    *end = index + msg_index;
    if (crc16 != check_sum)
    {
        fprintf(stderr, "data check error\n");
        // return UART_READ_CHECK_ERR;
    }
    //----------------------
    if (recv_cb != NULL)
        recv_cb((unsigned char *)data, data_len);
    return UART_READ_VALID;
}

void data_cycle_parse(unsigned char *in, int *in_len, uart_parse_cb parse_cb, uart_cb recv_cb)
{
    int index = 0, end;
    int msg_len = *in_len;
    uart_read_status_t status;
    for (;;)
    {
        // printf("index:%d,end:%d,msg_len:%d\n", index, end, msg_len);
        status = parse_cb(&in[index], msg_len, &end, recv_cb);
        msg_len -= end;
        index += end;

        if (status == UART_READ_VALID || status == UART_READ_CHECK_ERR || status == UART_READ_TAILER_ERR || status == UART_READ_LEN_ERR)
        {
        }
        else if (status == UART_READ_LEN_SMALL)
        {
            break;
        }
        else if (status == UART_READ_NO_HEADER)
        {
            msg_len = 0;
            break;
        }
        else
        {
            fprintf(stderr, "error return value\n");
        }
        if (msg_len <= 0)
        {
            break;
        }
    }

    // printf("last move index:%d,msg_len:%d\n", index, msg_len);
    if (msg_len <= 0)
    {
        msg_len = 0;
    }
    else
    {
        memmove(in, &in[index], msg_len);
    }
    *in_len = msg_len;
}

void roki_uart_recv_msg(unsigned char *in, int *in_len, uart_cb recv_cb)
{
    data_cycle_parse(in, in_len, roki_uart_parse_func, recv_cb);
}
