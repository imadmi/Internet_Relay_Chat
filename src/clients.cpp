#include "../headers/irc_header.hpp"
#include "../headers/Channel.hpp"

// void Irc::add_new_client(int client_fd)
// {
//     Client new_client;

//     _clients.insert(std::make_pair(client_fd, new_client));
// }

// void Irc::remove_client(int client_fd)
// {
//     _clients.erase(client_fd);
// }

Client::Client() : _is_registered(false), _is_authenticated(false)
{
}
std::string Client::get_nickname()
{
    return _nickname;
}

void Client::set_nickname(std::string nickname)
{
    _nickname = nickname;
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

std::string Client::get_username()
{
    return _username;
}
// bool client_already_exist(std::string nickname, std::map<std::string, Client> &clients)
// {
//     if (clients.find(nickname) != clients.end())
//         return (true);
//     return (false);
// }

// void Client::nick(std::string messgae){
//     std::string nickname = messgae.substr(6, messgae.length() - 6);
//     //check if nickname is valid
//     //check if nickname is already taken
//     if (client_already_exist(nickname, _clients) == true)
//     {
//         std::cout << "nickname already taken" << std::endl;
//         return;
//     }
//     this->set_nickname(nickname);
//     std::cout << "nickname set to " << nickname << std::endl;
// }

std::string Client::get_pass()
{
    return (_password);
}

void Client::set_pass(std::string pass)
{
    _password = pass;
}
