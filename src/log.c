#include "main.h"

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
int mysql_insert_fd(MYSQL *conn, char *miner_mac, int fd, agent_type_t agent_type) {
    char query_insert[256] = {0, };
    char query_update[256] = {0, };

    sprintf(query_insert, 
            "insert into cmd_agent_session (mac, fd, agent_type) values ('%s', '%d', '%d')", 
            miner_mac, fd, agent_type); 

    sprintf(query_update, 
            "update cmd_agent_session set fd = '%d', agent_type = '%d' where mac = '%s'",
            fd, agent_type, miner_mac);

    /*  insert */
    if (mysql_query(conn, query_insert) == 0) {
        return fd;
    } else if (mysql_query(conn, query_update) == 0) {
        /* update */
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
            if (row != NULL) {
                fd = atoi(row[0]);
                return fd;
            } else {
                return -5;
            }
        } else {
            return -4;
        }
    } else {
        return -3;
    }
    return -2;
}

int mysql_delete_mac(MYSQL *conn, char *miner_mac) {
    char query[256] = {0, };

    sprintf(query, 
            "delete from cmd_agent_session where mac='%s'", 
            miner_mac); 

    /*  delete */
    if (mysql_query(conn, query) == 0) {
        return 0;
    } else {
        DEBUG("[Fail] mysql_query");
        return -2;
    }
    DEBUG("[Fail] mysql_select_fd");
    return -1;
}

int mysql_delete_fd(MYSQL *conn, int fd) {
    char query[256] = {0, };

    sprintf(query, 
            "delete from cmd_agent_session where fd='%d'", 
            fd); 

    /*  delete */
    if (mysql_query(conn, query) == 0) {
        return 0;
    } else {
        DEBUG("[Fail] mysql_query");
        return -2;
    }
    DEBUG("[Fail] mysql_select_fd");
    return -1;
}




/** @brief mysql_update_log 
 * @param  conn        db connection 
 *         sequence
 *         status
 * @return when successed return positive else nagative 
 */
int mysql_update_log(MYSQL *conn, int sequence, char *miner_mac, int status) {
    char query_update[256] = {0, };

    sprintf(query_update, 
            "update cmd_agent_session set status = %d \
            where sequence = '%d' ANDi mac = '%s'",
            status, sequence, miner_mac);

    /* update */
    if (mysql_query(conn, query_update) == 0) {
        return 0;
    } else {
        DEBUG("[Fail] mysql_query");
        return -2;
    }
    DEBUG("[Fail] mysql_update_log");
    return -1;
}

/** @brief  mysql_select_session
 *         FD 로 해당 세션이 UX 인지 마이너인지 조회
 * @return 성공 : zeor or agent_type
 *         실패 : negative number
 */
int mysql_select_session(MYSQL *conn, int fd) {
    MYSQL_RES       *ret;
    MYSQL_ROW       row;
    agent_type_t    agent_type = 0;

    char query[256] = {0, };

    sprintf(query, "select agent_type from cmd_agent_session where fd = '%d'",
            fd);

    if (mysql_query(conn, query) == 0) {
        ret = mysql_store_result(conn);
        if (ret != NULL) {
            row = mysql_fetch_row(ret);
            if (row != NULL) {
                agent_type = atoi(row[0]);
                return agent_type;
            } else {
                DEBUG("Fail : mysql_fetch_row");
                return -5;
            }
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

