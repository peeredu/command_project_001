#include "logger.h"

static FILE *plog_file = NULL;
static FILE *fot_db_log = NULL;

void init_logger(void) {
    plog_file = fopen("log.txt", "a");
    fot_db_log = fopen("db_log.txt", "a");
    if (plog_file == NULL) {
            fprintf(stderr, "Can't open log file\n");
            exit(EXIT_FAILURE);
    }
    if (fot_db_log == NULL) {
        fprintf(stderr, "Can't open log file\n");
    }
    fprintf(plog_file, "-------------------\n");
}

void log_message(const char *message) {
    if (plog_file == NULL) {
        init_logger();
    }
    if (plog_file != NULL) {
        print_date_time();
        fprintf(plog_file, "%s\n", message);
        fflush(plog_file);
    }
}
void log_message2(const char *message, const char *message2) {
    char log_msg[LOG_MESSAGE_BUFFER_SIZE];
    sprintf(log_msg, "%s:\n%s",message, message2);
    log_message(log_msg);
}

void close_logger(void) {
    if (plog_file != NULL) {
        fclose(plog_file);
    }
    plog_file = NULL;
}

void print_date_time(void) {
    if (plog_file == NULL) {
        init_logger();
    }
    if (plog_file != NULL) {
        time_t rawtime;
        struct tm *timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        fprintf(plog_file, "%d-%02d-%02d %02d:%02d:%02d | ", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,
                timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    }
}

void db_close_logger(void) {
    if (fot_db_log != NULL) {
        fclose(fot_db_log);
    }
    fot_db_log = NULL;
}

void db_log_message(const char *type, const char *message) {
    if (fot_db_log == NULL) {
        init_logger();
    }
    if (fot_db_log != NULL) {
        time_t current_time = time(NULL);
        struct tm *now = localtime(&current_time);
        fprintf(fot_db_log, "[DB] ");
        fprintf(fot_db_log, "[%02d.%02d.%d ", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
        fprintf(fot_db_log, "%02d:%02d:%02d] ", now->tm_hour, now->tm_min, now->tm_sec);
        fprintf(fot_db_log, "[%s] %s\n", type, message);
        fflush(fot_db_log);
    }
    db_close_logger();
}
