#include "db_api.h"

#include "../../backend/src/common/logger.h"

/* Функция установления соединения с БД. Возвращает 0 если соединение установлено */
int db_get_connect(MYSQL *conn) {
    char mysql_root_host[MAX_NAME_LENGTH];
    char mysql_user[MAX_NAME_LENGTH];
    char mysql_password[MAX_NAME_LENGTH];
    char mysql_database[MAX_NAME_LENGTH];
    int mysql_port = 0;

    const char *env_mysql_root_host = getenv("MYSQL_ROOT_HOST");
    if (env_mysql_root_host != NULL)
        strncpy(mysql_root_host, env_mysql_root_host, MAX_NAME_LENGTH - 1);
    else
        strcpy(mysql_root_host, DB_DEFAULT_ROOT_HOST);

    const char *env_mysql_user = getenv("MYSQL_USER");
    if (env_mysql_user != NULL)
        strncpy(mysql_user, env_mysql_user, MAX_NAME_LENGTH - 1);
    else
        strcpy(mysql_user, DB_DEFAULT_USER);

    const char *env_mysql_password = getenv("MYSQL_PASSWORD");
    if (env_mysql_password != NULL)
        strncpy(mysql_password, env_mysql_password, MAX_NAME_LENGTH - 1);
    else
        strcpy(mysql_password, DB_DEFAULT_PASSWORD);

    const char *env_mysql_database = getenv("MYSQL_DATABASE");
    if (env_mysql_database != NULL)
        strncpy(mysql_database, env_mysql_database, MAX_NAME_LENGTH - 1);
    else
        strcpy(mysql_database, DB_DEFAULT_DATABASE);

    char *env_mysql_port = getenv("MYSQL_PORT");
    if (env_mysql_port != NULL) mysql_port = atoi(env_mysql_port);
    mysql_port = (mysql_port > 1000) ? mysql_port : DB_DEFAULT_PORT;

    // Получаем дескриптор соединения
    if (!mysql_init(conn)) {
        db_log_message(DB_LOG_MSG_ERROR, "Can't create MySQL-descriptor");
        fprintf(stderr, "Error: can't create MySQL-descriptor\n");
        return DB_RETURN_ERROR;
    } else {
        unsigned int db_timeout = DB_TIMEOUT;
        // bool db_report_truncation = 0;
        mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, (int *)&db_timeout);
        // mysql_options(conn, MYSQL_REPORT_DATA_TRUNCATION, (bool *)&db_report_truncation);
        // Устанавливаем соединение с базой данных
        if (!mysql_real_connect(conn, mysql_root_host, mysql_user, mysql_password, mysql_database, mysql_port,
                                NULL, 0)) {
            // Если соединение не установлено выводим сообщение об ошибке
            db_log_message(DB_LOG_MSG_ERROR, mysql_error(conn));
            fprintf(stderr, "Error: %s\n", mysql_error(conn));
            return DB_RETURN_ERROR;
        }
        // Устанавливаем кодировку соединения. Можно использовать cp1251 чтобы предотвратить искажения
        // русского текста
        if (mysql_query(conn, "SET NAMES 'UTF8'") != 0) {
            // Если кодировку установить невозможно - выводим сообщение об ошибке
            db_log_message(DB_LOG_MSG_ERROR, "Can't set character set");
            fprintf(stderr, "Error: can't set character set\n");
            return DB_RETURN_ERROR;
        }
    }
    db_log_message(DB_LOG_MSG_INFO, "Connect OK");

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
    bind[par_name].buffer = (char *)product.product_name;
    bind[par_name].buffer_length = strlen(product.product_name);
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
    db_log_message(DB_LOG_MSG_INFO, "Add product OK");

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
    db_log_message(DB_LOG_MSG_INFO, "Remove product OK");

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
    /* Подготавливает оператор SQL */
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
    bind[par_id].buffer = (char *)&product->product_id;
    bind[par_id].is_null = &is_null[par_id];
    bind[par_id].length = &length[par_id];
    bind[par_id].error = &error[par_id];

    bind[par_name].buffer_type = MYSQL_TYPE_STRING;
    bind[par_name].buffer = (char *)&product->product_name;
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
    db_log_message(DB_LOG_MSG_INFO, "Get product OK");

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
    /* Проверяет количество параметров из оператора */
    if (mysql_stmt_param_count(stmt) != 0) {
        fprintf(stderr, " invalid parameter count returned by MySQL\n");
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    /* Выполняем запрос SELECT */
    if (mysql_stmt_execute(stmt)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    /* Извлекаем метаинформацию набора результатов */
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
        bind[par_id].buffer = (char *)&arr_products->products[i].product_id;
        bind[par_id].is_null = &is_null[par_id];
        bind[par_id].length = &length[par_id];
        bind[par_id].error = &error[par_id];
        /* Колонка ProductName в таблице Products */
        bind[par_name].buffer_type = MYSQL_TYPE_STRING;
        bind[par_name].buffer = (char *)&arr_products->products[i].product_name;
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
    db_log_message(DB_LOG_MSG_INFO, "Get products OK");

    return DB_RETURN_OK;
}

/* Функция заполняет количество товаров в заказе по id */
int db_count_order_detalis(MYSQL *conn, int *count, const int id) {
    enum select_params { par_id, par_count };
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[par_count] = {0};
    MYSQL_RES *prepare_meta_result;
    stmt = mysql_stmt_init(conn);
    if (!stmt) return DB_RETURN_ERROR;
    char *command = "SELECT COUNT(*) FROM OrderDetails WHERE OrderId=?";
    if (mysql_stmt_prepare(stmt, command, strlen(command))) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }

    bind[par_id].buffer_type = MYSQL_TYPE_LONG;
    bind[par_id].buffer = (char *)&id;
    /* Используется для привязки входных данных для маркеров параметров в операторе SQL */
    if (mysql_stmt_bind_param(stmt, bind)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    /* Выполняет подготовленный запрос, связанный с обработчиком инструкций */
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
    if (mysql_num_fields(prepare_meta_result) != 1) {
        fprintf(stderr, " invalid column count returned by MySQL\n");
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }

    memset(bind, 0, sizeof(bind));
    bind[par_id].buffer_type = MYSQL_TYPE_LONG;
    bind[par_id].buffer = (char *)count;

    if (mysql_stmt_bind_result(stmt, bind)) {
        fprintf(stderr, " mysql_stmt_bind_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    mysql_stmt_fetch(stmt);
    mysql_free_result(prepare_meta_result);
    if (mysql_stmt_close(stmt)) return DB_RETURN_ERROR;

    return DB_RETURN_OK;
}

/* Функция заполняет информацию о заказе по id */
int db_get_order(MYSQL *conn, Order *order, const int id) {
    int count_order = -1;
    if (db_count_order_detalis(conn, &count_order, id)) {
        fprintf(stderr, "Error: can't get count order\n");
        return DB_RETURN_ERROR;
    }
    order->count = count_order;
    order->order_details = malloc(order->count * sizeof(OrderDetail));
    enum select_params {
        par_id_1,
        par_date,
        par_detail_id,
        par_id_2,
        par_prod_id,
        par_price,
        par_quantity,
        par_count
    };
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[par_count] = {0};
    MYSQL_RES *prepare_meta_result;
    MYSQL_TIME ts;
    stmt = mysql_stmt_init(conn);
    if (!stmt) return DB_RETURN_ERROR;
    char *command =
        "SELECT * \
        FROM Orders O CROSS JOIN OrderDetails OD \
        WHERE O.OrderId=OD.OrderId AND O.OrderId=?";
    /* Подготавливает оператор SQL */
    if (mysql_stmt_prepare(stmt, command, strlen(command))) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    bind[par_id_1].buffer_type = MYSQL_TYPE_LONG;
    bind[par_id_1].buffer = (char *)&id;
    /* Используется для привязки входных данных для маркеров параметров в операторе SQL */
    if (mysql_stmt_bind_param(stmt, bind)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    /* Проверяет количество параметров из оператора */
    if (mysql_stmt_param_count(stmt) != 1) {
        fprintf(stderr, " invalid parameter count returned by MySQL\n");
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    /* Выполняем запрос SELECT */
    if (mysql_stmt_execute(stmt)) {
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    /* Извлекаем метаинформацию набора результатов */
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
    if (mysql_num_fields(prepare_meta_result) != 7) {
        fprintf(stderr, " invalid column count returned by MySQL\n");
        mysql_stmt_close(stmt);
        return DB_RETURN_ERROR;
    }
    for (int i = 0; i < order->count; i++) {
        memset(bind, 0, sizeof(bind));
        /* Колонка OrderId в таблице запроса SELECT */
        bind[par_id_1].buffer_type = MYSQL_TYPE_LONG;
        bind[par_id_1].buffer = (char *)&order->order_id;
        /* Колонка OrderDate в таблице запроса SELECT */
        bind[par_date].buffer_type = MYSQL_TYPE_TIMESTAMP;
        bind[par_date].buffer = (char *)&ts;
        /* Колонка OrderDetailId в таблице запроса SELECT */
        bind[par_detail_id].buffer_type = MYSQL_TYPE_LONG;
        bind[par_detail_id].buffer = (char *)&order->order_details[i].order_detail_id;
        /* Колонка OrderId в таблице запроса SELECT */
        bind[par_id_2].buffer_type = MYSQL_TYPE_LONG;
        bind[par_id_2].buffer = (char *)&order->order_details[i].order_id;
        /* Колонка ProductId в таблице запроса SELECT */
        bind[par_prod_id].buffer_type = MYSQL_TYPE_LONG;
        bind[par_prod_id].buffer = (char *)&order->order_details[i].product_id;
        /* Колонка UnitPrice в таблице запроса SELECT */
        bind[par_price].buffer_type = MYSQL_TYPE_LONG;
        bind[par_price].buffer = (char *)&order->order_details[i].unit_price;
        /* Колонка Quantity в таблице запроса SELECT */
        bind[par_quantity].buffer_type = MYSQL_TYPE_LONG;
        bind[par_quantity].buffer = (char *)&order->order_details[i].quantity;
        if (mysql_stmt_bind_result(stmt, bind)) {
            fprintf(stderr, " mysql_stmt_bind_result() failed\n");
            fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
            mysql_stmt_close(stmt);
            return DB_RETURN_ERROR;
        }
        mysql_stmt_fetch(stmt);
        db_convert_time_to_string(ts, order->order_date);
    }
    mysql_free_result(prepare_meta_result);
    if (mysql_stmt_close(stmt)) return DB_RETURN_ERROR;
    db_log_message(DB_LOG_MSG_INFO, "Get order OK");

    return DB_RETURN_OK;
}

/* Функция переводит дату в строку */
int db_convert_time_to_string(MYSQL_TIME ts, char *str) {
    char result[MAX_DATE_LENGTH] = {0};
    snprintf(result, sizeof(result), "%04d-%02d-%02d %02d:%02d:%02d", ts.year, ts.month, ts.day, ts.hour,
             ts.minute, ts.second);
    strcpy(str, result);

    return DB_RETURN_OK;
}
