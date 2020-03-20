#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define ERROR   1
#define WARN    2
#define DEBUG   3
#define INFO    4


#define log(file, level, format, ...)\
        {\
            char buf[1024];\
            switch (level)\
            {\
            case ERROR:\
                sprintf(buf, "ERROR: Function: %s, Line: %d, ---> "format"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
                break;\
            case WARN:\
                sprintf(buf, "WARN: Function: %s, Line: %d, ---> "format"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
                break;\
            case DEBUG:\
                sprintf(buf, "DEBUG: Function: %s, Line: %d, ---> "format"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
                break;\
            case INFO:\
                sprintf(buf, "INFO: Function: %s, Line: %d, ---> "format"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
                break;\
            default:\
                break;\
            }\
            write(file, buf, strlen(buf));\
        }

#endif