cmake_minimum_required(VERSION 4.0)
project(ClientServer)

set(CMAKE_CXX_STANDARD 20)

#include_directories(server/headers)

add_executable(Server
        server/src/main.cpp
        server/src/server.cpp
)
target_include_directories(Server PRIVATE server/headers)

#include_directories(client/headers)

add_executable(Client
        client/src/main.cpp
)
target_include_directories(Client PRIVATE client/headers)
