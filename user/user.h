#ifndef _USER_H_
#define _USER_H_

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "typedef.h"

void print_usage(void);
void process_url(const char* url, host_info* hinfo);
int process_user_input(int argc, char** argv, configuration* conf);
struct tm get_local_time(); 

#endif