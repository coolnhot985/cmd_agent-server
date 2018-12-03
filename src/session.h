#ifndef _SESSION_H_
#define _SESSION_H_

#include "tcp.h"
#include "parse.h"
#include "main.h"
#include "json.h"
#include "json_tokener.h"

#define MAX_RECV_BUFF_SIZE 10

char* socket_read(int fd);
int do_use_fd (int efd, struct epoll_event ev);

#endif

