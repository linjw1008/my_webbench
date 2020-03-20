#include "log.h"

//#define _USE_MAIN_

#ifdef _USE_MAIN_
int main()
{
    int fp = open("./log_test.txt", O_WRONLY | O_CREAT | O_APPEND);
    log(fp, ERROR, "log%d\n", 100);
    printf("log file: %d\n", fp);
    close(fp);
    return 0;
}
#endif