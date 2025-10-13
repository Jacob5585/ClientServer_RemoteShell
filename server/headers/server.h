#ifndef Server_H
#define Server_H

#include <string>
#include <sys/socket.h>
#include <atomic>

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
};

#endif //Server_H