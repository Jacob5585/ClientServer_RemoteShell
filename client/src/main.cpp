#include <iostream>
#include <ostream>
#include <string>
#include <csignal>
#include "../headers/client.h"

static Client* clientPrt = nullptr;

void handleSignal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout <<  "\nCaught signal " << signal << ". Shutting down server" << std::endl;

        if (clientPrt) {
            clientPrt->stop();
        }
    }
}

int main(int argc, char *argv[]) {
    std::string address = "127.0.0.1";
    uint port = 8080;

    if (argc > 1) {
        try {
            address = argv[1];
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
    }

    if (argc > 2) {
        try {
            port = std::stoi(argv[2]);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
    }

    Client client(address, port);
    clientPrt = &client;

    try {
        std::signal(SIGINT, handleSignal);
        client.start();

        //Keep server running
        // while (clientPrt->state.load()) {
            // std::chrono::milliseconds(500);
        // }
    }
    catch (const std::exception &e) {
        std::cerr << "" << e.what() << std::endl;
        return 1;
    }
}