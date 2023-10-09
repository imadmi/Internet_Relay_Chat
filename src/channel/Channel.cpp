

#include "../../headers/Channel.hpp"
#include "../../headers/Irc.hpp"

Channel::Channel(std::string channel_name)
{
    _name = channel_name;
    _modes['i'] = '+';
    _modes['t'] = '+';
    _modes['k'] = '+';
    _modes['o'] = '+';
    _modes['l'] = '+';
}

Channel::~Channel()
{
}

std::string Channel::get_name()
{
    return _name;
}

std::map<int, Client> Channel::get_clients()
{
    return _clients;
}

int Channel::add_client(Client &new_client)
{
    if (this->_clients.find(new_client.get_socket_fd()) != this->_clients.end())
        return (-1);
    this->_clients.insert(std::pair<int, Client>(new_client.get_socket_fd(), new_client));
    return (0);
}

int Channel::remove_client(Client &client)
{
    if (this->_clients.find(client.get_socket_fd()) == this->_clients.end())
        return (-1);
    this->_clients.erase(client.get_socket_fd());
    if (this->_moderators.find(client.get_socket_fd()) != this->_moderators.end())
        this->_moderators.erase(client.get_socket_fd());
    return (0);
}

int Channel::set_topic(std::string topic)
{
    this->_topic = topic;
    return (0);
}

std::string Channel::get_topic()
{
    return (this->_topic);
}

int Channel::set_mode(char mode, char sign)
{
    this->_modes[mode] = sign;
    return (0);
}

std::map<char, char> Channel::get_modes()
{
    return (this->_modes);
}

char Channel::get_signe_mode(char mode)
{
    if (this->_modes.find(mode) == this->_modes.end())
        return (0);
    return (this->_modes[mode]);
}