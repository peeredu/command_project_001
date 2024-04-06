#include "../json_export.h"

int main(void) {
  Product product = {
      .product_id = 1, .product_name = "test", .unit_price = 100, .quantity = 10, .active = 1};

  char *json = calloc(MAX_PRODUCT_JSON_LENGTH, sizeof(char));
  json = json_from_product(product);
  printf("%s\n", json);
  free(json);

  Product product2 = {
      .product_id = 2, .product_name = "test2", .unit_price = 124, .quantity = 3, .active = 1};
  Product product3 = {
      .product_id = 3, .product_name = "test3", .unit_price = 223, .quantity = 2, .active = 1};

  Products products;

  products.count =
      3;  // TODO: Временно
  products.products = calloc(products.count,
                             sizeof(Product));  // TODO: Временно
  products.products[0] = product;
  products.products[1] = product2;
  products.products[2] = product3;

  char *json2 = calloc(MAX_PRODUCT_JSON_LENGTH * products.count, sizeof(char));
  json2 = json_from_products(products);
  printf("%s\n", json2);
  free(json2);
  free(products.products);  // Временно

  return 0;
}