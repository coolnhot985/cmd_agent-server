#include "log.h"

#define HOST            "127.0.0.1"
#define USER            "root"
#define PASSWD          ""
#define DB_NAME         "monitoring"
#define DB_PORT         "3306"
#define QUERY_LEN       256

#if 0
MYSQL* mysql_conn(MYSQL *conn){ 
    MYSQL mysql;
    //MYSQL *conn = NULL;
    MYSQL *ret  = NULL;
//    conn = (MYSQL*)malloc(sizeof(MYSQL));
    mysql_init(conn);
    ret = mysql_real_connect(conn, HOST, USER, PASSWD, DB_NAME, 
            3306, (char*)NULL, 1);
    if (ret == NULL) {
        DEBUG("Fail : conn databases");
        return NULL;
    } 

    return conn; 
}
#endif

MYSQL* mysql_conn(void) {
    MYSQL *conn = NULL;
    conn = (MYSQL*)malloc(sizeof(MYSQL));

    mysql_init(conn);

    if (!mysql_real_connect(conn, HOST, USER, PASSWD, DB_NAME, 3306, NULL, 0)) {
        /* ERR */
        DEBUG("Fail : mysql_real_connectr [%s]\n", mysql_error(conn));
    }

    return conn; 
}

int mysql_select_fd(MYSQL *conn, char *miner_mac) {
    MYSQL_RES   *ret;
    MYSQL_ROW   row;

    unsigned int    fd          = 0;
    int             result      = 0;

    char query[256] = {0, };

    sprintf(query, "select fd from cmd_agent_session where mac = '%s'", 
            miner_mac);

    result = mysql_query(conn, query);
    if (result == 0) {
        ret = mysql_store_result(conn);
        if (ret != NULL) {
            row = mysql_fetch_row(ret);
            if (row == NULL) {
                DEBUG("Fail : mysql_fetch_row");
            }
            /* succese mysql_select_fd */
            fd = atoi(row[0]);
            return fd;
        } else {
            DEBUG("Fail : mysql_store_result");
            return -4;
        }
    } else {
        DEBUG("Fail : mysql_query");
        return -3;
    }
    DEBUG("Fail : mysql_select_fd");
    return -2;
}


