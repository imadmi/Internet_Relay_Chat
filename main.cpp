#include "irc_header.hpp"




int main(int argc, char const *argv[])
{
    int server_sock_fd, connected_sock_fd[MAX_CLIENTS];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    
    server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::atoi(argv[1]));
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    
    if (listen(server_sock_fd, MAX_CLIENTS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Waiting for a connection..." << std::endl;

    
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        connected_sock_fd[i] = -1; 
    }

    while (true)
    {
        struct pollfd fds[MAX_CLIENTS + 1]; 
        int activeClients = 0;

        
        fds[0].fd = server_sock_fd;
        fds[0].events = POLLIN;

        
        for (int i = 0; i < MAX_CLIENTS; ++i)
        {
            if (connected_sock_fd[i] != -1)
            {
                fds[activeClients + 1].fd = connected_sock_fd[i];
                fds[activeClients + 1].events = POLLIN;
                activeClients++;
            }
        }

        // Use poll() to wait for events on server and client sockets
        
        int pollResult = poll(fds, activeClients + 1, -1);

        if (pollResult < 0)
        {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if (pollResult > 0)
        {
            
            if (fds[0].revents & POLLIN)
            {
                for (int i = 0; i < MAX_CLIENTS; ++i)
                {
                    if (connected_sock_fd[i] == -1)
                    {
                        connected_sock_fd[i] = accept(server_sock_fd, (struct sockaddr*)&clientAddr, &clientLen);
                        if (connected_sock_fd[i] < 0)
                        {
                            perror("accept");
                            continue;
                        }
                        std::cout << "Accepted connection from client #" << i + 1 << std::endl;
                        break;
                    }
                }
            }

            for (int i = 0; i < MAX_CLIENTS; ++i)
            {
                if (connected_sock_fd[i] != -1 && fds[i + 1].revents & POLLIN)
                {
                    memset(buffer, 0, BUFFER_SIZE);
                    int bytesRead = recv(connected_sock_fd[i], buffer, sizeof(buffer), 0);
                    if (bytesRead < 0)
                    {
                        perror("recv");
                    }
                    else if (bytesRead == 0)
                    {
                        std::cout << "Connection closed by client #" << i + 1 << std::endl;
                        close(connected_sock_fd[i]);
                        connected_sock_fd[i] = -1;
                    }
                    else
                    {
                        std::cout << "Received from client #" << i + 1 << ": " << buffer << std::endl;
                        
                    }
                }
            }
        }
    }

    
    close(server_sock_fd);

    return 0;
}
