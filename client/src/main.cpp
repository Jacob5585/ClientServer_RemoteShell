#include <iostream>
#include <ostream>
#include <string>

int main(int argc, char *argv[]) {
    std::string address = "127.0.0.1";

    if (argc > 1) {
        try {
            address = argv[1];
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
    }
}