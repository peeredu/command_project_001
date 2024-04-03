#ifndef DB_MODELS_H
#define DB_MODELS_H

#include "db_api.h"

#define MAX_NAME_LENGTH 50

/* Структура для хранения строки из таблицы Products */
typedef struct Product {
    int id;
    char name[MAX_NAME_LENGTH];
    int unit_price;
    int quantity;
    int active;
} Product;

/* Структура хранит массив строк из таблицы Products */
typedef struct Products {
    Product *products;
    int count;
} Products;

#endif  // DB_MODELS