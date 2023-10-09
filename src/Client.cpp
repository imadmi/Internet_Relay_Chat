#include "../headers/irc_header.hpp"
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
    _channels.insert(std::pair<std::string, Channel&>(channel.get_name(), channel));
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