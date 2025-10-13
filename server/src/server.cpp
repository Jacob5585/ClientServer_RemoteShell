#include "../headers/server.h"

#include <cstring>
#include <iostream>
#include <ostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(uint port) : port(port), serverSocket(-1), state(false) {}

Server::~Server() {
    stop();
}

void Server::stop() {
    state = false;

    if (serverSocket != -1) {
        shutdown(serverSocket, SHUT_RDWR);
        close(serverSocket);
        serverSocket = -1;
    }

    if (acceptThread.joinable()) {
        acceptThread.join();
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void Server::start() {
    // Setup socket, bind to port, then listen for incoming connection

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

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Socket failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 10) == -1) {
        perror("Socket failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on port: " << port << std::endl;

    state = true;

    acceptThread = std::thread(&Server::acceptConnections, this);
}

void Server::acceptConnections() {
    while(state) {
        sockaddr_in clientAddress;
        socklen_t clientLength = sizeof(clientAddress);

        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);

        if (clientSocket == -1) {
            if (errno == EINTR && !state) {
                break;
            }

            if (state) {
                continue;
            }
        }

        if (state) {
            threads.emplace_back(&Server::handleClient, this, clientSocket);
        }
        else {
            close(clientSocket);;
        }
    }
}

void Server::handleClient(int clientSocket) {

}