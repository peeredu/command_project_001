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

/* Структура для храниения одной строки из таблицы OrderDetails */
typedef struct OrderDetail {
    int OrderDetailId;
    int OrderId;
    int ProductId;
    int UnitPrice;
    int Quantity;
} OrderDetail;

/* Структура для хранения одного заказа */
typedef struct Order {
    int OrderId;
    char *OrderDate;
    OrderDetail *order_details;
    int count;
} Order;

/* Структура хранит массив заказов из таблицы Orders */
typedef struct Orders {
    Order *orders;
    int count;
} Orders;

#endif  // DB_MODELS