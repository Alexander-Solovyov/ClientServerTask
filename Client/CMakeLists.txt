cmake_minimum_required(VERSION 2.8)

project(TestClient)

add_library(Client STATIC Client.cpp)
add_executable(TestClient main.cpp)

target_link_libraries(TestClient Client)
