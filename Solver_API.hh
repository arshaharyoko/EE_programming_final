#ifndef H_SOLVER_API
#define H_SOLVER_API
#define FAILED "Failed"
#define SUCCESS "Success"
#define PORT 8000

#include <cstdio>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <map>

#include "ODE_Solver.hh"

class SolverAPI {
    private:
        int sockfd, sock_conn;
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        int opt = 1;
        int recv_size;
        bool loop = true;

    protected:
        void define_socket();
        void bind_socket();
        void listen_socket();

    public:
        void destroy();
        void serve();
};

#endif