#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool client_already_exist(std::string nickname, std::map<int, Client> clients)
{
    std::map<int, Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        Client cl = it->second;
        if (cl.get_nickname() == nickname)
            return true;
    }
    return false;
}

std::string filteredString(std::string str)
{
    std::string filteredString;
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (str[i] != '\n' && str[i] != '\r' && str[i] != '\t' && str[i] != '\v' && str[i] != '\f' && str[i] != ' ')
        {
            filteredString += str[i];
        }
    }
    return filteredString;
}

void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{
    if (command.substr(0, 4) == "PASS")
        pass(command, client, channels, clients);
    else if (command.substr(0, 4) == "NICK")
        nick(command, client, channels, clients);
    else if (command.substr(0, 4) == "USER")
        user(command, client, channels, clients);
    else if (command.substr(0, 4) == "JOIN")
    {
        join(command, client, channels);
    }
    else if (command.substr(0, 4) == "KICK")
    {
        kick(command, client, channels);
    }
    // INVITE
    if (command.substr(0, 6) == "INVITE")
    {
        invite(command, client, channels, clients);
    }
    // if (client.is_authenticated())
    if (1)
    {
        send(client.get_fd(), client.get_buff_to_send().c_str(), client.get_buff_to_send().length(), 0);
        client.set_buff_to_send("");
    }

    if (command.substr(0, 7) == "PRIVMSG" && client.is_authenticated())
    {
        privmsg(command, client, clients, channels);
    }
    if (command.substr(0, 7) == "LOGTIME" && client.is_authenticated())
    {
        Irc::handleLogTime(client);
    }
    if (command.substr(0, 6) == "QUOTES" && client.is_authenticated())
    {
        Irc::handleQuotes(client);
    }
}