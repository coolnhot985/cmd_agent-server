#include "main.h"

#define USE_ET 1

#define MAXFDS 16 * 1024

typedef enum { INITIAL_ACK, WAIT_FOR_MSG, IN_MSG } ProcessingState;

#define SENDBUF_SIZE 1024
#define SYNACK_LEN 8

typedef struct {
    ProcessingState state;
    char *send_data;
    int sendbuf_end;
    int sendptr;
} peer_state_t;

typedef struct _fd_status_t{
    bool want_read;
    bool want_write;
}fd_status_t;

const fd_status_t fd_status_R = {.want_read = true, .want_write = false};
const fd_status_t fd_status_W = {.want_read = false, .want_write = true};
const fd_status_t fd_status_RW = {.want_read = true, .want_write = true};
const fd_status_t fd_status_NORW = {.want_read = false, .want_write = false};

// Each peer is globally identified by the file descriptor (fd) it's connected
// on. As long as the peer is connected, the fd is unique to it. When a peer
// disconnects, a new peer may connect and get the same fd. on_peer_connected
// should initialize the state properly to remove any trace of the old peer on
// the same fd.
peer_state_t global_state[MAXFDS];

// Callbacks (on_XXX functions) return this status to the main loop; the status
// instructs the loop about the next steps for the fd for which the callback was
// invoked.
// want_read=true means we want to keep monitoring this fd for reading.
// want_write=true means we want to keep monitoring this fd for writing.
// When both are false it means the fd is no longer needed and can beclosed.

fd_status_t on_peer_connected(int sockfd, const struct sockaddr_in* peer_addr, socklen_t peer_addr_len);
fd_status_t on_peer_ready_recv(MYSQL *conn, int fd, session_t *session);
fd_status_t on_peer_ready_send(int sockfd);

fd_status_t on_peer_connected(int sockfd, const struct sockaddr_in* peer_addr, 
        socklen_t peer_addr_len) {
    assert(sockfd < MAXFDS);
    report_peer_connected(peer_addr, peer_addr_len);

    // Initialize state to send back a '*' to the peer immediately.
    peer_state_t* peerstate = &global_state[sockfd];

    peerstate->state = INITIAL_ACK;
    peerstate->sendptr = 0;
    peerstate->sendbuf_end = SYNACK_LEN;

    // Signal that this socket is ready for writing now.
    // return fd_status_W;
    return fd_status_R;
}

