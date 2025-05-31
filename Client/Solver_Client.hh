#ifndef H_SOLVER_CLIENT
#define H_SOLVER_CLIENT
#define PORT 8000

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>

class SolverClient {
    private:
        int sockfd, sock_conn;
        struct sockaddr_in address;
        char message_recv[100];
        std::string message;
        bool loop = 1;

    public:
        void define_socket();
        void connect_socket();
        void destroy();
        void request();
};

#endif