#include "json_export.h"

char *json_from_product(Product product) {
    char *json = calloc(MAX_PRODUCT_JSON_LENGTH, sizeof(char));
    sprintf(json,
            "{ \"id\" : %d, \"name\" : \"%s\", \"price\" : %d, \"quantity\" : "
            "%d, \"in_stock\" : %d }",
            product.product_id, product.product_name, product.unit_price, product.quantity, product.active);
    return json;
}

char *json_from_products(Products products) {
    char *json = calloc(MAX_PRODUCT_JSON_LENGTH * products.count, sizeof(char));
    strcat(json, "{\"items\": [");
    for (int i = 0; i < products.count; i++) {
        char *product_json = json_from_product(products.products[i]);
        strcat(json, product_json);
        free(product_json);
        if (i < products.count - 1) strcat(json, ", ");
    }
    strcat(json, "]}");
    return json;
}

// {"id": int, "price": int, "quantity": int}
char *json_from_order_detail(OrderDetail order_detail) {
    char *json = calloc(MAX_PRODUCT_JSON_LENGTH, sizeof(char));
    sprintf(json,
            "{ \"id\" : %d, \"price\" : %d, \"quantity\" : "
            "%d }",
            order_detail.product_id, order_detail.unit_price, order_detail.quantity);
    return json;
}

// {"id": int, "date": datetime, "items": [{"id": int, "price": int, "quantity": int}]}
char *json_from_order(Order order) {
    char *json = calloc(MAX_ORDER_JSON_LENGTH, sizeof(char));
    sprintf(json, "{ \"id\" : %d, \"date\" : \"%s\", \" items \": [", order.order_id, order.order_date);
    for (int i = 0; i < order.count; i++) {
        char *order_detail_json = json_from_order_detail(order.order_details[i]);
        strcat(json, order_detail_json);
        free(order_detail_json);
        if (i < order.count - 1) strcat(json, ", ");
    }
    strcat(json, "]}");
    return json;
}

// {"orders": [{"id": int, "date": datetime, "items": [{"id": int, "price": int, "quantity": int}]}]}
char *json_from_orders(Orders orders) {
    char *json = calloc(MAX_PRODUCT_JSON_LENGTH * orders.count, sizeof(char));
    strcat(json, "{\"orders\": [");
    for (int i = 0; i < orders.count; i++) {
        char *order_json = json_from_order(orders.orders[i]);
        strcat(json, order_json);
        free(order_json);
        if (i < orders.count - 1) strcat(json, ", ");
    }
    strcat(json, "]}");
    return json;
}