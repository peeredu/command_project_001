#ifndef DB_API_H
#define DB_API_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>

#include "db_models.h"

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASSWORD "root"
#define DB_NAME "shop"
#define DB_PORT 3306
#define DB_TIMEOUT 2  // unsigned int

#define DB_RETURN_OK 0
#define DB_RETURN_ERROR 1

int db_get_connect(MYSQL *conn);
int db_add_product(MYSQL *conn, Product product);
int db_remove_product(MYSQL *conn, const int id);
int db_get_product(MYSQL *conn, Product *product, const int id);
int db_count_products(MYSQL *conn, int *count);
int db_get_products(MYSQL *conn, Products *products);
int db_count_order_detalis(MYSQL *conn, int *count, const int id);
int db_get_order(MYSQL *conn, Order *order, const int id);

#endif  // DB_API