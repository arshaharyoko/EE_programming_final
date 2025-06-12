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
}

void SolverAPI::accept_socket_connection() {
    if((sock_conn = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen))<0) {
        perror("Connection failure.");   
    } else {
        std::cout << "Connection succesful." << std::endl;
    }
}

void SolverAPI::destroy_socket_connection() {
    close(sock_conn);
}

void SolverAPI::destroy() {
    close(sock_conn);
    close(sockfd);
}

void SolverAPI::serve() {
    ODESolver solver;
    SolverAPI::define_socket();
    SolverAPI::bind_socket();
    SolverAPI::listen_socket();

    std::ofstream log_file("records.bin", std::ios::binary | std::ios::app);
    if(!log_file) {
        std::cerr << "Failed to open records.bin\n";
    }

    while(loop) {
        SolverAPI::accept_socket_connection();

        int function_len;
        if(recv(sock_conn, &function_len, sizeof(function_len), 0)<=0) {
            throw std::runtime_error("Error recieving function length.");
        }
        function_len = ntohl(function_len);

        std::string function(function_len, '\0');
        if(recv(sock_conn, &function[0], function_len, 0)<=0) {
            throw std::runtime_error("Error recieving function string.");
        }

        char primary_variable;
        if(recv(sock_conn, &primary_variable, sizeof(primary_variable), 0)<=0) {
            throw std::runtime_error("Error recieving primary variable character.");
        }

        double integration_target;
        if(recv(sock_conn, &integration_target, sizeof(integration_target), 0)<=0) {
            throw std::runtime_error("Error recieving integration target floating point integer.");
        }

        int map_count;
        if(recv(sock_conn, &map_count, sizeof(map_count), 0)<=0) {
            throw std::runtime_error("Error recieving variable map size.");
        }

        std::vector<std::pair<char, double>> map_buffer(map_count);
        size_t total_bytes = map_count*sizeof(std::pair<char, double>);
        if(recv(sock_conn, map_buffer.data(), total_bytes, 0)<=0) {
            throw std::runtime_error("Error receiving variable map.");
        }

        std::map<char, double> variable_map(map_buffer.begin(), map_buffer.end());
        std::array<double, 2> solution = solver.rk2solve(function, variable_map, primary_variable, integration_target);

        std::string solution_str = std::to_string(solution[0]);
        send(sock_conn, solution_str.c_str(), solution_str.size(), 0);
        SolverAPI::destroy_socket_connection();

        std::string rec = "{";
        rec += "\"function\":\"" + function + "\",";
        rec += "\"primary_variable\":\"" + std::string(1, primary_variable) + "\",";
        rec += "\"integration_target\":" + std::to_string(integration_target) + ",";
        rec += "\"variable_map\":{";
        for (auto& kv : variable_map) {
            rec += "\"" + std::string(1, kv.first) + "\":" + std::to_string(kv.second) + ",";
        }
        if (!variable_map.empty()) rec.pop_back();
        rec += "},";
        rec += "\"solution\":" + std::to_string(solution[0]);
        rec += "}";

        uint32_t len = static_cast<uint32_t>(rec.size());
        log_file.write(reinterpret_cast<char*>(&len), sizeof(len));
        log_file.write(rec.data(), rec.size());
        log_file.flush();
    }

    SolverAPI::destroy();
}