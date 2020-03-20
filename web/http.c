#include "http.h"

//#define _USE_MAIN_

//http request

void get_http_request(host_info* hf, request_method rm, http_version http_version, int cache, char *request_message)
{
    memset(request_message, 0, REQUEST_SIZE);

    //1. request line
    switch (rm)
    {
    case METHOD_GET:
        strcpy(request_message, "GET");
        break;
    case METHOD_HEAD:
        strcpy(request_message, "HEAD");
        break;
    case METHOD_OPTIONS:
        strcpy(request_message, "OPTIONS");
        break;
    case METHOD_TRACE:
        strcpy(request_message, "TRACE");
        break;

    default:
        strcpy(request_message, "GET");
        break;
    }
    strcat(request_message, " ");

    //uri
    strcat(request_message, "\/");

    //http version
    switch (http_version)
    {
    case HTTP_0_9:
        strcat(request_message, "\r\n");
        break;
    case HTTP_1_0:
        strcat(request_message, " HTTP/1.0\r\n");
        break;
    case HTTP_1_1:
        strcat(request_message, " HTTP/1.1\r\n");
        break;
    
    default:
        break;
    }

    //2. request head
    //agent
    if (http_version > HTTP_0_9)
    {
        strcat(request_message, "User-Agent: WebBench\r\n");
        strcat(request_message, "Host: ");
        strcat(request_message, (const char*)hf->hostname);
        strcat(request_message, "\r\n");
    }
    if (!cache)
    {
        strcat(request_message, "Pragma: no-cache\r\n");
    }
    if (http_version > HTTP_1_0)
    {
        strcat(request_message, "Connection: close\r\n");
    }
    if (http_version > HTTP_0_9)
    {
        strcat(request_message, "\r\n");
    }

    //3. request body
    /*
    TODO
    */
    printf("-------------");
    printf("Request message is:\n%s\n", request_message);
    printf("-------------");
}

#ifdef _USE_MAIN_
int main()
{
    host_info hf;
    const char tmp[64] = "www.baidu.com";
    char message[REQUEST_SIZE];
    
    memcpy(hf.hostname, tmp, sizeof(tmp));

    get_http_request(&hf, METHOD_GET, 1, 1, &message);
    
    return 0;
}
#endif