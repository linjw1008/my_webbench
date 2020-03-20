#include "sockets.h"

//#define _USE_MAIN_

int get_socket(const char* hostname, int port)
{
    //create socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
    {
        return sock;
    }

    //set ip and port
    struct sockaddr_in addr;
    unsigned long inaddr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inaddr = inet_addr(hostname);
    if (inaddr != INADDR_NONE)  //hostname is an ip address
    {
        addr.sin_addr.s_addr = inaddr;
    }
    else  //hostname is not an ip address
    {
        //get ip address by hostname
        struct hostent* ip;
        ip = gethostbyname(hostname);
        if (ip == NULL)
        {
            return -1;
        }
        memcpy(&addr.sin_addr, ip->h_addr_list[0], ip->h_length);
    }
    addr.sin_port = htons(port);

    //connect server
    if (connect(sock, (struct sockaddr_in*)&addr, sizeof(addr)) < 0)
    {
        return -1;
    }
    
    return sock;
}

int test_connection(const char* hostname, int port)
{
    int i = get_socket(hostname, port);
    if (i < 0)
    {
        printf("Connect to %s:%d failed!\n", hostname, port);
        return i;
    }
    
    close(i);

    return 0;
}

/*
return:
    0, success
    1, get socket failed
    2, write failed
    3, http 0.9 shutdown failed
    4, wait respone failed
    5, close failed
*/
int send_message(const char* hostname, int port, const char* message, int* bytes_number, http_version http_version, int wait_respone)
{
    int sock = get_socket(hostname, port);
    int len = strlen(message);

    if (sock < 0)
    {
        return 1;
    }

    //write message 
    if (len != write(sock, message, len))
    {
        close(sock);
        return 2;
    }


    if (http_version == HTTP_0_9)
    {
        if (shutdown(sock, SHUT_WR) == -1)
        {
            close(sock);
            return 3;
        }
    }

    if (wait_respone == 1)
    {
        char buf[1500];
        int ret = 0;
        *bytes_number = 0;

        while(1)
        {
            ret = read(sock, buf, 1500);
            if (ret < 0)
            {
                close(sock);
                return 4;
            }
            else if (ret == 0)
            {
                break;
            }
            else
            {
                *bytes_number += ret;
                //printf(buf);
            }
        }
    }
    
    if (close(sock) == -1)
    {
        return 5;
    }

    return 0;
}

#ifdef _USE_MAIN_

int main()
{
    int sock = get_socket("baidu.com", 80);
    char req[] = "GET / HTTP/1.0\r\nUser-Agent: WebBench\r\nHost: www.baidu.com\r\nPragma: no-cache\r\n\r\n";
    int bytes = 0;

    close(sock);

    test_connection("www.baidu.", 80);

    int ret = send_message("www.baidu.com", 80, req, &bytes, 1, 0);

    printf("----%d---\n", ret);
    printf("----%d---\n", bytes);

    return 0;
}

#endif