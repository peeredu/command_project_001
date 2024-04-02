#ifndef LOGGER_H
#define LOGGER_H
#include <stdio.h>

#define LOG_MESSAGE_BUFFER_SIZE 1024

#include "errors.h"
void init_logger();

void log_message(const char* message);
void log_message2(const char* message, const char* message2);
void print_date_time(void);
void close_logger();

#endif
