#include "session.h"

char* socket_read(int fd, int *ret) {
    int     len = 0;
    int     recv_len = 0;
    size_t  offset = 0;
    
    char    *recv_data = NULL;
    char    buff[MAX_RECV_BUFF_SIZE];
    
    recv_data = (char*)malloc(MAX_RECV_BUFF_SIZE);
    memset(recv_data, 0x0, MAX_RECV_BUFF_SIZE);

    while (len >= 0) {
        memset(buff, 0x00, MAX_RECV_BUFF_SIZE);
        //len = read(fd, buff, MAX_RECV_BUFF_SIZE);
        len = recv(fd, buff, MAX_RECV_BUFF_SIZE, MSG_DONTWAIT);
        if (len == 0) {
            /* return fd_status_NORW */
            *ret = -1;
            return recv_data;
        } else if (len < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                *ret = -2;
                /* return fd_status_R */
                return recv_data;
            } else {
                *ret = -3;
                /* return recv */
                return recv_data;
            }
        }

        recv_len += len;
        recv_data = realloc(recv_data, recv_len);

        memcpy(recv_data + offset, buff, len);
        offset += len;
        if (recv_data[recv_len - 1] == '\0' || recv_data[recv_len] == '\0') {
            *ret = 0;
            return recv_data;
        }
    }
    *ret = 0;
    return recv_data;
}


#if 0
int socket_read(int fd, char* recv_data) {
    int     len = 0;
    int     recv_len = 0;
    size_t  offset = 0;

    char    buff[MAX_RECV_BUFF_SIZE];
    
    recv_data = (char*)malloc(MAX_RECV_BUFF_SIZE);
    memset(recv_data, 0x0, MAX_RECV_BUFF_SIZE);

    while (len >= 0) {
        memset(buff, 0x00, MAX_RECV_BUFF_SIZE);
        //len = read(fd, buff, MAX_RECV_BUFF_SIZE);
        len = recv(fd, buff, MAX_RECV_BUFF_SIZE, MSG_DONTWAIT);
        if (len <= 0) {
            DEBUG("Fail .. ERR [%d]", len);
            return -1;
        }

        recv_len += len;
        recv_data = realloc(recv_data, recv_len);

        memcpy(recv_data + offset, buff, len);
        offset += len;

        if (recv_data[recv_len - 1] == '\0' || recv_data[recv_len] == '\0') {
            return -2;
        }
    }

    return 1;
}
#endif
