#include "../../headers/irc_header.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

Irc::Irc(int port, char *password)
{
    _passWord = password;
    _port = port;
    _serverName = ":MSN ";

    createSocket();
    bindSocket();
    listeningToClients();

    std::cout << BLACK << "IRC Server is running on port : " << _port << RESET << std::endl;
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
            std::cerr << RED << "Error in poll" << RESET << std::endl;
            continue;
        }
        if (activity == 0)
        {
            continue; // No activity, continue waiting
        }
        // Check for incoming connections on the server socket
        if (_pollfds[0].revents & POLLIN)
        {
            addClient();
        }
        Handle_activity();
        // print_map();
    }
}

void Irc::addClient()
{
    // Accept a new client connection
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    _newSocket = accept(_serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

    if (_newSocket < 0)
        printc("Error accepting client connection", RED, 1);

    Client new_client;

    pollfd client_pollfd = {_newSocket, POLLIN | POLLOUT, 0};
    _pollfds.push_back(client_pollfd);

    _clients.insert(std::pair<std::string, Client>(std::to_string(_newSocket), new_client)); // insert a new nod in client map with the fd as key
    std::cout << GREEN << "[Server] Added client #" << _newSocket << " successfully" << RESET << std::endl;

    // send welcome msg to the client
    std::string welcomeMsg;
    welcomeMsg = "\033[0;32mClient nbr #";
    send(_newSocket, welcomeMsg.c_str(), strlen(welcomeMsg.c_str()), 0);
    send(_newSocket, std::to_string(_newSocket).c_str(), strlen(std::to_string(_newSocket).c_str()), 0);
    welcomeMsg = " connected...\n\033[0m";
    send(_newSocket, welcomeMsg.c_str(), strlen(welcomeMsg.c_str()), 0);
}

void Irc::printc(std::string msg, std::string color, int ex)
{
    std::cout << color << msg << RESET << std::endl;
    if (ex)
        exit(EXIT_SUCCESS);
}

void Irc::Handle_activity()
{
    for (size_t i = 1; i < _pollfds.size(); ++i)
    {
        if (_pollfds[i].revents & POLLIN)
        {
            char buffer[BUFFER_SIZE];
            memset(buffer, 0, sizeof(buffer));

            int bytesRead = recv(_pollfds[i].fd, buffer, BUFFER_SIZE, 0);

            if (bytesRead <= 0)
            {
                // Handle client disconnection or error
                if (bytesRead == 0)
                {
                    std::cout << YELLOW << "Client " << _pollfds[i].fd << " disconnected." << RESET << std::endl;
                    std::cout << PURPLE << "Total clients is : " << _pollfds.size() - 2 << RESET << std::endl;
                    close(_pollfds[i].fd);
                    _pollfds.erase(_pollfds.begin() + i);
                    _clients.erase(std::to_string(_pollfds[i].fd));
                    --i; // Decrement i to account for the removed socket
                }
                else
                {
                    std::cerr << RED << "Error reading from client " << _pollfds[i].fd << RESET << std::endl;
                }
            }
            else
            {

                std::string message(buffer);

                excute_command(message, _clients[std::to_string(_pollfds[i].fd)], _channels, _clients);

                std::cout
                    << BLUE << "Received from client [" << _pollfds[i].fd << "] : " << message << std::flush;

                // buffer the message in the client class
            }
            //print_map();
        }
        else if (_pollfds[i].revents & POLLHUP)
        {
            // std::cout << YELLOW << "Client " << _pollfds[i].fd << " disconnected." << RESET << std::endl;
            // std::cout << "Total clients is : " << _pollfds.size() - 1 << RESET << std::endl;
            // close(_pollfds[i].fd);
            // _pollfds.erase(_pollfds.begin() + i);
            // _clients.erase(std::to_string(_pollfds[i].fd));
            // --i; // Decrement i to account for the removed socket
        }
    }
}


// void Irc::print_map()
// {

//     for (std::map<std::string, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
//         {
//             std::cout << "Key: " << it->first << ", Value: [print the Client class details here]" << std::endl;
//         }
// }
