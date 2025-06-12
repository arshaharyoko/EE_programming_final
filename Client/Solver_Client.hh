#ifndef H_SOLVER_CLIENT
#define H_SOLVER_CLIENT
#define PORT 8000

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <vector>

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
        int request(std::string function, std::map<char, double> variable_map, char primary_variable, double integrator_target);
};

#endif
