
#pragma once

/* includes */

// #include <sys/types.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <cstring>
// #include <cstdlib>
// #include <cerrno>
#include <ctime>
#include <sys/time.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <poll.h>
#include <unistd.h>
#include <sstream>
#include <string>
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

#define BUFFER_SIZE 20
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
        int _serverSocket, _newSocket ;
        struct sockaddr_in _server_addr;
        std::vector<pollfd> _pollfds;
        std::map<int, Client> _clients;
        std::map<int, Client>::iterator it;

    public:
        std::map<std::string, Channel > _channels;
        
        // Irc constructor
        Irc(int port, char *password);



    std::map<int, Client>& getClients()
    {
        return _clients;
    }



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
         * @param POLLIN : monitor the file descriptor for incoming data that is ready to be read
         * @return zero on success
         */
        void addClient();

        /**
         * @brief check for clients activities and handle them
         *
         * @param flags : in this case '0' , recv behave like read() 
         * @return socket stream sends zero bytes to notifiy the server that the connection has ended
         * -1 when an error occurred 
         * count of bytes sent by the socket
         */
        void Handle_activity();

        // print using a color and % exit
        void printc(std::string, std::string, int);

        // buffer the received message in the client message
        void recvClientsMsg(Client &, std::string);

        // handleLogTime
        static void handleLogTime(Client &);

        // handleBot
        static void handleQuotes(Client &);

        // handleBot
        static void handleBot(Client &);


};