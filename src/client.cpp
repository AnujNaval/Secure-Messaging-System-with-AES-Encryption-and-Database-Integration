#include <iostream>
#include <fstream>
#include <sstream>
#include "encryption.h"
#include "utils.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

void send_message(const std::string& message) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) throw std::runtime_error("Socket creation failed");

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        throw std::runtime_error("Invalid address");

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("Connection failed");

    unsigned char key[32] = {0};
    unsigned char iv[16];
    generate_random_iv(iv, sizeof(iv));

    unsigned char ciphertext[1024];
    size_t ciphertext_len;

    aes_encrypt(reinterpret_cast<const unsigned char*>(message.c_str()), message.size(), key, iv, ciphertext, ciphertext_len);

    unsigned char buffer[2048];
    memcpy(buffer, iv, 16);
    memcpy(buffer + 16, ciphertext, ciphertext_len);

    send(sock, buffer, 16 + ciphertext_len, 0);
    std::cout << "Message sent successfully!\n";

    close(sock);
}

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    try {
        // Read the contents of a file
        std::string filename = "messages.txt";  // Change the file path as needed
        std::string file_content = read_file(filename);

        // Send the file content
        send_message(file_content);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
