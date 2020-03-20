#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "user.h"
#include "bench.h"
#include "typedef.h"

//main function
int main(int argc, char** argv)
{
    //0.define var
    configuration conf;
    bench_data bd;
    int ret = 0;

    //1.proccess input
    process_user_input(argc, argv, &conf);

    //2.run bench
    ret = run_bench(&conf, &bd);

    //3.show result
    switch (ret)
    {
    case 0:
        printf("run sucess\n");
        fflush(stdout);
        count_bench_data(&bd);
        print_bench_data(&bd);
        break;
    case 1:
    case 2:            
    case 3:            
    case 4:
        printf("run failed!\n");
        fflush(stdout);
        break;

    default:
        break;
    }

    return 0;
}