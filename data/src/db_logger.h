#ifndef DB_LOGGER_H
#define DB_LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DB_LOG_MSG_INFO "Info"
#define DB_LOG_MSG_ERROR "Error"

void db_init_logger();
void db_close_logger(void);
void db_log_message(const char *type, const char *message);

#endif  // DB_LOGGER_H
