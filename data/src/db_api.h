#ifndef DB_API_H
#define DB_API_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>

#define DB_HOST "172.17.0.1"
#define DB_USER "root"
#define DB_PASSWORD "root"
#define DB_NAME "shop"
#define DB_PORT 0
#define DB_TIMEOUT 2  // unsigned int
#define MAX_NAME_LENGTH 50

typedef struct Product {
    int id;
    char name[MAX_NAME_LENGTH];
    int unit_price;
    int quantity;
    int active;
} Product;

int db_get_connect(MYSQL *const conn);
int db_add_product( MYSQL *const conn, Product product);

// char *cat_string(char *dest, char *append);

#endif // DB_API