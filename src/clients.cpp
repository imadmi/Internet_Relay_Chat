#include "../headers/irc_header.hpp"
#include "../headers/Channel.hpp"

void Irc::add_new_client(int client_fd)
{
    Client new_client;

    _clients.insert(std::make_pair(client_fd, new_client));
}

void Irc::remove_client(int client_fd)
{
    _clients.erase(client_fd);
}

std::string Client::get_nickname()
{
    return _nickname;
}

int Client::join_channel(std::string channel_name)
{
    if (_channels.find(channel_name) != _channels.end())
        return (1);
    _channels.insert(std::make_pair(channel_name, 1));
    return (0);
}
