#include "user.h"

//#define _USE_MAIN_

void print_usage(void)
{
    printf( "my_webbench [option]... URL\n"
            "  -w|--wait                Wait for reply from server.\n"
            "  -t|--time <sec>          Run benchmark for <sec> seconds.\n"
            "  -c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"
            "  -l|--log <n>             Log level. -1 for no log; 1 for ERROR; 2 for WARN; 3 for DEBUG; 4 for INFO.\n"
            "  -9|--http09              Use HTTP/0.9 style requests.\n"
            "  -1|--http10              Use HTTP/1.0 protocol.\n"
            "  -2|--http11              Use HTTP/1.1 protocol.\n"
            "  --cache                  Use cache.\n"
            "  --get                    Use GET request method.\n"
            "  --head                   Use HEAD request method.\n"
            "  --options                Use OPTIONS request method.\n"
            "  --trace                  Use TRACE request method.\n"
            "  -?|-h|--help             This information.\n"
            "  -V|--version             Display program version.\n"
            );
}

void process_url(const char* url, host_info* hinfo)
{
    int i = strstr(url, "://") - url + 3;

    char port[10];
    strncpy(port, index(url + i, ':') + 1, strchr(url + i, '/') - index(url + i, ':') - 1);    
    hinfo->port = atoi(port);
    if (hinfo->port == 0)
    {
        hinfo->port = 80;
    }

    strncpy(hinfo->hostname, url + i, strchr(url + i, ':') - url - i);
}

/*
return: 
*/
int process_user_input(int argc, char** argv, configuration* conf)
{
    //long parameters list
    struct option long_options[] = 
    {
        {"wait",    no_argument,        &conf->wait_respone,     1              },
        {"cache",   no_argument,        &conf->cache,            1              },
        {"time",    required_argument,  NULL,                   't'             },
        {"help",    no_argument,        NULL,                   '?'             },
        {"http09",  no_argument,        NULL,                   '9'             },
        {"http10",  no_argument,        NULL,                   '1'             },
        {"http11",  no_argument,        NULL,                   '2'             },
        {"get",     no_argument,        &conf->method,          METHOD_GET      },
        {"head",    no_argument,        &conf->method,          METHOD_HEAD     },
        {"options", no_argument,        &conf->method,          METHOD_OPTIONS  },
        {"trace",   no_argument,        &conf->method,          METHOD_TRACE    },
        {"version", no_argument,        NULL,                   'V'             },
        {"clients", required_argument,  NULL,                   'c'             },
        {"log",     required_argument,  NULL,                   'l'             },
        {0,         0,                  0,                       0              }
    };
    int options_index = 0;//parameter index
    int opt = 0;
    char* url;
    struct tm time;
    
    if (argc == 1)
    {
        print_usage();
        return 2;
    }

    while ((opt = getopt_long(argc, argv, "912Vwt:p:c:l:?h", long_options, &options_index)) != -1)
    {
        switch (opt)
        {
        case 0:
            break;
        case 'w':
            conf->wait_respone = 1;
            break;
        case '9':
            conf->http_version = HTTP_0_9;
            break;
        case '1':
            conf->http_version = HTTP_1_0;
            break;
        case '2':
            conf->http_version = HTTP_1_1;
            break;
        case 'V':
            printf("%s\n", conf->software_version);
            return 0;
            break;
        case 't':
            conf->benchtime = atoi(optarg);
            break;
        case 'p':
            /*
            TODO: proxy
            */
            break;
        case ':':
        case 'h':
        case '?':
            print_usage();
            return 2;
            break;
        case 'c':
            conf->clients_number = atoi(optarg);
            break;
        case 'l':
            conf->log_level = atoi(optarg);
            break;
        default:
            printf("?? getopt returned character code 0x%02x ??\n", opt);
            break;
        }
    }

    //process the url
    if (optind == argc) //didn't input url
    {
        printf("Missing URL!\n");
        print_usage();
        return 2;
    }
    else
    {
        url = argv[optind];
        process_url(url, &(conf->host_info));
    }

    if (conf->cache == 0 && conf->http_version < HTTP_1_0)
    {
        conf->http_version = HTTP_1_0;
    }
    if (conf->method == METHOD_HEAD && conf->http_version < HTTP_1_0)
    {
        conf->http_version = HTTP_1_0;
    }
    if (conf->method == METHOD_OPTIONS && conf->http_version < HTTP_1_1)
    {
        conf->http_version = HTTP_1_1;
    }
    if (conf->method == METHOD_TRACE && conf->http_version < HTTP_1_1)
    {
        conf->http_version = HTTP_1_1;
    }
    

    if (conf->clients_number == 0)
    {
        conf->clients_number = 1;
    }

    if (conf->benchtime == 0)
    {
        conf->benchtime = 1;
    }    

    time = get_local_time();
    sprintf(conf->log_file_name, "%04u_%02u_%02u_%02u_%02u_%02u.txt",
             time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
             
    return 0;
}

struct tm get_local_time()
{
    time_t tnow;
    tnow = time(0);

    struct tm *sttm;
    sttm = localtime(&tnow);

    return *sttm;
}

#ifdef  _USE_MAIN_
int main(int argc, char** argv)
{

    configuration conf;

    //print_usage();

    process_user_input(argc, argv, &conf);

    printf("software_version %s\n", conf.software_version);
    printf("http_version %d\n", conf.http_version);
    printf("method %d\n", conf.method);
    printf("clients_number %d\n", conf.clients_number);
    printf("wait_respone %d\n", conf.wait_respone);
    printf("cache %d\n", conf.cache);
    printf("benchtime %d\n", conf.benchtime);
    printf("hostname %s\n", (const char*)conf.host_info.hostname);
    printf("port %d\n", conf.host_info.port);

    struct tm sttm = get_local_time();
    printf("%04u-%02u-%02u\n", sttm.tm_year+1900, sttm.tm_mon+1, sttm.tm_mday);

    return 0;
}
#endif