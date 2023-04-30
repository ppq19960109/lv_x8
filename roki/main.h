#ifndef __MAIN_H_
#define __MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/reboot.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/select.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include "cJSON.h"
#include "base64.h"
#include "cmd_run.h"
#include "commonFunc.h"
#include "networkFunc.h"
#include "POSIXTimer.h"
#include "UartCfg.h"

#include "curl/curl.h"

// #include "hloop.h"
// #include "hbase.h"
// #include "hlog.h"
// #include "nlog.h"
// #include "hsocket.h"
// #include "hssl.h"
// #include "hmain.h"
// #include "hthread.h"

#include "lv_roki/lv_roki_ext.h"

#include "roki_dev_state.h"
#include "uds_protocol.h"
#include "hv_http_client.h"
#include "roki_uart_parse_msg.h"

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
