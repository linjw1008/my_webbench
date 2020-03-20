# 简介
一个参考网站压测工具，参考了webbench源码，增加了log功能，并将各个功能模块化处理。
# 如何编译
## make
```
> make
```

## 如何使用
### 命令列表
```
main [option]... URL\n"
-w|--wait                Wait for reply from server.\n"
-t|--time <sec>          Run benchmark for <sec> seconds.\n"
-c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"
-l|--log <n>             Log level. -1 for no log; 1 for ERROR; 2 for WARN; 3 for DEBUG; 4 for INFO.\n"
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
### 示例
```
> main -w -t 60 -c 10 -l -1 -2 --cache --get http://www.baidu.com
```
