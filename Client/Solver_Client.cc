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

void SolverClient::request(std::string function, std::map<char, double> variable_map, char primary_variable, double integrator_target) {
    int function_len = htonl(function.size());
    send(sockfd, &function_len, sizeof(function_len), 0);
    send(sockfd, function.c_str(), function.size(), 0);
    send(sockfd, &primary_variable, sizeof(primary_variable), 0);
    send(sockfd, &integrator_target, sizeof(integrator_target), 0);

    int map_count = variable_map.size();
    send(sockfd, &map_count, sizeof(map_count), 0);

    std::vector<std::pair<char, double>> map_buffer(variable_map.begin(), variable_map.end());
    send(sockfd, map_buffer.data(), map_buffer.size()*sizeof(std::pair<char, double>), 0);

    if(recv(sockfd, message_recv, sizeof(message_recv) - 1, 0)!=0) {
        std::cout << "Solution from server: " << message_recv << std::endl;
        memset(message_recv, 0, sizeof(message_recv));
    } else {
        std::cerr << "Error recieving message/s" << std::endl;
    }
}