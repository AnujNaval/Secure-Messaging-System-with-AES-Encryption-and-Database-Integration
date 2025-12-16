#include <iostream>
#include <cstring>
#include "encryption.h"
#include "database.h"
#include "utils.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
// #include "server.h"

void start_server() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) throw std::runtime_error("Socket creation failed");

    struct sockaddr_in address;
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        throw std::runtime_error("Set socket options failed");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw std::runtime_error("Socket bind failed");

    if (listen(server_fd, 3) < 0)
        throw std::runtime_error("Socket listen failed");

    std::cout << "Server is listening on port 8080...\n";

    setup_database();

    while (true) {
        int addrlen = sizeof(address);
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) throw std::runtime_error("Client accept failed");

        unsigned char buffer[2048] = {0};
        int bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            std::cerr << "Failed to read data from client.\n";
            close(client_socket);
            continue;
        }

        unsigned char key[32] = {0};
        unsigned char iv[16];
        memcpy(iv, buffer, 16);
        unsigned char decrypted_message[1024];
        size_t decrypted_len;

        try {
            aes_decrypt(buffer + 16, bytes_read - 16, key, iv, decrypted_message, decrypted_len);
            decrypted_message[decrypted_len] = '\0';

            store_message("EncryptedUser", std::string(reinterpret_cast<char*>(decrypted_message)), iv);
            std::cout << "Message stored successfully.\n";
        } catch (const std::exception& e) {
            std::cerr << "Decryption failed: " << e.what() << '\n';
        }

        close(client_socket);
    }

    close(server_fd);
}

int main() {
    try {
        start_server();  // Function in your server.cpp file
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

