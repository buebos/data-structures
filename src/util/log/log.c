#include "log.h"

void cres_log(LogLevel log_level, char* msg, ...) {
    va_list params;

    char log_prefix[16] = {0};
    char* color_prefix = AC_RED;

    switch (log_level) {
        case LOG_DEBUG:
            strcat(log_prefix, "DEBUG");
            color_prefix = AC_PURPLE;
            break;
        case LOG_INFO:
            strcat(log_prefix, "INFO");
            color_prefix = AC_GREEN;
            break;
        case LOG_WARN:
            strcat(log_prefix, "WARN");
            color_prefix = AC_YELLOW;
            break;
        default:
            strcat(log_prefix, "ERROR");
    }

    va_start(params, msg);

    printf("%s[%s]: " AC_CYAN, color_prefix, log_prefix);
    vprintf(msg, params);
    printf(AC_RESET "\n");

    va_end(params);
}
