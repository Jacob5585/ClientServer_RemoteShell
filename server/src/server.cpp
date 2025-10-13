#include "../headers/server.h"

#include <cstring>
#include <iostream>
#include <ostream>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(uint port) : port(port), serverSocket(-1), state(false) {}

Server::~Server() {
    stop();
    close(serverSocket);
}

void Server::stop() {
    state = false;

    if (serverSocket != -1) {
        shutdown(serverSocket, SHUT_RDWR);
        close(serverSocket);
        serverSocket = -1;
    }
}

void Server::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    const int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) == -1) {
        perror("Socket failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) == -1) {
        perror("Socket failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on port: " << port << std::endl;
}

