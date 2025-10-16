#ifndef CLIENTSERVER_CLIENT_H
#define CLIENTSERVER_CLIENT_H

#include <string>
#include <sys/socket.h>
#include <atomic>

class Client {
    public:
        Client(const std::string address, uint port);
        ~Client();
        void stop();
        void start();

        std::atomic<bool> state;

    private:
        int clientSocket;
        uint port;
        std::string address;

        void control();
        std::string executeCommand(std::string &command);
        void receive(std::string &message);
        void send(std::string &message);
};

#endif //CLIENTSERVER_CLIENT_H