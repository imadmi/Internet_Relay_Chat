#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"


void broadcastTochannel(Client client , std::string message, std::string channel, std::map<std::string, Channel> &channels)
{
    std::map<std::string, Channel>::iterator it = channels.find(channel);
    if (it != channels.end())
    {
        std::map<int, Client>::iterator it2 = it->second.get_clients().begin();
        while (it2 != it->second.get_clients().end())
        {
            send(it2->second.get_fd(), message.c_str(), message.length(), 0);
            ++it2;
        }
    }
    else 
        send(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel).c_str(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel).length(), 0);
}

std::string parseMessage(std::string message)
{
    std::string parsedMessage;
    std::string::iterator it;
    bool flag = false;
    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it == '\r' || *it == '\n')
        {
            break;
        }
        if (*it == ':')
        {
            flag = true;
            continue;
        }
        if (flag == true)
            parsedMessage += *it;
    }
    return parsedMessage;
}

std::string extractName(std::string message)
{
    std::string name;
    std::string::iterator it;
    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it == '!')
        {
            break;
        }
        name += *it;
    }
    return name;
}
std::string extract_channel_name(std::string message)
{
    std::string channel_name;
    std::string::iterator it;
    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it == ' ')
        {
            break;
        }
        channel_name += *it;
    }
    return channel_name;
}

void privmsg(std::string message, Client &client , std::map<int, Client> &clients, std::map<std::string, Channel> &channels)
{
    std::string receiver = message.find("!") == std::string::npos ? "": filteredString( extractName(message.substr(7, message.length() - 8))) ;
   std::string channel_name =message.find("#") == std::string::npos ? "" :filteredString( extract_channel_name( message.substr(message.find("#") , message.length() - message.find("#") - 2 )))  ;
   std::map<std::string, Channel>::iterator it = channels.find(channel_name);

    std::map<int, Client>::iterator it2;
    if(parseMessage(message).empty())
        send(client.get_fd(), ERR_NOTEXTTOSEND(client.get_nickname()).c_str(), ERR_NOTEXTTOSEND(client.get_nickname()).length(), 0);

    if (!receiver.empty())
    {
        int flag = 0;
        for (it2 = clients.begin(); it2 != clients.end(); ++it2)
        {
            if (it2->second.get_nickname() == receiver)
            {
                send(it2->second.get_fd(), RPL_PRIVMSG(client.get_nickname(), client.get_username(), receiver,  parseMessage(message)).c_str(), RPL_PRIVMSG(client.get_nickname(), client.get_username(), receiver,  parseMessage(message)).length(), 0);
                flag = 1;
                break;
            }
    }
        if (flag == 0)
        {
            send(client.get_fd(), ERR_NOSUCHNICK(client.get_nickname(), receiver).c_str(), ERR_NOSUCHNICK(client.get_nickname(), receiver).length(), 0);
        }   
    }
    else if (!channel_name.empty())
        broadcastTochannel(client,RPL_PRIVMSG(client.get_nickname(), client.get_username(), channel_name,  parseMessage(message)), channel_name, channels);
}