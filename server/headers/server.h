#ifndef Server_H
#define Server_H

#include <string>
#include <vector>
#include <sys/socket.h>
#include <atomic>
#include <thread>

class Server {
    public:
        Server(uint port);
        ~Server();
        void stop();
        void start();

        std::atomic<bool> state;

    private:
        int serverSocket;
        uint port;
        std::thread acceptThread; // Thread dedicated to accepting connections
        std::vector<std::thread> threads; // Vector to store all active threads

        void acceptConnections();
        void handleClient(int clientSocket);
};

#endif //Server_H