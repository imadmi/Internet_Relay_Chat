#include "irc_header.hpp"

int main(int argc, char const *argv[])
{

	if (argc == 3)
	{
        int server_sock_fd, connected_sock_fd;
        struct sockaddr_in serverAddr, clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        char buffer[BUFFER_SIZE];

        // Create socket
        server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_sock_fd < 0)
        {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        // Bind to a specific IP address and port

        // memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(std::atoi(argv[1]));
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(server_sock_fd, (struct sockaddr*)&serverAddr, clientLen) < 0)
        {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        // Listen for incoming connections
        if (listen(server_sock_fd, 2) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        std::cout << "Waiting for a connection..." << std::endl;

        // Accept a connection from a client
        connected_sock_fd = accept(server_sock_fd, (struct sockaddr*)&clientAddr, &clientLen);
        if (connected_sock_fd < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        std::cout << "Connected to a client." << std::endl;

        // Read data from the client and process it
        while (true)
        {
            memset(buffer, 0, BUFFER_SIZE);
            int bytesRead = recv(connected_sock_fd, buffer, sizeof(buffer), 0);
            if (bytesRead < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
            else if (bytesRead == 0)
            {
                // std::cout << "Connection closed by the client." << std::endl;
                // break;
            }

            std::cout << "Received: " << buffer << std::endl;

            // Add your processing logic here
        }

        // Close the sockets
        close(connected_sock_fd);
        close(server_sock_fd);
		
        return (EXIT_SUCCESS);
	}
	else
	{
		std::cout << "Try ./ircserv [port] [password] :)" << std::endl;
		return (EXIT_FAILURE);
	}
    return (EXIT_SUCCESS);
}
