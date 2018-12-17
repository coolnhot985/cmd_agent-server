#ifndef _LOG_H_
#define _LOG_H_

#include "main.h"
#include "../lib/mysql/mysql.h"

MYSQL* mysql_conn(void); 
int mysql_select_fd(MYSQL *conn, char *miner_mac);


#endif
