
#pragma once

/* includes */
#include <iostream>
// #include <sys/types.h>
#include <sys/socket.h>
// #include <netinet/in.h>
#include <arpa/inet.h>
// #include <netdb.h>
// #include <cstring>
// #include <cstdlib>
#include <vector>
#include <map>
#include <poll.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <cerrno>
// #include <ctime>
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

        int _serverSocket, _newSocket;
        struct sockaddr_in _server_addr;

        std::vector<pollfd> _pollfds;
        std::map<int, Client> _clients;

    public:
        std::map<std::string, Channel > _channels;
        Irc(int port, char *password);

        /**
         * @brief socket() Create server socket and initialize it
         *
         * @param INADDR_ANY : the server listening on all available network interfaces
         * @param AF_INET : the addr family for IPv4
         * @param  htons : convert to network byte order (big-endian)
=         * @return zero on success
         */
        void createSocket();

        /**
         * @brief here we allow the server socket fd to be reusable
         *
         * @param AF_INET : IPv4 Internet protocol
         * @param SOCK_STREAM : sock type, ensure reliable, two-way, without loss or duplication connection
         * @param IPPROTO_TCP : tcp rotocol
         * @param SOL_SOCKET : the opteions are the set to the socket level
         * @param SO_REUSEADDR : support the use of the local address
         * if we set the the opt to 0 we will disable this option
         * @return zero on success
         */
        void settingsockopt();

        /**
         * @brief here we set the server socket to be nonbloking
         *
         * @param F_SETFL : file status flag
         * @param O_NONBLOCK : socket operations wait
         * @return zero on success
         */
        void nonBlockFd();

        /**
         * @brief here we set the server socket to be nonbloking
         *
         * @param F_SETFL : file status flag
         * @param O_NONBLOCK : socket operations wait
         * @return zero on success
         */
        void bindSocket();

        // listening for the clients (wait for incoming connection from the client)
        void listeningToClients();

        /**
         * @brief Establish connections and start communication
         *
         * @return poll() returns number of elements in the pollfds whose revents,
         * 0 on timeout -1 on error
         */
        void runServer();

        /**
         * @brief Accept a new client connection
         *
         * @param sockfd : socket that has been listening for connections after a
         * @param addr : socket operations wait
         * @return zero on success
         */
        void addClient();
        void Handle_activity();
        void printc(std::string, std::string, int);
        void recvClientsMsg(Client &, std::string);

};