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


    else if (command.substr(0, 4) == "JOIN" && client.is_authenticated())
    {
        join(command, client, channels);
    }
    else if (command.substr(0, 4) == "KICK" && client.is_authenticated())
    {
        kick(command, client, channels);
    }
    // INVITE
    else if (command.substr(0, 6) == "INVITE" && client.is_authenticated())
    {
        invite(command, client, channels, clients);
    }
    else if (command.substr(0, 4) == "MODE" && client.is_authenticated())
    {
        mode(command, client, channels);
    }
    else if (command.substr(0, 7) == "PRIVMSG" && client.is_authenticated())
    {
        privmsg(command, client, clients, channels);
    }
    else if (filteredString(command) == "LOGTIME" && client.is_authenticated())
    {
        Irc::handleLogTime(client);
    }
    else if (filteredString(command) == "QUOTES" && client.is_authenticated())
    {
        Irc::handleQuotes(client);
    }
    else if (filteredString(command) == "DOWNLOAD" && client.is_authenticated())
    {
        Irc::handleBot(client);
    }
    else if (command.substr(0, 5) == "TOPIC" && client.is_authenticated())
    {
        topic(command, client, channels, clients);
    }
    else
    {
        
        send(client.get_fd(), ERR_UNKNOWNCOMMAND(command.substr(0, command.find(" "))).c_str(), ERR_UNKNOWNCOMMAND(command.substr(0, command.find(" "))).length(), 0);
    }


    if (client.is_authenticated())
    {
        send(client.get_fd(), client.get_buff_to_send().c_str(), client.get_buff_to_send().length(), 0);
        client.set_buff_to_send("");
    }
}