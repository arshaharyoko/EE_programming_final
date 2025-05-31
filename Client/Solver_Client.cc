#include "Solver_Client.hh"

void SolverClient::define_socket() {
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0) {
        perror("Socket creation failed.");  
    } else {
        std::cout << "Socket created." << std::endl;   
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0) {
        std::cerr << "Invalid address." << std::endl;
    }
}

void SolverClient::connect_socket() {
    if(connect(sockfd, (struct sockaddr*)&address, sizeof(address))<0) {
        perror("Connection failure.");
    } else {
        std::cout << "Connected successfully." << std::endl;
    }
}

void SolverClient::destroy() {
    loop = 0;
    close(sock_conn);
    close(sockfd);
}

void SolverClient::request() {
    std::cout << "Message: ";
    std::getline(std::cin, message);

    send(sockfd, message.c_str(), message.size(), 0);
    if(recv(sockfd, message_recv, sizeof(message_recv) - 1, 0)!=0) {
        std::cout << "Server: " << message_recv << std::endl;
        memset(message_recv, 0, sizeof(message_recv));
    } else {
        std::cerr << "Error recieving message/s" << std::endl;
    }
}