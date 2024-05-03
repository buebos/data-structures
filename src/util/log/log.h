#ifndef __CRES_LOG_H__

#define __CRES_LOG_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"

typedef enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

void cres_log(LogLevel log_level, char* msg, ...);

#endif /** __CRES_LOG_H__ */
