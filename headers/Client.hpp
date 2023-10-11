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
    std::string _hostname;
    std::string _servername;
    std::string _buff_to_send;
    std::string _old_nick;
    int _send_flag;
    bool _is_registered;
    bool _is_authenticated;

public:
    Client(int);
    std::map<std::string, Channel> _channels;

    std::string get_nickname();
    std::string get_username();
    std::string view_topic(Channel &);
    std::string get_socket_fd_str();
    std::string get_realname();
    std::string get_hostname();
    std::string get_servername();
    std::string get_old_nick();
    std::string get_buff_to_send();
    int set_topic(Channel &, std::string);
    bool is_operator(Channel &);
    int get_socket_fd();
    int join_channel(Channel &channel);
    int leave_channel(Channel &channel);
    int kick_user(Client &, Channel &);
    bool is_registered();
    bool is_authenticated();
    void set_registered(bool);
    void set_authenticated(bool);
    void set_username(std::string);
    void set_realname(std::string);
    void set_nickname(std::string nickname);
    void set_hostname(std::string hostname);
    void set_servername(std::string servername);
    void add_buffer_to_send(std::string);
    void set_old_nick(std::string);
    void set_buff_to_send(std::string);
    int set_operator(Channel &channel, Client &client);
    int remove_operator(Channel &channel, Client &client);
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
};
