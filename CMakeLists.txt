cmake_minimum_required(VERSION 3.20)
project(net_learn)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(./tcp_buf)
#
#add_executable(net_learn main.cpp tcp_buf/tcpserver.cpp tcp_buf/tcpclient.cpp)
