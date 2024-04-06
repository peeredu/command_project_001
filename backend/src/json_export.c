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
    strcat(json, "{\" items \": [");
    for (int i = 0; i < products.count; i++) {
        char *product_json = json_from_product(products.products[i]);
        strcat(json, product_json);
        free(product_json);
        if (i < products.count - 1) strcat(json, ", ");
    }
    strcat(json, "]}");
    return json;
}
