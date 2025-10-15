cmake_minimum_required(VERSION 4.0)
project(ClientServer)

set(CMAKE_CXX_STANDARD 20)

add_executable(Server
        server/src/main.cpp
        server/src/server.cpp
)
target_include_directories(Server PRIVATE server/headers)

add_executable(Client
        client/src/main.cpp
        client/src/client.cpp
)
target_include_directories(Client PRIVATE client/headers)
