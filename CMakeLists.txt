cmake_minimum_required(VERSION 4.0)
project(Server)

set(CMAKE_CXX_STANDARD 20)

include_directories(server/headers)

add_executable(Server
        server/src/main.cpp
        server/src/server.cpp
)