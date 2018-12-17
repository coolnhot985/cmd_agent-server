#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "json.h"
#include "session.h"
#include "parse.h"
#include "log.h"

#define DEBUG(x, a...) printf("%s: " x "\n", __func__, ## a) 
#define BREAK(x, a...) printf("==## file [%s] func [%s] line [%d] " x "\n", __FILE__, __func__, __LINE__, ## a)

#define PORT                88
#define EPOLL_SIZE          60
#define EPOLL_EVENT_SIZE    100

#define AGENT_TYPE_LINUX_NVIDIA     "linux-claymore-nvidia"
#define AGENT_TYPE_UX               "ux"

typedef enum {
    REQ_INVALID         = 0,
    REQ_UX,
    REQ_LINUX_CLAYMORE,
    REQ_WINDOW_CLAYMORE,
}agent_type_t;

#endif
