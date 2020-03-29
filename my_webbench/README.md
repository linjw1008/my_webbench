# 简介
一个参考网站压测工具，参考了webbench源码，将各个功能模块化处理。
## 基本原理
采用父-子进程模型。
子进程不断对目标网站发起http请求，并统计数据。
将到达指定时间后，子进程将统计数据通过管道发送给父进程，随后exit。
父进程生成所有子进程后，开始等待子进程exit并回收，同时读取管道数据。
所有子进程回收完成后，统计输出总的结果。
## 各模块简介
### ./bench
核心模块，在其中完成父子进程的管理，执行父子进程的相关操作。
### ./log
一个基于宏定义的log模块，较为独立，可以单独移植到其他程序中使用。
### ./user
一些与用户交互相关的功能。
### ./web
网络相关功能，其中http.c用于生成http报文，socket.c处理套接字与消息发送。
### ./typedef.h
定义了程序中用到的一些数据结构，如用户配置、测试结果等。
### ./main.c
主函数。

# 如何编译
## cmake
```
> cmake .
> make
```

# 如何使用
## 命令列表
```
./bin/main [option]... URL\n"
-w|--wait                Wait for reply from server.\n"
-t|--time <sec>          Run benchmark for <sec> seconds.\n"
-c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"
-9|--http09              Use HTTP/0.9 style requests.\n"
-1|--http10              Use HTTP/1.0 protocol.\n"
-2|--http11              Use HTTP/1.1 protocol.\n"
--cache                  Use cache.\n"
--get                    Use GET request method.\n"
--head                   Use HEAD request method.\n"
--options                Use OPTIONS request method.\n"
--trace                  Use TRACE request method.\n"
-?|-h|--help             This information.\n"
-V|--version             Display program version.\n"
```
## 示例
```
> ./bin/main -w -t 60 -c 10 -1 -2 --cache --get http://www.baidu.com:80/
```
