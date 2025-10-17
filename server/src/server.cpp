#include "../headers/server.h"

#include <cstring>
#include <iostream>
#include <ostream>
#include <thread>
#include <arpa/inet.h>
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
        std::cout << "Client connected: " << inet_ntoa(clientAddress.sin_addr) << std::endl;

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
    std::cout << "Client connected" << std::endl;

    sendCommand(clientSocket, "ls -la; pwd; whoami;fakecommand");

    std::string output = "";
    recvOutput(clientSocket, output);
    std::cout << output << std::endl;
}

void Server::sendCommand(int clientSocket, const std::string &command) {
    uint len = htonl(command.length());

    // Send message length
    if (send(clientSocket, &len, sizeof(len), 0) == -1) {
        perror("send");
        return;
    }

    // Send message
    if (send(clientSocket, command.c_str(), command.length(), 0) == -1) {
        perror("send");
        return;
    }
}

void Server::recvOutput(int clientSocket, std::string &output) {
    // Sets len to the length of the incoming data
    uint len = 0;
    if (recv(clientSocket, &len, sizeof(len), 0) <= 0) {
        perror("recv");
        return;
    }
    len = ntohl(len);

    std::vector<char> buffer(len); // buffer the length of the data
    ssize_t bytesRead = 0;

    while (bytesRead < len) {
        // Read the received message into the buffer, and stores the number of bytes read into bytesRead
        bytesRead = recv(clientSocket, buffer.data() + bytesRead, len - bytesRead, 0);

        if (bytesRead <= 0) {
            perror("recv");
            return;
        }
    }

    output.assign(buffer.begin(), buffer.end());
}