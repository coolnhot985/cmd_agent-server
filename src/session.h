#ifndef _SESSION_H_
#define _SESSION_H_

#include "main.h"
#include "parse.h"

#define MAX_RECV_BUFF_SIZE 256

char* socket_read(int fd, int *result);
int append_null(char *str, int len);

#endif