fd_status_t on_peer_ready_recv(MYSQL *conn, int fd, session_t *session) {
    assert(fd < MAXFDS);
    peer_state_t* peerstate = &global_state[fd];
    char    *recv_data      = NULL;
    char    *send_data      = NULL;
    bool    ready_to_send   = false;
    int     ret             = 0;
    size_t  len_send_data   = 0;
    size_t  result          = 0;
    
    recv_data = socket_read(fd, &ret);
    if (recv_data == NULL) {
        mj_free(recv_data);
        return fd_status_NORW;
    }

    if (ret == -1) {
        return fd_status_NORW;
    } else if (ret == -2) {
        return fd_status_R;
    } else if (ret == -3) {
        perror_die("recv");
    }

    DEBUG("recv_data [%s] fd [%d]", recv_data, fd);

    json_object     *recv_data_json;
    agent_type_t    agent_type;

    recv_data_json = parse_string_to_json(recv_data);
    agent_type = get_agent_type(recv_data_json);
   
    if (agent_type == REQ_UX) {
        session->fd         = fd;
        session->agent_type = REQ_UX;
        parse_json_cmd(recv_data_json, session);
    } else if (agent_type == REQ_LINUX_CLAYMORE) {
        session->fd         = fd;
        session->agent_type = REQ_LINUX_CLAYMORE;
        parse_json_agent(recv_data_json, session);
    }

    switch(agent_type) {
        case (REQ_INVALID):
            break;
        case (REQ_UX):
            /* UX 요청의 맥으로 세션테이블을 조회해 FD 를 얻음 */
            ret = mysql_insert_fd(conn, "UX_SESSION", session->fd, REQ_UX);
            ret = mysql_select_fd(conn, session->miner_mac);
            if (ret < 0) {
                /* 클라이언트 접속정보 쿼리 실패*/
                DEBUG("Fail : not found session");
            } else {
                /* 클라이언트 접속정보 쿼리 성공 - 클라이언트 세션과 연결 */
                send_data = msg_client_info(&len_send_data, session);
                result = send(ret, send_data, len_send_data, MSG_CONFIRM);
                if (result > 0) {
                    /* 접속된 클라이언트로 실제 트레픽이 발송됨*/
                } else {
                    /** 세션테이블에서 FD 로 가져와 클라이언트로 접속시도
                      * 세션테이블 FD 동기화 실패로 클라이언트와 통신실패 */
                    DEBUG("fail : session_table_match_non_client");
                }
            }

            break; 
        case (REQ_LINUX_CLAYMORE):
#if 0
            /* 핸드쉐이크는 없어지는게 맞는거같다 */
            peerstate->send_data = (char*)malloc(SYNACK_LEN);
            memcpy(peerstate->send_data, "SYN_ACK\0", SYNACK_LEN);
            ready_to_send = true;
#endif 
            /* 클라이언트 세션이 맺어지면 해당 세션FD 를 디비에 저장 */
            ret = mysql_insert_fd(conn, session->miner_mac, session->fd, REQ_LINUX_CLAYMORE);
            break;
        case (REQ_WINDOW_CLAYMORE):
            break;
    }

#if 0
    for (int i = 0; i < nbytes; ++i) {
        switch (peerstate->state) {
            case INITIAL_ACK:
                assert(0 && "can't reach here");
                break;
            case WAIT_FOR_MSG:
                if (buf[i] == '^') {
                    peerstate->state = IN_MSG;
                }
                break;
            case IN_MSG:
                if (buf[i] == '$') {
                    peerstate->state = WAIT_FOR_MSG;
                } else {
                    assert(peerstate->sendbuf_end < SENDBUF_SIZE);
                    peerstate->sendbuf[peerstate->sendbuf_end++] = buf[i] + 1;
                    ready_to_send = true;
                }
                break;
        }
    }
#endif
    // Report reading readiness iff there's nothing to send to the peer as a
    // result of the latest recv.

    //ready_to_send = true;
    
    // ready_to_send 플래그에 따라서 다음 상대가 read 혹은 write 로 천이됨
    mj_free(recv_data);
    json_object_put(recv_data_json);
    return (fd_status_t){.want_read = !ready_to_send, 
        .want_write = ready_to_send};
}

fd_status_t on_peer_ready_send(int sockfd) {
    assert(sockfd < MAXFDS);
    peer_state_t* peerstate = &global_state[sockfd];

    if (peerstate->sendptr >= peerstate->sendbuf_end) {
        // Nothing to send.
        return fd_status_RW;
    }
    int sendlen = peerstate->sendbuf_end - peerstate->sendptr;
    int nsent = send(sockfd, peerstate->send_data, sendlen, 0);
    if (nsent == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return fd_status_W;
        } else {
            perror_die("send");
        }
    }
    if (nsent < sendlen) {
        peerstate->sendptr += nsent;
        return fd_status_W;
    } else {
        // Everything was sent successfully; reset the send queue.
        peerstate->sendptr = 0;
        peerstate->sendbuf_end = 0;

        // Special-case state transition in if we were in INITIAL_ACK until now.
        if (peerstate->state == INITIAL_ACK) {
            peerstate->state = WAIT_FOR_MSG;
        }

        return fd_status_R;
    }
}

