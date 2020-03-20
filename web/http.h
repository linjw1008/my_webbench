#ifndef _HTTP_H_
#define _HTTP_H_

#include <string.h>
#include <stdio.h>
#include "typedef.h"

#define REQUEST_SIZE    2048

void get_http_request(host_info* hf, request_method rm, http_version http_version, int cache, char *request_message);

#endif