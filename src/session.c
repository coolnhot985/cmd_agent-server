#include "main.h"

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
        len = recv(fd, buff, MAX_RECV_BUFF_SIZE, MSG_DONTWAIT);

        if (len == 0) {
            /* return fd_status_NORW */
            *ret = -1;
            mj_free(recv_data);
            return NULL;
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

char* msg_client_info(size_t *len, const session_t *session) {
    const char  *str        = NULL;
    char        *buff       = NULL;
    int         cmd_type    = 0;

    FILE        *fp         = NULL;
    char        *file_buff  = NULL;
    long        file_size   = 0; 

    json_object *json   = json_object_new_object();
    json_object *cmd    = json_object_new_string(session->cmd_type);
    json_object *path   = json_object_new_string(session->path);
    json_object *sequence = json_object_new_int(session->sequence);

    json_object_object_add(json, "commend_type", cmd);
    json_object_object_add(json, "path", path);
    json_object_object_add(json, "sequence", sequence);

    cmd_type = parse_cmd_type(session->cmd_type);

    switch (cmd_type) {
        case 1:
            if (((fp = fopen(session->path, "r")) != NULL)) {
                size_t ret = 0;
                fp = fopen(session->path, "r");

                /* 파일포인터를 파일끝으로 이동시켜 길이 계산 */
                fseek(fp, 0, SEEK_END);
                file_size = ftell(fp);

                /* 계산한 파일 길이만큼 메모리 할당 */
                file_buff = (char*)malloc((size_t)file_size + 1);
                memset(file_buff, 0x0, file_size + 1);

                /* 파일을 읽어오기 위해 파일포인터를 처음으로 이동 */

                fseek(fp, 0, SEEK_SET);
                ret = fread(file_buff, sizeof(char), (size_t)file_size, fp);
                if (ret < 1) {
                    DEBUG("not found fils");
                }

                file_buff[file_size + 1] = '\0';
                break;
            }
    }
    
    json_object *content = json_object_new_string(file_buff);
    json_object_object_add(json, "content", content);

    json_object *path_install = json_object_new_string(session->path_install);
    json_object_object_add(json, "path_install", path_install);

    str = json_object_get_string(json);

    *len = strlen(str);
    buff = strndup(str, *len);
    
    mj_free(file_buff);
    fclose(fp);
    return buff;
}

int append_null(char *str, int len) {
    len++;
    str = realloc(str, len);

    *(str + len) = '\0';

    return len;
}

