#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "utils/log.h"
#include "utils/macros.h"
#include "utils/colors.h"

void _sds_set_color(char *seq) {
    printf("\033[%sm", seq);
}

SDS_INTERNAL int sds_log(sds_log_level, const char * restrict, va_list);
SDS_INTERNAL void sds_write_to_log(const char * restrict);

SDS_PUBLIC int sds_log_debug(const char * restrict format, ...) {
    va_list args;
    va_start(args, format);

    int i = sds_log(SDS_LOG_LEVEL_DEBUG, format, args);

    va_end(args);
    return i;
}

SDS_PUBLIC int sds_log_warning(const char * restrict format, ...) {
    va_list args;
    va_start(args, format);

    int i = sds_log(SDS_LOG_LEVEL_WARNING, format, args);

    va_end(args);
    return i;
}

SDS_PUBLIC int sds_log_error(const char * restrict format, ...) {
    va_list args;
    va_start(args, format);

    int i = sds_log(SDS_LOG_LEVEL_ERROR, format, args);

    va_end(args);
    return i;
}

SDS_PUBLIC int sds_log_fatal(const char * restrict format, ...) {
    va_list args;
    va_start(args, format);

    int i = sds_log(SDS_LOG_LEVEL_FATAL, format, args);

    va_end(args);
    return i;
}

SDS_INTERNAL int sds_log(sds_log_level level, const char * restrict format, va_list args) {
    if(LOG_LEVEL > level) {
        return 1;
    }

    int written;
    size_t max_buffer = 256;
    char *buffer = (char *) calloc(max_buffer, sizeof(char));

    if(!buffer) {
        sds_log_error("[CORE] Failed to allocate log-buffer.");
        return 1;
    }
        
    written = vsnprintf(buffer, max_buffer, format, args);

    if(written >= max_buffer) {
        sds_log_warning("The following log-output has been truncated, due to the character limit (%d)", max_buffer);
    }

    switch(level) {
        case SDS_LOG_LEVEL_DEBUG:
            sds_set_color(SDS_LIGHT_BLUE);
            printf("[DEBUG]    ");
            sds_set_color(SDS_RESET);
            printf("%s\n", buffer);
            break;

        case SDS_LOG_LEVEL_WARNING:
            sds_set_color(SDS_YELLOW);
            printf("[WARNING]  ");
            sds_set_color(SDS_RESET);
            printf("%s\n", buffer);
            break;

        case SDS_LOG_LEVEL_ERROR:
            sds_set_color(SDS_LIGHT_RED);
            printf("[ERROR]    ");
            sds_set_color(SDS_RESET);
            printf("%s\n", buffer);
            break;

        case SDS_LOG_LEVEL_FATAL:
            sds_set_color(SDS_RED);
            printf("[FATAL]    ");
            sds_set_color(SDS_RESET);
            printf("%s\n", buffer);
            break;

        case SDS_LOG_LEVEL_NONE:  return 1;
        default:                  return 1;
    }

    sds_write_to_log(buffer);

    return 0;
}

SDS_INTERNAL void sds_write_to_log(const char * restrict message) {
    size_t max_buffer = 296;
    char *buffer;
    time_t cur_time = time(NULL);
    struct tm *date;

    buffer = (char *) calloc(max_buffer, sizeof(char));
    if(!buffer) {
        sds_log_error("[CORE] Failed to allocate log-buffer.");
        return;
    }

    date = localtime(&cur_time);
    snprintf(buffer, max_buffer, "[%02d-%02d-%d %02d:%02d:%02d] %s\n",
        date->tm_mday, date->tm_mon, date->tm_year + 1900,
        date->tm_hour, date->tm_min, date->tm_sec, //^^^^ tm_year counts from 1900.
        message
    );

#if SDS_LOG_TO_FILE
    FILE *log_file = fopen(SDS_LOG_FILE, "a");
    
    if(!log_file) {
        // Prevent log-recursion.
        sds_set_color(SDS_LIGHT_RED);
        printf("[ERROR]    ");
        sds_set_color(SDS_RESET);
        printf("[CORE] Error loading log-file '%s'\n", SDS_LOG_FILE);
        return;
    }

    fwrite(buffer, sizeof(char), strlen(buffer), log_file);
    fclose(log_file);
#endif /* SDS_LOG_TO_FILE */
}
