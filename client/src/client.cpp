#include "../headers/client.h"

#include <iostream>
#include <ostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>

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

    state = true;

    std::cout << "Connected: " << address.c_str() << ":" << port << std::endl;

    Client::control();
}

void Client::control() {
    std::string command;
    while (state) {
        std::cout << "looping" << std::endl;

        //TODO Call receive function
        receive(command);
        std::cout << "command: " << command << std::endl;

        //TODO Call executeCommand function
        //TODO Call sendOutput function
        sendOutput(clientSocket, command);
    }
}

std::string Client::executeCommand(std::string &command) {

}

void Client::receive(std::string &message) {
    // Sets len to the length of the incoming data
    uint len = 0;
    if (recv(clientSocket, &len, sizeof(len), 0) <= 0) {
        // std::cerr << "recv failed";
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
            // std::cerr << "recv failed";
            perror("recv");
            return;
        }
    }

    message.assign(buffer.begin(), buffer.end());
}

void Client::sendOutput(int serverSocket, std::string &message) {
    uint len = htonl(message.length());

    if (send(serverSocket, &len, sizeof(len), 0) == -1) {
        perror("send");
        return;
    }

    if (send(serverSocket, message.c_str(), message.length(), 0) == -1) {
        perror("send");
        return;
    }
    std::cout << "output: " << message << std::endl;
}