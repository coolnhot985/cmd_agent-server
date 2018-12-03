#include "session.h"

int do_use_fd (int efd, struct epoll_event ev) {
    int         cfd                 = ev.data.fd;

    char        *recv_data                  = NULL;
//  char        *recv_cmd                   = NULL;
//  char        buff[MAX_RECV_BUFF_SIZE]    = {'\0'};

    json_object     *recv_data_json;
    agent_type_t    agent_type;

    recv_data = socket_read(cfd);                       /* 데이터 수신*/
    recv_data_json = parse_string_to_json(recv_data);   /* 문자열 데이터를 JSON 포멧으로 변환 */
    agent_type = get_agent_type(recv_data_json); 
    free(recv_data);

    switch(agent_type) {
        case (REQ_INVALID):
            break;
        case (REQ_UX):
            break;
        case (REQ_LINUX_CLAYMORE):
            send(cfd, "SYN_ACK", strlen("SYN_ACK"), MSG_DONTWAIT);
            BREAK("wait commend .. [%d]", cfd);
            recv_data = socket_read(cfd);
        case (REQ_WINDOW_CLAYMORE):
            break;
    }

    do_del_fd(efd,cfd); 
    close (cfd); 
    return -1;
}


#if 0
    while(1) {
        DEBUG("");
        readn = read (cfd, readbuf, 255); 
        if (readn <0 ) { 
            if (EAGAIN == errno ) { 
                break; 
            } 
            do_del_fd(efd, cfd); 
            close (cfd); 
            printf ("Close fd %d by %d\n", cfd,readn); 
            perror("Closed"); 
            return -1; 
        } 
        sizen += readn; 
        if (sizen >= 1024) { 
            do_del_fd(efd, cfd); 
            close (cfd); 
            printf ("Close buffer full fd %d by %d\n", cfd,readn); 
            perror("Closed"); 
            return -1; 
        }
        DEBUG("");
        memcpy(buf_in,readbuf,readn);
        break;
    } 

#ifdef USE_ETONESHOT 
    // re-set fd to epoll 
    do_modify_fd(efd, cfd); 
#endif // USE_ETONESHOT 

#endif

#if 0
    //memset (buf_in, 0x00, 256); 
    readn = read (cfd, buf_in, 255); 
    // if it occured ploblems with reading, delete from epoll event pool and close socket 
    if (readn <= 0) {
        do_del_fd(efd,cfd); 
        close (cfd); 
        printf ("Close fd %d", cfd); 
    } else { 
        printf ("read data %s", buf_in); 
        send (cfd, buf_in, strlen (buf_in), sendflags); 
    }
    return 1; 
} 
#endif

char* socket_read(int fd) {
    int     len = 0;
    int     recv_len = 0;
    size_t  offset = 0;

    char    buff[MAX_RECV_BUFF_SIZE];
    char    *recv_data = NULL;
    
    recv_data = (char*)malloc(MAX_RECV_BUFF_SIZE);

    while (len >= 0) {
        memset(buff, 0x00, MAX_RECV_BUFF_SIZE);
        len = read(fd, buff, MAX_RECV_BUFF_SIZE);
        if (len <= 0) {
            DEBUG("Fail");
            return recv_data;
        }

        recv_len += len;
        recv_data = realloc(recv_data, recv_len);

        memcpy(recv_data + offset, buff, len);
        offset += len;
        if (recv_data[recv_len - 1] == '\0') {
            return recv_data;
        }
    }

    BREAK("recv_data [%s]", recv_data);

    return recv_data;
}

