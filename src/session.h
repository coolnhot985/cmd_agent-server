#ifndef _SESSION_H_
#define _SESSION_H_

#include "main.h"

#define MAX_RECV_BUFF_SIZE 256

char* socket_read(int fd, int *result);
char* msg_client_info(size_t *len, const char *req_cmd, const char *req_path);
int append_null(char *str, int len);

#endif

