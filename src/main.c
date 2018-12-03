#include "main.h"

int main (int argc, char **argv) {
    struct epoll_event  *events; 
    const int           poolsize        = EPOLL_SIZE; 
    const int           epollsize       = EPOLL_EVENT_SIZE; 
    int                 port            = PORT; 

    int sfd, efd; 
    int i; 
    int max_got_events; 
    int result; 

    efd = init_epoll (epollsize); 
    if (efd < 0) { 
        perror ("init_epoll error"); 
        return -1; 
    } 

    events = (struct epoll_event *) malloc (sizeof (*events) * poolsize); 
    if (NULL == events) { 
        perror ("epoll_create (0) error"); 
        return -1; 
    } 

    sfd = init_acceptsock (port); 
    if (sfd < 0) { 
        perror ("init_acceptsock error"); 
        return 1; 
    } 

    printf("Running Server port %d\n",port); 

    result = do_add_fd(efd,sfd); 
    if (result < 0) { 
        perror ("epoll_ctl error"); 
        return 1; 
    } 

    while (1) {
        max_got_events = epoll_wait (efd, events, poolsize, -1); 
        for (i = 0; i < max_got_events; i++) {
            if (events[i].data.fd == sfd) {
                do_accept(efd, sfd); 
            } else {
                do_use_fd(efd, events[i]); 
            } 
        } 
    } 
    return 1; 
}

int setnonblock (int sock) {
    int flags = fcntl (sock, F_GETFL); 
    flags |= O_NONBLOCK; 
    if (fcntl (sock, F_SETFL, flags) < 0) { 
        perror("fcntl, executing nonblock error"); 
        return -1; 
    } 
    return 0; 
} 

int settcpnodelay (int sock) {
    int flag = 1; 
    int result = setsockopt (sock,    /* socket affected */ 
            IPPROTO_TCP,    /* set option at TCP level */ 
            TCP_NODELAY,    /* name of option */ 
            (char *) &flag,    /* the cast is historical  cruft */ 
            sizeof (int));    /* length of option value */ 
    return result; 
} 

int setreuseaddr(int sock) {
    int flag = 1; 
    int result = setsockopt (sock,    /* socket affected */ 
            SOL_SOCKET,    /* set option at TCP level */ 
            SO_REUSEADDR,    /* name of option */ 
            (char *) &flag,    /* the cast is historical  cruft */ 
            sizeof (int));    /* length of option value */ 
    return result; 
} 

int init_epoll (int eventsize) { 
    int efd; 

    if ((efd = epoll_create (eventsize)) < 0) { 
        perror ("epoll_create (1) error"); 
        return -1; 
    } 
    return efd; 
} 

int init_acceptsock (unsigned short port) { 
    struct sockaddr_in addr;

    int sfd = socket (AF_INET, SOCK_STREAM, 0); 

    if (sfd == -1) { 
        perror("socket error :"); 
        close(sfd); 
        return -1; 
    } 

    setreuseaddr(sfd); 
    addr.sin_family = AF_INET; 
    addr.sin_port = htons (port); 
    addr.sin_addr.s_addr = htonl (INADDR_ANY); 

    if (bind(sfd,(struct sockaddr *)&addr, sizeof(addr)) == -1) { 
        close(sfd); 
        return -2; 
    } 
    listen(sfd, 5); 
    return sfd; 
} 

int do_accept (int efd, int sfd) {
    int sock_client_fd = 0;

    socklen_t clilen;
    struct sockaddr_in clientaddr;

    sock_client_fd = accept(sfd, (struct sockaddr *) &clientaddr, &clilen);
    if (sock_client_fd < 0) {
        perror ("Accept error"); 
        return -1; 
    } 

#ifdef USE_ET 
    setnonblock (sock_client_fd); 
#endif // USE_ET 
    settcpnodelay (sock_client_fd); 

    do_add_fd(efd, sock_client_fd); 
    
    return sock_client_fd; 
} 

int do_add_fd(int efd, int cfd) {
    struct epoll_event ev; 

    ev.events = EPOLLIN; 
#ifdef USE_ET 
    ev.events |= EPOLLET; 
#ifdef USE_ETONESHOT 
    ev.events |= EPOLLONESHOT; 
#endif // USE_ETONESHOT 
#endif // USE_ET 
    ev.data.fd = cfd; 
    return epoll_ctl (efd, EPOLL_CTL_ADD, cfd, &ev); 
} 

// fixme 
int do_modify_fd(int efd, int cfd) {
struct epoll_event ev; 

    ev.events = EPOLLIN; 
#ifdef USE_ET 
    ev.events |= EPOLLET; 
#ifdef USE_ETONESHOT 
    ev.events |= EPOLLONESHOT; 
#endif // USE_ETONESHOT 
#endif // USE_ET 
    ev.data.fd = cfd; 
    return epoll_ctl (efd, EPOLL_CTL_MOD, cfd, &ev); 
} 


int do_del_fd(int efd, int cfd) {
    struct epoll_event ev; 

    return epoll_ctl (efd, EPOLL_CTL_DEL, cfd, &ev); 
} 

