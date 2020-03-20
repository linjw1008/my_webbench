#ifndef _BENCH_H_
#define _BENCH_H_

#include <stdlib.h>
#include <stdio.h>
#include <rpc/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include "sockets.h"
#include "http.h"
#include "log.h"
#include "typedef.h"

int run_bench(configuration* conf, bench_data* bd);
void alarm_handler(int signal);
int benchcore(configuration* conf, const char* request_message, int* bytes, int* success, int* fail);
void count_bench_data(bench_data* bd);
void print_bench_data(bench_data* bd);
void show_timing_progress_bar(int time);
void show_collect_data_progress_bar(int index, int total);

#endif