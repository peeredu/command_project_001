#ifndef DB_MODELS_H
#define DB_MODELS_H

#define MAX_NAME_LENGTH 50
#define MAX_DATE_LENGTH 20


/* Структура для хранения строки из таблицы Products */
typedef struct Product {
    int product_id;
    char product_name[MAX_NAME_LENGTH];
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
    int order_detail_id;
    int order_id;
    int product_id;
    int unit_price;
    int quantity;
} OrderDetail;

/* Структура для хранения одного заказа */
typedef struct Order {
    int order_id;
    char order_date[MAX_DATE_LENGTH];
    OrderDetail *order_details;
    int count;
} Order;

/* Структура хранит массив заказов из таблицы Orders */
typedef struct Orders {
    Order *orders;
    int count;
} Orders;

#endif  // DB_MODELS