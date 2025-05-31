#include "Solver_API.hh"

void SolverAPI::define_socket() {
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==0) {
        perror("Socket creation failed.");  
    };

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
}

void SolverAPI::bind_socket() {
    if(bind(sockfd, (struct sockaddr*)&address, sizeof(address))<0) {
        perror("Binding failure.");
    } else {
        std::cout << "Binding succesful." << std::endl;
    }
}

void SolverAPI::listen_socket() {
    if(listen(sockfd, 3)<0) {
        perror("Listening failure.");
    }

    if((sock_conn = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen))<0) {
        perror("Connection failure.");   
    } else {
        std::cout << "Connection succesful." << std::endl;
    }
}

void SolverAPI::destroy() {
    loop = false;
    close(sock_conn);
    close(sockfd);
}

void SolverAPI::serve() {
    ODESolver solver;
    SolverAPI::define_socket();
    SolverAPI::bind_socket();

    while(loop) {
        SolverAPI::listen_socket();
        if((recv_size=recv(sock_conn, message_recv, sizeof(message_recv)-1, 0))!=0) {
            int shift = 0;
            int addshift = 0;
            std::string message = "";
            message_recv[recv_size] = '\0';
            std::cout << message_recv << std::endl; 
            
            // Do something w message
            
            std::string solution = std::to_string(solver.solve());
            send(sock_conn, solution.c_str(), solution.size(), 0);
            send(sock_conn, SUCCESS, sizeof(SUCCESS), 0);
            memset(message_recv, 0, sizeof(message_recv));
        } else {
            std::cerr << "Error recieving message/s" << std::endl;
        }
    }
}