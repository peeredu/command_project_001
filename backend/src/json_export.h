#ifndef _JSON_EXPORT_H_
#define _JSON_EXPORT_H_

#include "../../data/src/db_models.h"

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>


#define MAX_PRODUCT_JSON_LENGTH 200
#define MAX_ORDER_JSON_LENGTH 500

char *json_from_product(Product product);
char *json_from_products(Products products);

char *json_from_order_detail(OrderDetail order_detail);
char *json_from_order(Order order);
char *json_from_orders(Orders orders);

#endif  // _JSON_EXPORT_H_