int main(int argc, const char** argv) {
    MYSQL   *conn               = NULL;
    static session_t *session   = NULL;

    int epollfd = 0;

    conn = mysql_conn(); 
    if (conn == NULL) {
        DEBUG("Fail : conn databases");
        return -1;
    }

    setvbuf(stdout, NULL, _IONBF, 0);

    int portnum = 88;
    if (argc >= 2) {
        portnum = atoi(argv[1]);
    }
    printf("Serving on port %d\n", portnum);

    int listener_sockfd = listen_inet_socket(portnum);
    make_socket_non_blocking(listener_sockfd);

    epollfd = epoll_create1(0);
    if (epollfd < 0) {
        perror_die("epoll_create1");
    }

    struct epoll_event accept_event;
    accept_event.data.fd = listener_sockfd;
    accept_event.events = EPOLLIN;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listener_sockfd, &accept_event) < 0) {
        perror_die("epoll_ctl EPOLL_CTL_ADD");
    }

    struct epoll_event* events = calloc(MAXFDS, sizeof(struct epoll_event));
    if (events == NULL) {
        die("Unable to allocate memory for epoll_events");
    }

    while (1) {
        session = (session_t*)malloc(sizeof(session_t));
        int nready = epoll_wait(epollfd, events, MAXFDS, -1);
        for (int i = 0; i < nready; i++) {
            if (events[i].events & EPOLLERR) {
                perror_die("epoll_wait returned EPOLLERR");
            }

            if (events[i].data.fd == listener_sockfd) {
                // The listening socket is ready; 
                // this means a new peer is connecting.

                struct sockaddr_in peer_addr;
                socklen_t peer_addr_len = sizeof(peer_addr);
                int newsockfd = accept(listener_sockfd, 
                        (struct sockaddr*)&peer_addr, &peer_addr_len);

                if (newsockfd < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // This can happen due to the nonblocking socket mode; in this
                        // case don't do anything, but print a notice (since these events
                        // are extremely rare and interesting to observe...)
                        printf("accept returned EAGAIN or EWOULDBLOCK\n");
                    } else {
                        perror_die("accept");
                    }
                } else {
                    make_socket_non_blocking(newsockfd);
                    if (newsockfd >= MAXFDS) {
                        die("socket fd (%d) >= MAXFDS (%d)", newsockfd, MAXFDS);
                    }

                    fd_status_t status =
                        on_peer_connected(newsockfd, &peer_addr, peer_addr_len);
                    //fd_status_t status = fd_status_R;

                    struct epoll_event event = {0};
                    event.data.fd = newsockfd;
                    if (status.want_read) {
                        event.events |= EPOLLIN;
                    }
                    if (status.want_write) {
                        event.events |= EPOLLOUT;
                    }

                    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newsockfd, &event) < 0) {
                        perror_die("epoll_ctl EPOLL_CTL_ADD");
                    }
                }
            } else {
                // A peer socket is ready.
                if (events[i].events & EPOLLIN) {
                    struct epoll_event  event       = {0};
                    int fd                          = events[i].data.fd;
                    event.data.fd = fd;
                    fd_status_t status = on_peer_ready_recv(conn, fd, session);

                    if (status.want_read) {
                        event.events |= EPOLLIN;
                    }

                    if (status.want_write) {
                        event.events |= EPOLLOUT;
                    }

                    if (event.events == 0) {
                        mysql_delete_fd(conn, fd);

                        if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL) < 0) {
                            perror_die("epoll_ctl EPOLL_CTL_DEL");
                        }
                        printf("socket %d closing\n", fd);
                        close(fd);
                    } else if (epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event) < 0) {
                        perror_die("epoll_ctl EPOLL_CTL_MOD");
                    }
                } else if (events[i].events & EPOLLOUT) {
                    // Ready for writing.
                    int fd = events[i].data.fd;
                    fd_status_t status = on_peer_ready_send(fd);
                    struct epoll_event event = {0};
                    event.data.fd = fd;

                    if (status.want_read) {
                        event.events |= EPOLLIN;
                    }
                    if (status.want_write) {
                        event.events |= EPOLLOUT;
                    }
                    if (event.events == 0) {
                        printf("socket %d closing\n", fd);
                        if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL) < 0) {
                            perror_die("epoll_ctl EPOLL_CTL_DEL");
                        }
                        close(fd);
                    } else if (epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event) < 0) {
                        perror_die("epoll_ctl EPOLL_CTL_MOD");
                    }
                }
            }
        }
        mj_free(session);
    }
    mysql_close(conn);
    mj_free(conn);
    return 0;
}
