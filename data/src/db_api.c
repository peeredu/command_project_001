#include "db_api.h"

/* Функция установления соединения с БД. Возвращает 0 если соединение
 * установлено */
int db_get_connect(MYSQL *conn) {
    int result = 0;
    // Получаем дескриптор соединения
    if (!mysql_init(conn)) {
        fprintf(stderr, "Error: can't create MySQL-descriptor\n");
        result = 1;
    } else {
        unsigned int db_timeout = DB_TIMEOUT;
        mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, (int *)&db_timeout);
        // Устанавливаем соединение с базой данных
        if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0)) {
            // Если соединение не установлено выводим сообщение об ошибке
            fprintf(stderr, "Error: %s\n", mysql_error(conn));
            result = 1;
        }
    }

    return result;
}