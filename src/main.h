#ifndef _TCP_H_
#define _TCP_H_

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
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

#include "cmd_agent.h"
#include "json.h"
#include "session.h"

#define PORT        88

#define EPOLL_SIZE 60 
#define EPOLL_EVENT_SIZE 100 

int do_use_fd (int efd, struct epoll_event ev);
int do_accept (int efd, int sfd);
int do_del_fd(int efd, int cfd);
int do_add_fd(int efd, int cfd);
int parse_request_from(char *buf, int buf_len);

int setnonblock (int sock);
int settcpnodelay (int sock);
int setreuseaddr(int sock);
int get_portnumber_from_arg(int argc, char **argv);
int init_epoll (int eventsize);
int init_acceptsock (unsigned short port);

int do_use_fd (int efd, struct epoll_event ev);
int do_accept (int efd, int sfd);
int do_add_fd(int efd, int cfd);
int do_add_fd(int efd, int cfd);
int do_modify_fd(int efd, int cfd);
int do_del_fd(int efd, int cfd);

#endif

