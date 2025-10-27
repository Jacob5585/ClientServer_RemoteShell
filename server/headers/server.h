#ifndef Server_H
#define Server_H

#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <atomic>
#include <thread>
#include <mutex>

struct clientInfo {
    std::string ip;
    std::thread thread;
};

class Server {
    public:
        Server(uint port);
        ~Server();
        void stop();
        void start();
        void command();

        std::atomic<bool> state;

    private:
        int serverSocket;
        uint port;
        std::thread acceptThread; // Thread dedicated to accepting connections from cleints
        std::mutex clientMutex;
        std::map<int, clientInfo> clients;

        void acceptConnections();
        void sendCommand(int clientSocket, const std::string &command);
        void recvOutput(int clientSocket, std::string &output);
};

#endif //Server_H