#include "db_logger.h"

static FILE *fot_db_log = NULL;

void db_init_logger() {
    fot_db_log = fopen("db_log.txt", "a");
    if (fot_db_log == NULL) {
        fprintf(stderr, "Can't open log file\n");
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
        db_init_logger();
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
