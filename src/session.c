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

char* msg_client_info(size_t *len, const cmd_t *req_cl) {
    const char  *str    = NULL;
    char        *buff   = NULL;

    json_object *json   = json_object_new_object();
    json_object *cmd    = json_object_new_string(req_cl->cmd_type);
    json_object *path   = json_object_new_string(req_cl->path);
    json_object *sequence = json_object_new_int(req_cl->sequence);

    json_object_object_add(json, "commend_type", cmd);
    json_object_object_add(json, "path", path);
    json_object_object_add(json, "sequence", sequence);

    str = json_object_get_string(json);

    *len = strlen(str);
    buff = strndup(str, len);

    return buff;
}

int append_null(char *str, int len) {
    len++;
    str = realloc(str, len);

    *(str + len) = '\0';

    return len;
}

