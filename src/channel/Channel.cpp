

#include "../../headers/Channel.hpp"
#include "../../headers/Irc.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string channel_name)
{
    _name = channel_name;
    _modes['i'] = '-';
    _modes['t'] = '+';
    _modes['k'] = '-';
    _modes['o'] = '+';
    _modes['l'] = '+';
    _limit = 10;
    _key = "pass";
    _topic = " ";
}

Channel::~Channel()
{
}

std::string Channel::get_name()
{
    return _name;
}

std::map<int, Client> &Channel::get_clients()
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
    int fd = client.get_socket_fd();
    if (this->_clients.find(client.get_socket_fd()) == this->_clients.end())
        return (-1);
    this->_clients.erase(client.get_socket_fd());
    if (this->get_operators().find(fd) != this->get_operators().end())
        this->get_operators().erase(fd);
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

std::string Channel::get_mode_string()
{
    std::string mode_string;
    std::map<char, char>::iterator it = this->_modes.begin();
    while (it != this->_modes.end())
    {
        mode_string += it->second;
        mode_string += it->first;
        it++;
    }
    return (mode_string);
}

int Channel::set_operator(Client &client)
{
    if (this->_clients.find(client.get_socket_fd()) == this->_clients.end())
        return (-1);
    this->get_operators().insert(std::pair<int, Client>(client.get_socket_fd(), client));
    return (0);
}

int Channel::remove_operator(Client &client)
{
    if (this->_clients.find(client.get_socket_fd()) == this->_clients.end())
        return (-1);
    this->get_operators().erase(client.get_socket_fd());
    return (0);
}

std::map<int, Client> &Channel::get_operators()
{
    return (this->_operators);
}

void Channel::print_members()
{
    std::map<int, Client>::iterator it = this->_clients.begin();
    while (it != this->_clients.end())
    {
        it++;
    }
}

void Channel::add_invitee(std::string nickname)
{
    this->_invitees.push_back(nickname);
}

std::vector<std::string> Channel::get_invitees()
{
    return (this->_invitees);
}

std::string Channel::get_clients_nick()
{
    std::string str_list;
    std::map<int, Client>::iterator it = this->_clients.begin();
    while (it != this->_clients.end())
    {
        if (this->get_operators().find(it->first) != this->get_operators().end())
            str_list += "@";
        str_list += it->second.get_nickname();
        if (++it != this->_clients.end())
            str_list += " ";
    }
    return (str_list);
}

std::string Channel::get_key()
{
    return (this->_key);
}

void Channel::set_key(std::string key)
{
    this->_key = key;
}

size_t Channel::get_limit()
{
    return (this->_limit);
}

void Channel::set_limit(int limit)
{
    this->_limit = limit;
}

void Channel::set_topic(std::string topic)
{
    this->_topic = topic;
}
