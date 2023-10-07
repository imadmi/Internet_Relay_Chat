#include "../../headers/irc_header.hpp"

Irc::Irc(int port, char *password)
{
    _passWord = password;
    _port = port;
    _serverName = ":MSN ";

    createSocket();
    bindSocket();
    listeningToClients();

    std::cout << GREEN << "IRC Server is running on port " << _port << RESET << std::endl;

}

void Irc::createSocket()
{
    //  create server socket and initialize it
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    _server_addr.sin_addr.s_addr = INADDR_ANY;
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port);
}

void Irc::bindSocket()
{
    // here we allow the server socket fd to be reusable
    int optval = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // here we set the server socket to be nonbloking
    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // bind the server host address
    if (bind(_serverSocket, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

void Irc::listeningToClients()
{
    // listening for the clients (wait for incoming connection from the client)
    if (listen(_serverSocket, MAX_CLIENTS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}


// establish connections and start communication
void Irc::runServer()
{
    pollfd serverPollfd;
    serverPollfd.fd = _serverSocket;
    serverPollfd.events = POLLIN;

    _pollfds.push_back(serverPollfd);

    while (true)
    {
        int activity = poll(&_pollfds[0], _pollfds.size(), -1);

        if (activity < 0)
        {
            std::cerr << RED << "Error in poll: " << strerror(errno) << RESET << std::endl;
            continue;
        }

        if (activity == 0)
        {
            // No activity, continue waiting
            continue;
        }

        // Check for incoming connections on the server socket
        if (_pollfds[0].revents & POLLIN)
        {
            addClient();
        }

// Handle activity on client sockets
for (size_t i = 1; i < _pollfds.size(); ++i)
{
    if (_pollfds[i].revents & (POLLIN | POLLHUP))
    {
        if (_pollfds[i].revents & POLLIN)
        {
            char buffer[BUFFER_SIZE];
            memset(buffer, 0, sizeof(buffer));

            int bytesRead = recv(_pollfds[i].fd, buffer, sizeof(buffer) - 1, 0);

            if (bytesRead <= 0)
            {
                // Handle client disconnection or error
                if (bytesRead == 0)
                {
                    // Client disconnected
                    std::cout << "Client " << _pollfds[i].fd << " disconnected." << std::endl;
                }
                else
                {
                    std::cerr << RED << "Error reading from client " << _pollfds[i].fd << ": " << strerror(errno) << RESET << std::endl;
                }

                // Close the socket and remove it from the poll list
                close(_pollfds[i].fd);
                _pollfds.erase(_pollfds.begin() + i);
                // Decrement i to account for the removed socket
                --i;
            }
            else
            {
                // Process the received message
                std::string message(buffer);
                std::cout << "Received from client " << _pollfds[i].fd << ": " << message << std::endl;

                // Handle the received message here
                // You can implement your message processing logic within this block
            }
        }
        else if (_pollfds[i].revents & POLLHUP)
        {
            // Handle client disconnection
            std::cout << "Client " << _pollfds[i].fd << " disconnected." << std::endl;

            // Close the socket and remove it from the poll list
            close(_pollfds[i].fd);
            _pollfds.erase(_pollfds.begin() + i);
            // Decrement i to account for the removed socket
            --i;
        }
    }
}

    }
}

void Irc::addClient()
{
    // Accept a new client connection
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    _newSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (_newSocket < 0)
    {
        std::cerr << RED << "Error accepting client connection: " << strerror(errno) << RESET << std::endl;
        return;
    }

    // Set the new socket to non-blocking mode
    int flags = fcntl(_newSocket, F_GETFL, 0);
    if (flags < 0)
    {
        std::cerr << RED << "Error getting socket flags: " << strerror(errno) << RESET << std::endl;
        close(_newSocket);
        return;
    }
    flags |= O_NONBLOCK;
    if (fcntl(_newSocket, F_SETFL, flags) < 0)
    {
        std::cerr << RED << "Error setting socket to non-blocking mode: " << strerror(errno) << RESET << std::endl;
        close(_newSocket);
        return;
    }

	Client new_client;

    pollfd client_pollfd = {_newSocket, POLLIN | POLLOUT, 0};
	_pollfds.push_back(client_pollfd);

	_clients.insert(std::pair<std::string, Client>("static_cast<std::string>_newSocket", new_client)); // insert a new nod in client map with the fd as key
	std::cout << PURPLE << "[Server] Added client #" << _newSocket << " successfully" << RESET << std::endl;
}