#include "bench.h"

//#define _USE_MAIN_

int time_out = 0;

/*
return:
    0: success
    1: connect failed
    2: create pipe failed
    3: fork failed
    4: open pipe error
*/
int run_bench(configuration* conf, bench_data* bd)
{
    //0.define var
    int pipedes[2];
    int i;
    pid_t pid = 0;
    FILE* f;
    char request_message[REQUEST_SIZE];
    int pid_list[conf->clients_number];
    
    //1.test the host
    i = test_connection((const char*)conf->host_info.hostname, conf->host_info.port);
    if (i < 0) //connect failed!
    {
        LOGERR("\033[KTry connecting to host failed!");
        return 1;
    }
    
    //2.create the pipe
    if (pipe(pipedes) == -1)
    {
        LOGERR("\033[KCreate pipe failed! Error: %s", strerror(errno));
        return 2;
    }
    
    //3.get the request message
    get_http_request(&(conf->host_info), conf->method, conf->http_version, conf->cache, request_message);
    LOGMSG("\033[Kthe request is: \n%s", request_message);

    //4.create child processes
    for (i = 0; i < conf->clients_number; i++)
    {
        //fork a child process
        pid = fork();
        if (pid == -1) //fork failed
        {
            int j = 0;
            sleep(1);
            LOGWARN("\033[KNo.%d process fork failed!", i);

            /*kill all children*/
            //force kill, not elegant
            //kill(, SIGKILL);
            for (j = 0; j < i; j++)
            {
                kill(pid_list[j], SIGKILL);
                LOGMSG("\033[KKill child process(pid: %d).", pid_list[j]);                
            }
            return 3;
        }
        else if (pid == 0) //is child process
        {
            LOGMSG("\033[KNo.%d client was created.", i);
            sleep(1);
            break;
        }
        else
        {
            pid_list[i] = pid;
        }
    }

    //5.deal with parent and child processes
    if (pid == 0)   //child process
    {
        int bytes = 0;
        int success = 0;
        int fail = 0;
        int pid;
        pid = getpid();

        if (benchcore(conf, request_message, &bytes, &success, &fail) != 0)
        {
            ERROR(1, errno, "\033[KPid %d: benchcore failed!", pid);
        }

        LOGMSG("\033[KPid %d results: bytes[%d], success[%d], fail[%d]", pid, bytes, success, fail);
        
        f = fdopen(pipedes[1], "w");
        if (f == NULL)
        {
            ERROR(2, errno, "\033[KPid %d: open pipe for writing failed.", pid);
        }
        close(pipedes[0]);

        //send result to parent
        fprintf(f, "%d %d %d %d %d\n", bytes, success, fail, i, pid);
        fclose(f);

        //return 0;
        exit(0);    //success
    }
    else    //parent process
    {
        int clients_number = conf->clients_number;
        int benchtime = conf->benchtime;
        int bytes = 0;
        int success = 0;
        int fail = 0;
        int total_bytes = 0;
        int total_success = 0;
        int total_fail = 0;
        int child_index = 0;
        int child_pid = 0;
        int child_status = 0;
        int i = 0;
        int pid = 0;

        f = fdopen(pipedes[0], "r");
        if (f == NULL)
        {
            LOGERR("\033[Kopen pipe for writing failed.");
            return 4;
        }
        close(pipedes[1]);

        //set buffer type
        //no buf
        setvbuf(f, NULL, _IONBF, 0);

        //show progress bar
        show_timing_progress_bar(benchtime);
        
        //collect data
        while (1)
        {            
            pid = wait(&child_status);

            if (pid == -1)  //no child process
            {
                break;
            }
            else    //child process exits
            {
                int ret = 0;
                
                switch (child_status) 
                {
                case 0: //success
                    //receive data from child
                    ret = fscanf(f, "%d %d %d %d %d\n", &bytes, &success, &fail, &child_index, &child_pid);
                    if (ret < 5)
                    {
                        LOGERR("\033[KSome of the children died.");
                        break;
                    }

                    LOGMSG("\033[KReceive message from No.%d client(pid: %d). The status is: %d.", child_index, child_pid, child_status);

                    total_bytes += bytes;
                    total_success += success;
                    total_fail += fail;
                    break;
                case 1: //benchcore failed
                    LOGERR("\033[KNo.%d client(pid %d) failed. Reason: Benchcore failed.\n", i, pid);
                    break;
                case 2: //pipe failed
                    LOGERR("\033[KNo.%d client(pid %d) failed. Reason: Pipe failed.\n", i, pid);              
                    break;

                default:
                    break;
                }

                show_collect_data_progress_bar(i, clients_number);  //show progress bar
                i++;
            }
        }
        
        fclose(f);

        LOGMSG("\033[Ktotal_success: %d total_fail: %d total_bytes: %d\n", total_success, total_fail, total_bytes);
        bd->total_bytes = total_bytes;
        bd->total_fail = total_fail;
        bd->total_success = total_success;
        bd->time = conf->benchtime;
        bd->clients_num = conf->clients_number;        
    }
    
    return 0;
}

