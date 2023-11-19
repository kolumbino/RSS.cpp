#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

const int PORT = 8002;  // Port number for RSS server

// Function to process the task
void processTask(char* task) {
    int complexity = 0;

    // Extracting task complexity from the received task string
    char* token = strtok(task, ":");
    while (token != nullptr) {
        complexity = atoi(token);
        token = strtok(nullptr, ":");
    }

    // Simulate processing the task
    std::cout << "Processing task with complexity: " << complexity << std::endl;
    Sleep(complexity * 1000);  // Simulate task processing time, Sleep takes milliseconds

    std::cout << "Task completed" << std::endl;
}

int main() {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    SOCKET server_fd = INVALID_SOCKET, new_socket = INVALID_SOCKET;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Replace 'true' with a condition for a proper server shutdown
    while (true) {
        if (listen(server_fd, 3) == SOCKET_ERROR) {
            std::cerr << "listen failed with error: " << WSAGetLastError() << std::endl;
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }

        new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (new_socket == INVALID_SOCKET) {
            std::cerr << "accept failed with error: " << WSAGetLastError() << std::endl;
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }

        char task[1024];
        if (recv(new_socket, task, sizeof(task), 0) > 0) {
            std::cout << "Received task: " << task << std::endl;
            processTask(task);

            char message[] = "Task completed";
            send(new_socket, message, strlen(message) + 1, 0); // +1 for null terminator
        }

        closesocket(new_socket);
    }

    // Clean up Winsock
    WSACleanup();
    return 0;
}
