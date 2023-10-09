#pragma once

/* includes */
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>
#include <poll.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <cerrno>
#include <ctime>
#include "Channel.hpp"
#include <fcntl.h>

/* colors */
#define RESET "\033[0m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define GREEN "\033[0;32m"
#define BLACK "\033[0;30m"
#define WHITE "\033[0;37m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"

#define BUFFER_SIZE 1000
#define MAX_CLIENTS 10
#define PRINT(x) std::cout << x << std::endl;

#include "Client.hpp"
#include "Channel.hpp"

class Client;

class Irc
{
private:
    char *_passWord;
    int _port;
    std::string _serverName;

    // std::string _buffer;

    int _serverSocket, _newSocket;
    struct sockaddr_in _server_addr;

    std::vector<pollfd> _pollfds;
    std::map<int, Client> _clients;

public:
    std::map<std::string, Channel &> _channels;

    Irc(int port, char *password);

    void createSocket();
    void bindSocket();
    void listeningToClients();

    void runServer();

    void addClient();

    void Handle_activity();

    void printc(std::string, std::string, int);

    void buffer_msg();

        void recvClientsMsg(Client &, std::string);
        
        // void add_new_client(int client_fd);
        // void remove_client(int client_fd);
};