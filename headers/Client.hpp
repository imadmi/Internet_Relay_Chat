#pragma once
#include "Irc.hpp"

class Channel;

class Client
{
private:
    int _fd;
    int _pwd;

    std::string _buffer;
    int _socket_fd;
    std::string _username;
    std::string _nickname;
    std::string _realname;
    std::string _password;
    bool _is_registered;
    bool _is_authenticated;
    unsigned long _start;
    // unsigned long _end;

public:
    Client(int);
    std::map<std::string, Channel > _channels;

    std::string get_nickname();
    std::string get_username();
    std::string view_topic(Channel &);
    std::string get_socket_fd_str();
    int set_topic(Channel &, std::string);
    bool is_operator(Channel &);
    int get_socket_fd();
    int join_channel(Channel &channel);
    int kick_user(Client &, Channel &);
    bool is_registered();
    bool is_authenticated();
    void set_registered(bool);
    void set_authenticated(bool);
    void set_username(std::string);
    void set_nickname(std::string nickname);

    std::string get_pass()
    {
        return _password;
    };
    void set_pass(std::string pass)
    {
        _password = pass;
    };

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

    void setStart(unsigned long value)
    {
        _start = value;
    }

    unsigned long getStart()
    {
        return _start;
    }
};
