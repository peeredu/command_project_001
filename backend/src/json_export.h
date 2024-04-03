#ifndef _JSON_EXPORT_H_
#define _JSON_EXPORT_H_

#include "../../data/src/db_models.h"

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>


#define MAX_PRODUCT_JSON_LENGTH 200

char *json_from_product(Product product);
char *json_from_products(Products products);

#endif  // _JSON_EXPORT_H_