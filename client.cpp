#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

// Server information structure
struct ServerInfo
{
    int port;
    std::string address;
    std::string schedulingAlgorithm;
};

int main()
{
    WSADATA wsaData;
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed." << std::endl;
        return 1;
    }

    // List of servers with their port numbers and scheduling algorithms
    std::vector<ServerInfo> servers = {
        {8002, "127.0.0.1", "RRS"},
        // Add other servers if needed
    };

    for (const auto& server : servers)
    {
        std::cout << "Connecting to " << server.schedulingAlgorithm << " server on port " << server.port << std::endl;

        SOCKET sock = INVALID_SOCKET;
        struct sockaddr_in serv_addr;

        // Creating the socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        {
            std::cerr << "Socket creation error: " << WSAGetLastError() << std::endl;
            continue;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(server.port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, server.address.c_str(), &serv_addr.sin_addr) <= 0)
        {
            std::cerr << "Invalid address/ Address not supported" << std::endl;
            closesocket(sock);
            continue;
        }

        // Connect to the server
        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
        {
            std::cerr << "Connection to server failed: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            continue;
        }

        // Example tasks distribution
        char task[1024] = "10:10:1"; // Simplified example of a task
        std::cout << "Sending task: " << task << " to " << server.schedulingAlgorithm << " server" << std::endl;
        send(sock, task, strlen(task), 0); // Don't send the pointer's address

        char buffer[1024] = { 0 };
        // Wait for acknowledgment
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            std::cout << server.schedulingAlgorithm << " Server: " << buffer << std::endl;
        }

        closesocket(sock);
    }

    // Cleanup Winsock
    WSACleanup();
    return 0;
}
