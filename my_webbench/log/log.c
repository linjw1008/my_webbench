#include "log.h"
#include <string.h>
#include <stdlib.h>

void print_log(int log_type, const char *msg)
{
    const char *log_type_str;
    switch (log_type)
    {
    case LOG_DEBUG:
        log_type_str = "debug";
        break;
    case LOG_MSG:
        log_type_str = "msg";
        break;
    case LOG_WARN:
        log_type_str = "warn";
        break;
    case LOG_ERR:
        log_type_str = "err";
        break;
    default:
        log_type_str = "???";
        break;
    }
    fprintf(stdout, "[%s] %s\n", log_type_str, msg);
}

void logx(int log_type, const char *fmt, ...)
{
    char buf[512];

    va_list ap;
    va_start(ap, fmt);
    
    if (fmt != NULL)
        vsnprintf(buf, sizeof(buf), fmt, ap);
    else
        buf[0] = '\0';

    print_log(log_type, buf);
    
    va_end(ap);
}


void error(int status, int err, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    if (err)
        fprintf(stderr, ": %s (errno %d)\n", strerror(err), err);
    
    if (status)
        exit(status);
}