void alarm_handler(int signal)
{
    time_out = 1;
}

/*
return:
    0, success
    1, sigaction failed
*/
int benchcore(configuration* conf, const char* request_message, int* bytes, int* success, int* fail)
{
    int b = 0;
    *success = 0;
    *fail = 0;
    *bytes = 0;
    struct sigaction sa;
    sa.sa_handler = alarm_handler;
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, NULL))
    {
        return 1;
    }

    alarm(conf->benchtime);
    while(1)
    {
        if (time_out)
        {
            break;
        }

        if(send_message(conf->host_info.hostname, conf->host_info.port, request_message, &b, conf->http_version, conf->wait_respone) == 0)
        {
            *success = (*success) + 1;
            *bytes = (*bytes) + b;
        }
        else
        {
            if (!time_out)  //if time out, send_message may be failed!
            {
                *fail = (*fail) + 1;
            }
        }                
    }

    return 0;
}

void count_bench_data(bench_data* bd)
{
    if (bd->time <= 0 || bd == NULL)
    {
        return;
    }

    bd->bytes_per_sec = bd->total_bytes / (float)(bd->time * 1.0);
    bd->pages_per_min = bd->total_success / ((bd->time * 1.0) / 60.0);
    bd->success_per_client = bd->total_success / (float)bd->clients_num;
    bd->fail_per_client = bd->total_fail / (float)bd->clients_num;
}

void print_bench_data(bench_data* bd)
{
    if (bd == NULL)
    {
        return;
    }
    LOGMSG("show results: ");
    printf("The bench results are:\n");
    printf("Bench time: \t%ds\n", bd->time);
    printf("Clients number: \t%d\n", bd->clients_num);
    printf("Success: \t%d[total], \t%.2f[per client]\n", bd->total_success, bd->success_per_client);
    printf("Fail: \t%d[total], \t%.2f[per client]\n", bd->total_fail, bd->fail_per_client);
    printf("Speed: \t%.2fbytes/s, \t%.2fMB/s, \t%.2fpages/min\n", bd->bytes_per_sec, bd->bytes_per_sec/1024.0/1024.0, bd->pages_per_min);
    fflush(stdout);
}

void show_timing_progress_bar(int time)
{
    const int bar_len = 40;
    int i, j;
    char bar[bar_len + 1];
    const char* label = "|/-\\";
    int index = 0;
    int last_index = 0;

    memset(bar, '\0', sizeof(bar));

    for (i = 0; i < time; i++)
    {
        index = (i == time - 1) ? bar_len - 1 : (int)(bar_len*1.0*i/time);
        for (j = last_index; j <= index; j++)
        {
            bar[j] = '#';
        }
        last_index = index + 1;
        printf("[Timing...][%-40s][%ds][%c]\r", bar, i + 1, label[i % 4]);
        if (i == time - 1)
        {
            printf("\n");
        }
        fflush(stdout);
        sleep(1);
    }
}

void show_collect_data_progress_bar(int index, int total)
{
    const int bar_len = 40;
    static char bar[40 + 1];
    const char* label = "|/-\\";
    static int last_i = 0;
    int i;
    int j;

    //if index == 0, then reset the bar
    if (index == 0)
    {
        last_i = 0;
        memset(bar, '\0', sizeof(bar));
    }

    i = (index == total - 1) ? bar_len - 1 : (int)(bar_len*1.0*index/total);
    for (j = last_i; j <= i; j++)
    {
        bar[j] = '#';
    }
    last_i = i + 1;
    printf("[Collecting results...][%-40s][No.%d client of %d in total][%c]\r", bar, index + 1, total, label[index % 4]);
    if (index == total - 1)
    {
        printf("\n");
    }
    fflush(stdout);
}


#ifdef _USE_MAIN_
int main()
{
    /*
    host_info hf;
    configuration conf;
    const char tmp[64] = "www.taobao.com";
    int ret = 0;
    int bytes = 0;

    memcpy(hf.hostname, tmp, sizeof(tmp));
    hf.port = 80;
    conf.host_info = hf;
    conf.clients_number = 20;
    conf.benchtime = 5;
    conf.wait_respone = 1;
    conf.cache = 0;
    conf.http_version = HTTP_1_0;
    conf.method = METHOD_GET;
    ret = run_bench(&conf);
    */
    //show_bench_progress_bar(12);
    
    //benchcore(&conf, "GET / HTTP/1.0\r\nUser-Agent: WebBench\r\nHost: www.baidu.com\r\nPragma: no-cache\r\n\r\n", &bytes);

    //printf("----%d----\n", ret);

    int i = 0;
    for (i = 0; i < 10; i++)
    {
        printf("\r\033[K");
        printf("test------------------\r\n");
        show_collect_data_progress_bar(i, 10);
        sleep(1);
    }
    

    return 0;
}
#endif