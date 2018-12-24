#include "log.h"

#define HOST            "127.0.0.1"
#define USER            "root"
#define PASSWD          ""
#define DB_NAME         "monitoring"
#define DB_PORT         "3306"
#define QUERY_LEN       256

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

/** @brief  mysql_insert_fd
  * @param  conn        db connection 
  *         miner_mac   client(miner) mac addr
  *         fd          client(miner) socket fd
  * @return when successed return positive else nagative 
  */
int mysql_insert_fd(MYSQL *conn, char *miner_mac, int fd) {
    char query_insert[256] = {0, };
    char query_update[256] = {0, };

    sprintf(query_insert, 
            "insert into cmd_agent_session (mac, fd) values ('%s', %d)", 
            miner_mac, fd); 

    sprintf(query_update, 
            "update cmd_agent_session set fd = %d where mac = '%s'",
            fd, miner_mac);

    /*  insert */
    if (mysql_query(conn, query_insert) == 0) {
        return fd;
    /* update */
    } else if (mysql_query(conn, query_update) == 0) {
        return fd;
    } else {
        DEBUG("[Fail] mysql_query");
        return -2;
    }
    DEBUG("[Fail] mysql_select_fd");
    return -1;
}

int mysql_select_fd(MYSQL *conn, char *miner_mac) {
    MYSQL_RES   *ret;
    MYSQL_ROW   row;

    unsigned int    fd          = 0;

    char query[256] = {0, };

    sprintf(query, "select fd from cmd_agent_session where mac = '%s'", 
            miner_mac);

    if (mysql_query(conn, query) == 0) {
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

