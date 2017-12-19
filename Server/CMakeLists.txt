cmake_minimum_required(VERSION 2.8)

project(TestServer)

add_library(ServerUDP STATIC ServerUDP.cpp)
add_library(ServerTCP STATIC ServerTCP.cpp)
add_executable(TestServer main.cpp)

target_link_libraries(TestServer ServerUDP ServerTCP pthread)
