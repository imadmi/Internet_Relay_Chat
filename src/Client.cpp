#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"

std::string Client::get_nickname()
{
    return _nickname;
}

int Client::join_channel(Channel &channel)
{
    if (this->_channels.find(channel.get_name()) != _channels.end())
        return (1);
    if (channel.add_client(*this) == 1)
        return (1);
    _channels.insert(std::pair<std::string, Channel>(channel.get_name(), channel));
    return (0);
}

std::string Client::get_username()
{
    return _username;
}

int Client::get_socket_fd()
{
    return _socket_fd;
}

std::string Client::get_socket_fd_str()
{
    return std::to_string(_socket_fd);
}

int Client::kick_user(Client &client, Channel &channel)
{
    // the client is not in the channel
    if (this->_channels.find(channel.get_name()) == _channels.end())
        return (1);
    // the client is not himself
    if (client.get_socket_fd() != this->get_socket_fd())
    {
        channel.remove_client(client);
        return (0);
    }
    return (1);
}

bool Client::is_operator(Channel &channel)
{
    if (channel._operators.find(get_socket_fd()) != channel._operators.end())
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
    channel._operators.insert(std::pair<int, Client>(client.get_socket_fd(), client));
    return (0);
}

int Client::remove_operator(Channel &channel, Client &client)
{
    if (channel.get_clients().find(client.get_socket_fd()) == channel.get_clients().end())
        return (-1);
    channel._operators.erase(client.get_socket_fd());
    return (0);
}

int Client::add_channel(Channel &channel)
{
    if (this->_channels.find(channel.get_name()) != _channels.end())
        return (1);
    _channels.insert(std::pair<std::string, Channel>(channel.get_name(), channel));
    return (0);
}