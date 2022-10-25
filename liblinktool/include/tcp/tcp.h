#ifndef _TCP_H_
#define _TCP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <errno.h>
#include <sys/un.h>
#include "socketFunc.h"

int tcpClientConnect(int *fd, const char *addr, const short port);
int tcpServerListen(int *fd, const char *addr, const short port, int listenNum);
int tcpClientConnect2(int *fd, void *addr, int domain);
#endif