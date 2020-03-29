#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "user.h"
#include "bench.h"
#include "typedef.h"
#include "log.h"

//main function
int main(int argc, char** argv)
{
    LOGMSG("start");

    //0.define var
    configuration conf = {.software_version = "1.0"};
    bench_data bd;
    int ret = 0;

    //1.proccess input
    LOGMSG("process input parameters.");
    ret = process_user_input(argc, argv, &conf);
    if (ret == 1 || ret == 2)
        return 0;

    //2.run bench
    LOGMSG("start bench.");
    ret = run_bench(&conf, &bd);

    //3.show result
    switch (ret)
    {
    case 0:
        LOGMSG("run successfully!");
        count_bench_data(&bd);
        print_bench_data(&bd);
        break;
    case 1:
    case 2:            
    case 3:            
    case 4:
        LOGERR("run failed!");
        fflush(stdout);
        break;

    default:
        break;
    }

    LOGMSG("exit...");
    return 0;
}