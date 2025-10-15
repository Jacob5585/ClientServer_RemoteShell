#ifndef CLIENTSERVER_CLIENT_H
#define CLIENTSERVER_CLIENT_H

#include <string>
#include <sys/socket.h>
#include <atomic>

class Client {
    public:
        Client(std::string address, uint port);
        ~Client();
        void stop();
        void start();

        std::atomic<bool> state;

    private:
        int clientSocket;
        uint port;
        std::string address;

};

#endif //CLIENTSERVER_CLIENT_H