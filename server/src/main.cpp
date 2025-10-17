#include <iostream>
#include <ostream>
#include <string>
#include <csignal>
#include "../headers/server.h"

static Server* serverPrt = nullptr;

void handleSignal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout <<  "\nCaught signal " << signal << ". Shutting down server" << std::endl;

        if (serverPrt) {
            serverPrt->stop();
        }
    }
}

int main(int argc, char *argv[]) {
    uint port = 8080;

    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
    }

    Server server(port);
    serverPrt = &server;

    try {
        std::signal(SIGINT, handleSignal);
        std::signal(SIGTERM, handleSignal);
        server.start();
        server.command();
    }
    catch (const std::exception &e) {
        std::cerr << "" << e.what() << std::endl;
        return 1;
    }
}