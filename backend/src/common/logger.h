#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LOG_MESSAGE_BUFFER_SIZE 1024
#define DB_LOG_MSG_INFO "Info"
#define DB_LOG_MSG_ERROR "Error"

void init_logger();

void log_message(const char* message);
void log_message2(const char* message, const char* message2);
void print_date_time(void);
void close_logger();

void db_close_logger(void);
void db_log_message(const char* type, const char* message);

#endif  // LOGGER_H
