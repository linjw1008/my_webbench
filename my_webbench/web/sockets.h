#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "typedef.h"
#include "log.h"

int get_socket(const char* hostname, int port);
int test_connection(const char* hostname, int port);
int send_message(const char* hostname, int port, const char* message, int* bytes_number, http_version http_version, int wait_respone);

#endif