# CMake version
cmake_minimum_required(VERSION 3.10.2)
# project name
project(bench)
# set bin directory
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set lib directory
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)  
# look for all source files
aux_source_directory(. DIR_SRCS)
# add header file include directories
include_directories(./ ./bench ./log ./user ./web)
# add block directories
add_subdirectory(bench)
add_subdirectory(log)
add_subdirectory(user)
add_subdirectory(web)
# build
add_executable(main ${DIR_SRCS})
# link
target_link_libraries(main bench log user sockets http)