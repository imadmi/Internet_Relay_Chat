#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"

std::string Client::get_nickname()
{
    return _nickname;
}

std::string Client::get_username()
{
    return _username;
}

int Client::get_socket_fd()
{
    return _fd;
}

std::string Client::get_socket_fd_str()
{
    return std::to_string(_fd);
}

bool Client::is_operator(Channel &channel)
{
    if (channel.get_operators().find(get_socket_fd()) != channel.get_operators().end())
        return (true);
    return (false);
}

int Client::set_topic(Channel &channel, std::string topic)
{
    if (is_operator(channel) || channel.get_signe_mode('t') == '-')
    {
        channel.set_topic(topic);
        return (0);
    }
    return (1);
}

void Client::set_nickname(std::string nickname)
{
    _nickname = nickname;
}

bool Client::is_registered()
{
    return _is_registered;
}

bool Client::is_authenticated()
{
    return _is_authenticated;
}

void Client::set_registered(bool is_registered)
{
    _is_registered = is_registered;
}

void Client::set_authenticated(bool is_authenticated)
{
    _is_authenticated = is_authenticated;
}

void Client::set_username(std::string username)
{
    _username = username;
}

int Client::set_operator(Channel &channel, Client &client)
{
    if (channel.get_clients().find(client.get_socket_fd()) == channel.get_clients().end())
        return (-1);
    channel.get_operators().insert(std::pair<int, Client>(client.get_socket_fd(), client));
    return (0);
}

int Client::remove_operator(Channel &channel, Client &client)
{
    if (channel.get_clients().find(client.get_socket_fd()) == channel.get_clients().end())
        return (-1);
    channel.get_operators().erase(client.get_socket_fd());
    return (0);
}

void Client::set_realname(std::string realname)
{
    _realname = realname;
}

void Client::set_hostname(std::string hostname)
{
    _hostname = hostname;
}

void Client::set_servername(std::string servername)
{
    _servername = servername;
}

void Client::add_buffer_to_send(std::string buffer)
{
    _buff_to_send += buffer;
}

void Client::set_old_nick(std::string old_nick)
{
    _old_nick = old_nick;
}

std::string Client::get_old_nick()
{
    return _old_nick;
}

std::string Client::get_buff_to_send()
{
    return _buff_to_send;
}

std::string Client::get_realname()
{
    return _realname;
}

std::string Client::get_hostname()
{
    return _hostname;
}

std::string Client::get_servername()
{
    return _servername;
}

void Client::set_buff_to_send(std::string buff)
{
    _buff_to_send = buff;
}
