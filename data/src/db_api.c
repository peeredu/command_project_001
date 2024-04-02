#include "db_api.h"

/* Функция установления соединения с БД. Возвращает 0 если соединение установлено */
int db_get_connect(MYSQL *conn) {
    int error = 0;
    // Получаем дескриптор соединения
    if (!mysql_init(conn)) {
        fprintf(stderr, "Error: can't create MySQL-descriptor\n");
        error = 1;
    } else {
        unsigned int db_timeout = DB_TIMEOUT;
        mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, (int *)&db_timeout);
        // Устанавливаем соединение с базой данных
        if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0)) {
            // Если соединение не установлено выводим сообщение об ошибке
            fprintf(stderr, "Error: %s\n", mysql_error(conn));
            error = 1;
        }
    }

    return error;
}

/* Функция добавления нового продукта */
int db_add_product(MYSQL *conn, Product product) {
    int error = 0;
    enum select_params { par_name, par_price, par_quantity, par_active, par_count };
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[par_count] = {0};
    stmt = mysql_stmt_init(conn);
    if (NULL == stmt) error = 1;
    if (!error) {
        char *command = "INSERT INTO Products (ProductName, UnitPrice, Quantity, Active) VALUES (?, ?, ?, ?)";
        bool prepare_fail = mysql_stmt_prepare(stmt, command, strlen(command));
        if (prepare_fail) error = 1;
    }
    if (!error) {
        bind[par_name].buffer_type = MYSQL_TYPE_STRING;
        bind[par_name].buffer = (char *)product.name;
        bind[par_name].buffer_length = strlen(product.name);
        bind[par_price].buffer_type = MYSQL_TYPE_DOUBLE;
        bind[par_price].buffer = (char *)&product.unit_price;
        bind[par_quantity].buffer_type = MYSQL_TYPE_LONG;
        bind[par_quantity].buffer = (char *)&product.quantity;
        bind[par_active].buffer_type = MYSQL_TYPE_LONG;
        bind[par_active].buffer = (char *)&product.active;
        if (mysql_stmt_bind_param(stmt, bind)) error = 1;
    }
    if (mysql_stmt_execute(stmt)) error = 1;
    if (mysql_stmt_close(stmt)) error = 1;
    return error;
}
