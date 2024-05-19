#ifdef DEBUG

#ifndef printdev

#include "log/log.h"
#define printdev(message, ...) printlog(LOG_DEBUG, message, ##__VA_ARGS__)

#endif /* printdev */

#ifndef debug

#define debug(snippet) snippet

#endif /* debug */

#else

#define printdev(message, ...)
#define debug(snippet)

#endif
