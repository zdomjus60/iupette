#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include "json.hpp" // Path to the header-only JSON library

using json = nlohmann::json;

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    std::cout << "Server starting on port " << PORT << std::endl;

    // 1. Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Set socket options to reuse address (helps with quick restarts)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt failed." << std::endl;
        close(server_fd);
        return 1;
    }

    // 2. Bind socket to address and port
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Failed to bind socket to port " << PORT << std::endl;
        close(server_fd);
        return 1;
    }

    // 3. Listen for incoming connections
    if (listen(server_fd, 3) < 0) { // Max 3 pending connections
        std::cerr << "Failed to listen on socket." << std::endl;
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening for connections..." << std::endl;

    while (true) {
        // 4. Accept a client connection
        int client_socket;
        socklen_t addrlen = sizeof(address);
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket < 0) {
            std::cerr << "Failed to accept client connection." << std::endl;
            continue;
        }
        std::cout << "Client connected." << std::endl;

        bool client_connected = true;
        while (client_connected) {
            try {
                // First, read the 4-byte message length
                uint32_t net_msg_len;
                ssize_t bytes_read = recv(client_socket, &net_msg_len, sizeof(net_msg_len), 0);
                if (bytes_read == 0) {
                    std::cout << "Client disconnected gracefully." << std::endl;
                    client_connected = false;
                    break;
                } else if (bytes_read < 0) {
                    std::cerr << "Error receiving message length from client." << std::endl;
                    client_connected = false;
                    break;
                }
                uint32_t msg_len = ntohl(net_msg_len); // Convert from network byte order

                std::vector<char> msg_buffer(msg_len);
                bytes_read = recv(client_socket, msg_buffer.data(), msg_len, 0);
                if (bytes_read == 0) {
                    std::cout << "Client disconnected gracefully." << std::endl;
                    client_connected = false;
                    break;
                } else if (bytes_read < 0) {
                    std::cerr << "Error receiving message data from client." << std::endl;
                    client_connected = false;
                    break;
                }

                std::string received_data(msg_buffer.begin(), msg_buffer.end());
                std::cout << "Received from client: " << received_data << std::endl;

                json received_json = json::parse(received_data);
                std::cout << "Parsed JSON: " << received_json.dump(4) << std::endl;

                // Construct a simple JSON response
                json response_json;
                response_json["status"] = "received";
                response_json["original_message"] = received_json;

                std::string response_str = response_json.dump();
                uint32_t resp_msg_len = response_str.length();
                uint32_t net_resp_msg_len = htonl(resp_msg_len); // Convert to network byte order

                send(client_socket, &net_resp_msg_len, sizeof(net_resp_msg_len), 0);
                send(client_socket, response_str.c_str(), resp_msg_len, 0);
                std::cout << "Sent response: " << response_str << std::endl;

            } catch (const json::parse_error& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
                std::string error_response = "{\"error\": \"Invalid JSON received\"}";
                uint32_t err_msg_len = error_response.length();
                uint32_t net_err_msg_len = htonl(err_msg_len);
                send(client_socket, &net_err_msg_len, sizeof(net_err_msg_len), 0);
                send(client_socket, error_response.c_str(), err_msg_len, 0);
            } catch (const std::exception& e) {
                std::cerr << "Error processing message: " << e.what() << std::endl;
                std::string error_response = "{\"error\": \"Server internal error\"}";
                uint32_t err_msg_len = error_response.length();
                uint32_t net_err_msg_len = htonl(err_msg_len);
                send(client_socket, &net_err_msg_len, sizeof(net_err_msg_len), 0);
                send(client_socket, error_response.c_str(), err_msg_len, 0);
            }
        }
        // Close the client socket after its inner loop finishes
        close(client_socket); // Keep client socket open for multiple requests
    }

    // Close the server socket (this part is technically unreachable in the infinite loop)
    close(server_fd);
    std::cout << "Server shut down." << std::endl;

    return 0;
}