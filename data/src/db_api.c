#include "db_api.h"

/* Функция установления соединения с БД. Возвращает 0 если соединение установлено */
int db_get_connect(MYSQL *conn) {
    // Получаем дескриптор соединения
    if (!mysql_init(conn)) {
        fprintf(stderr, "Error: can't create MySQL-descriptor\n");
        return DB_RETURN_ERROR;
    } else {
        unsigned int db_timeout = DB_TIMEOUT;
        // bool db_report_truncation = 0;
        mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, (int *)&db_timeout);
        // mysql_options(conn, MYSQL_REPORT_DATA_TRUNCATION, (bool *)&db_report_truncation);
        // Устанавливаем соединение с базой данных
        if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0)) {
            // Если соединение не установлено выводим сообщение об ошибке
            fprintf(stderr, "Error: %s\n", mysql_error(conn));
            return DB_RETURN_ERROR;
        }
    }
    return DB_RETURN_OK;
}

/* Функция добавления нового продукта */
int db_add_product(MYSQL *const conn, Product product) {
    enum select_params { par_name, par_price, par_quantity, par_active, par_count };
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[par_count] = {0};
    stmt = mysql_stmt_init(conn);
    if (NULL == stmt) return DB_RETURN_ERROR;
    char *command = "INSERT INTO Products (ProductName, UnitPrice, Quantity, Active) VALUES (?, ?, ?, ?)";
    if (mysql_stmt_prepare(stmt, command, strlen(command))) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    bind[par_name].buffer_type = MYSQL_TYPE_STRING;
    bind[par_name].buffer = (char *)product.name;
    bind[par_name].buffer_length = strlen(product.name);
    bind[par_price].buffer_type = MYSQL_TYPE_LONG;
    bind[par_price].buffer = (char *)&product.unit_price;
    bind[par_quantity].buffer_type = MYSQL_TYPE_LONG;
    bind[par_quantity].buffer = (char *)&product.quantity;
    bind[par_active].buffer_type = MYSQL_TYPE_LONG;
    bind[par_active].buffer = (char *)&product.active;
    if (mysql_stmt_bind_param(stmt, bind)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    if (mysql_stmt_execute(stmt)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    if (mysql_stmt_close(stmt)) return DB_RETURN_ERROR;
    return DB_RETURN_OK;
}

/* Функция удаления продукта по id */
int db_remove_product(MYSQL *conn, const int id) {
    enum select_params { par_id, par_count };
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[par_count] = {0};

    stmt = mysql_stmt_init(conn);
    if (NULL == stmt) return DB_RETURN_ERROR;
    char *command = "UPDATE Products SET Active=0 WHERE ProductId=?";
    if (mysql_stmt_prepare(stmt, command, strlen(command))) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    bind[par_id].buffer_type = MYSQL_TYPE_LONG;
    bind[par_id].buffer = (char *)&id;
    if (mysql_stmt_bind_param(stmt, bind)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    if (mysql_stmt_execute(stmt)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    if (mysql_stmt_close(stmt)) return DB_RETURN_ERROR;
    return DB_RETURN_OK;
}

/* Функция получения одного товара по id */
int db_get_product(MYSQL *conn, Product *product, const int id) {
    enum select_params { par_id, par_name, par_price, par_quantity, par_active, par_count };
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[par_count] = {0};
    MYSQL_RES *prepare_meta_result;
    unsigned long length[par_count];
    bool is_null[par_count];
    bool error[par_count];
    stmt = mysql_stmt_init(conn);
    if (!stmt) return DB_RETURN_ERROR;
    char *command = "SELECT * FROM Products WHERE ProductId=?";
    if (mysql_stmt_prepare(stmt, command, strlen(command))) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }

    bind[par_id].buffer_type = MYSQL_TYPE_LONG;
    bind[par_id].buffer = (char *)&id;

    if (mysql_stmt_bind_param(stmt, bind)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    if (mysql_stmt_param_count(stmt) != 1) {
        fprintf(stderr, " invalid parameter count returned by MySQL\n");
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    if (mysql_stmt_execute(stmt)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    prepare_meta_result = mysql_stmt_result_metadata(stmt);
    if (!prepare_meta_result) {
        fprintf(stderr,
                " mysql_stmt_result_metadata(), \
           returned no meta information\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    if (mysql_num_fields(prepare_meta_result) != 5) {
        fprintf(stderr, " invalid column count returned by MySQL\n");
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }

    memset(bind, 0, sizeof(bind));
    bind[par_id].buffer_type = MYSQL_TYPE_LONG;
    bind[par_id].buffer = (char *)&product->id;
    bind[par_id].is_null = &is_null[par_id];
    bind[par_id].length = &length[par_id];
    bind[par_id].error = &error[par_id];

    bind[par_name].buffer_type = MYSQL_TYPE_STRING;
    bind[par_name].buffer = (char *)&product->name;
    bind[par_name].buffer_length = MAX_NAME_LENGTH;
    bind[par_name].is_null = &is_null[par_name];
    bind[par_name].length = &length[par_name];
    bind[par_name].error = &error[par_name];

    bind[par_price].buffer_type = MYSQL_TYPE_LONG;
    bind[par_price].buffer = (char *)&product->unit_price;
    bind[par_price].is_null = &is_null[par_price];
    bind[par_price].length = &length[par_price];
    bind[par_price].error = &error[par_price];

    bind[par_quantity].buffer_type = MYSQL_TYPE_LONG;
    bind[par_quantity].buffer = (char *)&product->quantity;
    bind[par_quantity].is_null = &is_null[par_quantity];
    bind[par_quantity].length = &length[par_quantity];
    bind[par_quantity].error = &error[par_quantity];

    bind[par_active].buffer_type = MYSQL_TYPE_LONG;
    bind[par_active].buffer = (char *)&product->active;
    bind[par_active].is_null = &is_null[par_active];
    bind[par_active].length = &length[par_active];
    bind[par_active].error = &error[par_active];

    if (mysql_stmt_bind_result(stmt, bind)) {
        fprintf(stderr, " mysql_stmt_bind_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }

    int get_product = 0;
    if (!mysql_stmt_fetch(stmt)) {
        get_product = 1;
    }
    mysql_free_result(prepare_meta_result);
    if (mysql_stmt_close(stmt)) return DB_RETURN_ERROR;

    return get_product ? DB_RETURN_OK : DB_RETURN_ERROR;
}

/* Функция возвращает количество товаров */
int db_count_products(MYSQL *conn, int *count) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    if (mysql_query(conn, "SELECT COUNT(*) FROM Products") != 0) {
        fprintf(stderr, "Error: can't execute SQL-query\n");
        return DB_RETURN_ERROR;
    }
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "Error: can't get the result description\n");
        return DB_RETURN_ERROR;
    }
    mysql_num_rows(res);
    row = mysql_fetch_row(res);
    sscanf(row[0], "%d", count);
    mysql_free_result(res);

    return DB_RETURN_OK;
}

/* Функция возвращает массив всех товаров */
int db_get_products(MYSQL *conn, Products *arr_products) {
    int count_products = -1;
    if (db_count_products(conn, &count_products)) {
        fprintf(stderr, "Error: can't get count products\n");
        return DB_RETURN_ERROR;
    }
    printf("db_get_products count_products=%d\n", count_products);
    arr_products->count = count_products;
    arr_products->products = malloc(arr_products->count * sizeof(Product));

    enum select_params { par_id, par_name, par_price, par_quantity, par_active, par_count };
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[par_count] = {0};
    MYSQL_RES *prepare_meta_result;
    unsigned long length[par_count];
    bool is_null[par_count];
    bool error[par_count];

    stmt = mysql_stmt_init(conn);
    if (!stmt) return DB_RETURN_ERROR;
    char *command = "SELECT * FROM Products";
    if (mysql_stmt_prepare(stmt, command, strlen(command))) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    if (mysql_stmt_param_count(stmt) != 0) {
        fprintf(stderr, " invalid parameter count returned by MySQL\n");
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    if (mysql_stmt_execute(stmt)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    prepare_meta_result = mysql_stmt_result_metadata(stmt);
    if (!prepare_meta_result) {
        fprintf(stderr,
                " mysql_stmt_result_metadata(), \
           returned no meta information\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    /* Проверяет количество колонок в запросе */
    if (mysql_num_fields(prepare_meta_result) != 5) {
        fprintf(stderr, " invalid column count returned by MySQL\n");
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }

    for (int i = 0; i < arr_products->count; i++) {
        memset(bind, 0, sizeof(bind));

        /* Колонка ProductId в таблице Products */
        bind[par_id].buffer_type = MYSQL_TYPE_LONG;
        bind[par_id].buffer = (char *)&arr_products->products[i].id;
        bind[par_id].is_null = &is_null[par_id];
        bind[par_id].length = &length[par_id];
        bind[par_id].error = &error[par_id];

        /* Колонка ProductName в таблице Products */
        bind[par_name].buffer_type = MYSQL_TYPE_STRING;
        bind[par_name].buffer = (char *)&arr_products->products[i].name;
        bind[par_name].buffer_length = MAX_NAME_LENGTH;
        bind[par_name].is_null = &is_null[par_name];
        bind[par_name].length = &length[par_name];
        bind[par_name].error = &error[par_name];

        /* Колонка UnitPrice в таблице Products */
        bind[par_price].buffer_type = MYSQL_TYPE_LONG;
        bind[par_price].buffer = (char *)&arr_products->products[i].unit_price;
        bind[par_price].is_null = &is_null[par_price];
        bind[par_price].length = &length[par_price];
        bind[par_price].error = &error[par_price];

        /* Колонка Quantity в таблице Products */
        bind[par_quantity].buffer_type = MYSQL_TYPE_LONG;
        bind[par_quantity].buffer = (char *)&arr_products->products[i].quantity;
        bind[par_quantity].is_null = &is_null[par_quantity];
        bind[par_quantity].length = &length[par_quantity];
        bind[par_quantity].error = &error[par_quantity];

        /* Колонка Active в таблице Products */
        bind[par_active].buffer_type = MYSQL_TYPE_LONG;
        bind[par_active].buffer = (char *)&arr_products->products[i].active;
        bind[par_active].is_null = &is_null[par_active];
        bind[par_active].length = &length[par_active];
        bind[par_active].error = &error[par_active];

        if (mysql_stmt_bind_result(stmt, bind)) {
            fprintf(stderr, " mysql_stmt_bind_result() failed\n");
            fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
            mysql_stmt_close(stmt);
            return DB_RETURN_ERROR;
        }

        mysql_stmt_fetch(stmt);
    }

    mysql_free_result(prepare_meta_result);
    if (mysql_stmt_close(stmt)) return DB_RETURN_ERROR;

    return DB_RETURN_OK;
}
