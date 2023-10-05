#include "irc_header.hpp"

// int main(int argc, char const *argv[])
//
// {
// 	if (argc == 3)
// 	{

// 		return (0);
// 	}
// 	else
// 	{
// 		std::cout << "Try ./ircserv [port] [password] :)" << std::endl;
// 		return (1);
// 	}
//     return (0);
// }

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int PORT = 6666;
const int BUFFER_SIZE = 1024;

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind to a specific IP address and port
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Waiting for a connection..." << std::endl;

    // Accept a connection from a client
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to a client." << std::endl;

    // Read data from the client and process it
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead < 0)
        {
            perror("recv");
            exit(EXIT_FAILURE);
        } else if (bytesRead == 0)
        {
            std::cout << "Connection closed by the client." << std::endl;
            break;
        }

        std::cout << "Received: " << buffer << std::endl;

        // Add your processing logic here
    }

    // Close the sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
