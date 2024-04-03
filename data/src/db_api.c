#include "db_api.h"

/* Функция установления соединения с БД. Возвращает 0 если соединение установлено */
int db_get_connect(MYSQL *const conn) {

    // Получаем дескриптор соединения
    if (!mysql_init(conn)) {
        fprintf(stderr, "Error: can't create MySQL-descriptor\n");
        return DB_RETURN_ERROR;
    } else {
        unsigned int db_timeout = DB_TIMEOUT;
        mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, (int *)&db_timeout);
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
    if (mysql_stmt_close(stmt)) DB_RETURN_ERROR;
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
