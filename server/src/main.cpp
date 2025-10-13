#include <iostream>
#include <ostream>
#include <string>
#include "../headers/server.h"

int main(int argc, char *argv[]) {
    Server* serverPrt = nullptr;
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
        server.start();

        std::cout << "Server Starting" << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "" << e.what() << std::endl;
        return 1;
    }
}