#include "../headers/irc_header.hpp"

int main(int ac, char const *argv[])
{
    if (ac != 3)
    {
        std::cerr << "try ./ircserv [port] [password]" << std::endl;
        exit(EXIT_FAILURE);
    }

    int server_sock_fd;
    std::vector<int> connected_sock_fd(MAX_CLIENTS);
    sockaddr_in serverAddr, clientAddr;
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
    serverAddr.sin_port = htons(std::stoi(argv[1]));
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    // to prevent the adress already in use error
    int reuse = 1;
    if (setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
    }
    if (bind(server_sock_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
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
        fds[0].events = POLLIN; // POLLIN : There is data to read.

        for (int i = 0; i < MAX_CLIENTS; ++i)
        {
            if (connected_sock_fd[i] != -1)
            {
                fds[activeClients + 1].fd = connected_sock_fd[i];
                fds[activeClients + 1].events = POLLIN;
                activeClients++;
            }
        }
        std::cout << "active clients " << activeClients << std::endl;

        // Use poll() to wait for events on server and client sockets
        
        int pollResult = poll(fds, activeClients + 1, -1); //  -1 : no timeout

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
                        
                        irc.add_new_client(connected_sock_fd[i]);

                        std::cout << GREEN << "Accepted connection from client # " << i + 1 << RESET << std::endl;
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
                        std::cout << "Connection closed by client # " << i + 1 << std::endl;
                        close(connected_sock_fd[i]);
                        connected_sock_fd[i] = -1;
                    }
                    else
                    {
                        std::cout << "Received from client # " << i + 1 << ": " << buffer << std::endl;
                    }
                }
            }
        }
    }

    close(server_sock_fd);

    return 0;
}
