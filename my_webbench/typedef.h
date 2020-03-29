#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <rpc/types.h>

//some self defined types

//request method
typedef enum
{
    METHOD_GET      = 0,
    METHOD_HEAD     = 1,
    METHOD_OPTIONS  = 2,
    METHOD_TRACE    = 3
} request_method; 

//http version
typedef enum
{
    HTTP_0_9    = 0,
    HTTP_1_0    = 1,
    HTTP_1_1    = 2
} http_version;

typedef struct 
{
    char    hostname[MAXHOSTNAMELEN];
    int     port;
} host_info;

//configuration
typedef struct 
{
    const char      software_version[1000];
    http_version    http_version;
    request_method  method;
    int             clients_number;
    int             wait_respone;
    int             cache;
    int             benchtime;
    host_info       host_info;
} configuration;

//bench data
typedef struct
{
    int total_bytes;
    int total_fail;
    int total_success;
    int time;
    float pages_per_min;
    float bytes_per_sec;
    float success_per_client;
    float fail_per_client;

    int clients_num;
} bench_data;

#endif