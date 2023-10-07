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
#include "channel.hpp"

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

class Channel;
class Client
{
private:
    // int _socket_fd;
    std::string _username;
    std::string _nickname;
    std::vector<pollfd> poll_fds;
    std::map<std::string, Channel> _channels;

public:
    std::string get_nickname();
    int join_channel(Channel &channel);
};

class Irc
{
private:
    std::map<const int, Client> _clients;

public:
    std::map<std::string, Channel> channels;
    void add_new_client(int client_fd);
    void remove_client(int client_fd);
    Client &get_client(int client_fd);
};