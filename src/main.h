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
#include "../lib/mysql/mysql.h"

#define DEBUG(x, a...) printf("%s: " x "\n", __func__, ## a) 
#define BREAK(x, a...) printf("==## file [%s] func [%s] line [%d] " x "\n", __FILE__, __func__, __LINE__, ## a)

#define PORT                88
#define EPOLL_SIZE          60
#define EPOLL_EVENT_SIZE    100

#define AGENT_TYPE_LINUX_NVIDIA     "linux-claymore-nvidia"
#define AGENT_TYPE_UX               "ux"

/* session.c */
#define MAX_RECV_BUFF_SIZE 256

/* log.c */
#define HOST            "127.0.0.1"
#define USER            "root"
#define PASSWD          ""
#define DB_NAME         "monitoring"
#define DB_PORT         "3306"
#define QUERY_LEN       256


typedef struct _cmd_t {
    char*   cmd_type;
    int     sequence;
    char*   miner_mac;
    char*   path;
    char*   path_install;
}cmd_t;

typedef struct _agent_t {
    int     fd; 
    char*   miner_mac;
    char*   agent_type;
}agent_t;

typedef enum {
    REQ_INVALID         = 0,
    REQ_UX,
    REQ_LINUX_CLAYMORE,
    REQ_WINDOW_CLAYMORE,
}agent_type_t;

/* session.c */
char* socket_read(int fd, int *result);
char* msg_client_info(size_t *len, const cmd_t *req_cl);
int append_null(char *str, int len);

/* log.c */
MYSQL* mysql_conn(void); 
int mysql_insert_fd(MYSQL *conn, char *miner_mac, int fd);
int mysql_select_fd(MYSQL *conn, char *miner_mac);
int mysql_delete_fd(MYSQL *conn, char *miner_mac);
int mysql_update_log(MYSQL *conn, int sequence, char *miner_mac, int status);

/* parse.c */
json_object* parse_string_to_json(char* str);
int get_agent_type(json_object *json);
char* parse_json_to_string(json_object *json_obj);
cmd_t* parse_json_cmd(json_object *json_obj);
void parse_json_agent(json_object *json_obj, agent_t *agent_data);
int parse_cmd_type(const char *cmd_type);

/* util.c */
void die(char* fmt, ...);
void* xmalloc(size_t size);
void perror_die(char* msg);
void report_peer_connected(const struct sockaddr_in* sa, socklen_t salen);
int listen_inet_socket(int portnum);
void make_socket_non_blocking(int sockfd);

#endif
