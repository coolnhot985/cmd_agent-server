#include "main.h"
#include "json.h"
#include "json_tokener.h"

int do_use_fd (int efd, struct epoll_event ev) {
    int         cfd                 = ev.data.fd;
    int         sendflags           = 0; 
    int         sizen               = 0; 
    int         readn               = 0; 

    char        *recv_data                  = NULL;
    char        buff[MAX_RECV_BUFF_SIZE]    = {'\0'};
    char        readbuf[1024]       = { '\0' }; 

    json_object     *recv_data_json;
    agent_type_t    agent_type;

    while (1) {
        recv_data = socket_read(cfd);                       /* 데이터 수신*/
        recv_data_json = parse_string_to_json(recv_data);   /* 문자열 데이터를 JSON 포멧으로 변환 */
        agent_type = get_agent_type(recv_data_json); 

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

    char    buff[MAX_RECV_BUFF_SIZE] = {'\0'};
    char    *recv_data = NULL;
    
    recv_data = (char*)malloc(MAX_RECV_BUFF_SIZE);

    while (len == 0) {
        len += read(fd, buff, MAX_RECV_BUFF_SIZE);
        if (len < 0) {
            return recv_data;
        }
        realloc(recv_data, len);
        memcpy(recv_data, buff, len);
    }

    return recv_data;
}

