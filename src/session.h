#ifndef _SESSION_H_
#define _SESSION_H_

#include "main.h"

#define MAX_RECV_BUFF_SIZE 256

char* socket_read(int fd, int *result);

#endif

