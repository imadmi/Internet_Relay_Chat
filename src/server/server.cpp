#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"


// unsigned long Irc::get_time(void)
// {
//     struct timeval time;

//     gettimeofday(&time, NULL);
//     return (time.tv_sec * 1000 + time.tv_usec / 1000);
// }

Irc::Irc(int port, char *password)
{
    _passWord = password;
    _port = port;
    _serverName = ":MSN ";

    createSocket();
    settingsockopt();
    nonBlockFd();
    bindSocket();
    listeningToClients();

    std::cout << BLACK << "IRC Server is running on port : " << _port << RESET << std::endl;
}

void Irc::createSocket()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_serverSocket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    _server_addr.sin_addr.s_addr = INADDR_ANY;
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port);
}

void Irc::settingsockopt()
{
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}

void Irc::nonBlockFd()
{
    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}

void Irc::bindSocket()
{

    if (bind(_serverSocket, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

void Irc::listeningToClients()
{
    if (listen(_serverSocket, MAX_CLIENTS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void Irc::runServer()
{
    pollfd serverPoll;
    serverPoll.fd = _serverSocket;
    serverPoll.events = POLLIN;
    _pollfds.push_back(serverPoll);

    while (true)
    {
        if (poll(&_pollfds[0], _pollfds.size(), -1) == -1)
        {
            std::cerr << RED << "Error in poll" << RESET << std::endl;
            exit(EXIT_FAILURE);
        }
        if (_pollfds[0].revents & POLLIN)
            addClient();
        Handle_activity();
    }
}

Client::Client(int fd)
{
    _fd = fd;
    _is_authenticated = false;
    _is_registered = false;
    _pwd = 0;

    _nickname = "";
    _username = "";
    _buffer = "";

    struct timeval time;
    gettimeofday(&time, NULL);
    _start = (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void Irc::handleLogTime(Client &client)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    unsigned long end = (time.tv_sec * 1000 + time.tv_usec / 1000);
    unsigned long seconds = end / 1000;
    unsigned long startSeconds = client.getStart() / 1000;
    unsigned long minutes = (seconds - startSeconds) / 60;
    seconds %= 60;

    std::ostringstream oss;
    oss << minutes << " minutes and " << seconds << " seconds";
    std::string str = oss.str();
    std::string msg = ":@ " + std::to_string(001) + " " + client.get_nickname() + " LOGTIME : " + str + "\n";
    send(client.get_fd(), msg.c_str(), strlen(msg.c_str()), 0);
}

void Irc::addClient()
{
    _newSocket = accept(_serverSocket, NULL, NULL);
    if (_newSocket < 0)
        printc("Error accepting client connection", RED, 1);

    Client new_client(_newSocket);
    pollfd client_pollfd = {_newSocket, POLLIN, 0};
    _pollfds.push_back(client_pollfd);
    _clients.insert(std::pair<int, Client>(_newSocket, new_client));
    std::cout << GREEN << "[Server] Added client #" << _newSocket \
    << " successfully" << RESET << std::endl;
}

void Irc::printc(std::string msg, std::string color, int ex)
{
    std::cout << color << msg << RESET << std::endl;
    if (ex)
        exit(EXIT_SUCCESS);
}

void Irc::Handle_activity()
{
    for (int i = 1; i < _pollfds.size(); i++)
    {
        if (_pollfds[i].revents & POLLIN)
        {
            char buffer[BUFFER_SIZE];
            bzero(buffer, sizeof(buffer));
            int count = recv(_pollfds[i].fd, buffer, BUFFER_SIZE - 1, 0);
            if (count == 0)
            {
                std::cout << YELLOW << "Client " << _pollfds[i].fd << " disconnected.\n";
                std::cout << PURPLE << "Total clients : " << _pollfds.size() - 2 << RESET << std::endl;
            }
            else if (count < 0)
            {
                std::cerr << RED << "Error reading from client " << _pollfds[i].fd << "\n";
                std::cout << PURPLE << "Total clients : " << _pollfds.size() - 2 << RESET << std::endl;
            }
            else
            {
                std::string message(buffer);
                it = _clients.find(_pollfds[i].fd);

                if (it != _clients.end())
                    recvClientsMsg(it->second, message);
                if (it->second.get_buffer().find('\n') != std::string::npos)
                {
                    handleLogTime(it->second);
                    excute_command(it->second.get_buffer(), it->second, _channels, _clients);
                    std::cout << BLUE << "Client [" << it->second.get_fd() << "] : " << it->second.get_buffer() << RESET << std::flush;
                    it->second.set_buffer("");
                }
            }
            if (_pollfds[i].revents & POLLIN && count <= 0)
            {
                close(_pollfds[i].fd);
                _clients.erase(_pollfds[i].fd);
                _pollfds.erase(_pollfds.begin() + i);
            }
        }
    }
}

void Irc::recvClientsMsg(Client &client, std::string buffer)
{
    client.addt_buffer(buffer);
}
