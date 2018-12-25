#ifndef _SESSION_H_
#define _SESSION_H_

#include "main.h"
#include "parse.h"

#define MAX_RECV_BUFF_SIZE 256

char* socket_read(int fd, int *result);
//char* msg_client_info(size_t *len, const cmd_t *request_client);
int append_null(char *str, int len);

#endif
