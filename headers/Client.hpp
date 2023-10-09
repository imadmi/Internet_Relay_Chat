#pragma once
#include "irc_header.hpp"

class Channel;

class Client
{
private:
    int _fd;
    int _authenticated;
    int _pwd;

    std::string _buffer;
    int _socket_fd;
    std::string _username;
    std::string _nickname;

public:
    Client(int);
    std::map<std::string, Channel &> _channels;

    std::string get_nickname();
    std::string get_username();
    std::string view_topic(Channel &);
    std::string get_socket_fd_str();
    int set_topic(Channel &, std::string);
    bool is_operator(Channel &);
    int get_socket_fd();
    int join_channel(Channel &channel);
    int kick_user(Client &, Channel &);

    void set_buffer(std::string newValue)
    {
        _buffer = newValue;
    }

    std::string get_buffer()
    {
        return _buffer;
    }

    void addt_buffer(std::string tmp)
    {
        _buffer += tmp;
    }

    void set_fd(int newValue)
    {
        _fd = newValue;
    }

    int get_fd()
    {
        return _fd;
    }
};
