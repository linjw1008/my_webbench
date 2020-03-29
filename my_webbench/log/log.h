#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>
#include <stdio.h>

#define LOG_DEBUG   0
#define LOG_MSG     1
#define LOG_WARN    2
#define LOG_ERR     3

void print_log(int log_type, const char *msg);
void logx(int log_type, const char *fmt, ...);
void error(int status, int err, const char *fmt, ...);

#define LOGDEBUG(fmt, ...)  logx(LOG_DEBUG, "in %s: "fmt, __FUNCTION__, ##__VA_ARGS__)
#define LOGMSG(fmt, ...)    logx(LOG_MSG,   "in %s: "fmt, __FUNCTION__, ##__VA_ARGS__)
#define LOGWARN(fmt, ...)   logx(LOG_WARN,  "in %s: "fmt, __FUNCTION__, ##__VA_ARGS__)
#define LOGERR(fmt, ...)    logx(LOG_ERR,   "in %s: "fmt, __FUNCTION__, ##__VA_ARGS__)

#define ERROR(status, err, fmt, ...) error(status, err, "ERROR in %s: "fmt, __FUNCTION__, ##__VA_ARGS__)

#endif