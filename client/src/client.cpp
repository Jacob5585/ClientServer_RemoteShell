#include "../headers/client.h"

#include <iostream>
#include <ostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

Client::Client(const std::string address, uint port) : address(address), port(port), clientSocket(-1), state(false) {}

Client::~Client() {
    stop();
}

void Client::stop() {
    state = false;

    if (clientSocket != -1) {
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
        clientSocket = -1;
    }
}

void Client::start() {
    // setup socket, connect to server

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(address.c_str());

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress))) {
        perror("Connect");
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected: " << address.c_str() << ":" << port << std::endl;

}

