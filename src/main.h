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

int settcpnodelay (int sock);
int setreuseaddr(int sock);
int init_epoll (int eventsize);
int init_acceptsock (unsigned short port);
int do_accept (int efd, int sfd);
int do_add_fd(int efd, int cfd);
int do_modify_fd(int efd, int cfd);
int do_del_fd(int efd, int cfd);


#endif
