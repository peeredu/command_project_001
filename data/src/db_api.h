#ifndef DB_API_H
#define DB_API_H

#include <mysql/mysql.h>
#include <stdio.h>

#define DB_HOST "172.0.0.1"
#define DB_USER "root"
#define DB_PASSWORD "root"
#define DB_NAME "shop"
#define DB_PORT 0
#define DB_TIMEOUT 2  // unsigned int

int db_get_connect(MYSQL *conn);

#endif // DB_API