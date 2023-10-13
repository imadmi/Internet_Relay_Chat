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

void broadcastTochannel_1(Client client , std::string message, std::string channel, std::map<std::string, Channel> &channels)
{
    std::cout << channel << std::endl;
    std::map<std::string, Channel>::iterator it = channels.find(channel);
    if (it != channels.end())
    {
        std::map<int, Client>::iterator it2 = it->second.get_clients().begin();
        while (it2 != it->second.get_clients().end())
        {
            if (it2->second.get_fd() != client.get_fd())
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

std::string extractName(std::string message, int *spaces)
{
    std::string name;
    std::string::iterator it;
    //skip all spaces 
    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it != ' ')
        {
            break;
        }
        *spaces += 1;
    }

    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it == ' ')
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

void privmsg_user(std::string to_send, Client &client , std::map<int, Client> &clients, std::string receiver)
{
    std::map<int, Client>::iterator it2;
    if(to_send.empty())
        send(client.get_fd(), ERR_NOTEXTTOSEND(client.get_nickname()).c_str(), ERR_NOTEXTTOSEND(client.get_nickname()).length(), 0);
    else if (!receiver.empty())
    {
        int flag = 0;
        for (it2 = clients.begin(); it2 != clients.end(); ++it2)
        {
            if (it2->second.get_nickname() == receiver)
            {
                send(it2->second.get_fd(), RPL_PRIVMSG(client.get_nickname(), "USername", receiver, to_send).c_str(), RPL_PRIVMSG(client.get_nickname(), "USername", receiver, to_send).length(), 0);
                flag = 1;
                break;
            }
    }
        if (flag == 0)
        {
            send(client.get_fd(), ERR_NOSUCHNICK(client.get_nickname(), receiver).c_str(), ERR_NOSUCHNICK(client.get_nickname(), receiver).length(), 0);
        }   
    }
}

void privmsg(std::string message, Client &client , std::map<int, Client> &clients, std::map<std::string, Channel> &channels)
{
    int spaces = 0;
    std::string receiver = extractName(message.substr(8, message.length() - 8), &spaces);
   std::string channel_name =receiver.find("#") == std::string::npos ? "" :receiver;
    std::string to_send = message.substr(8 + receiver.length() + 1 + spaces , message.length());
   std::map<std::string, Channel>::iterator it = channels.find(channel_name);

//    std::cout<<RED<< receiver<<std::endl;
    std::map<int, Client>::iterator it2;
    if( receiver.empty() && channel_name.empty())
        send(client.get_fd(), ERR_NORECIPIENT(client.get_nickname()).c_str(), ERR_NORECIPIENT(client.get_nickname()).length(), 0);
    if (!receiver.empty() && channel_name.empty())
        privmsg_user(to_send, client, clients, receiver);
    if (!channel_name.empty())
    {
        std::cout<<RED<< "channel"<<std::endl;
         if(to_send.empty())
            send(client.get_fd(), ERR_NOTEXTTOSEND(client.get_nickname()).c_str(), ERR_NOTEXTTOSEND(client.get_nickname()).length(), 0);
        else
            broadcastTochannel_1(client,RPL_PRIVMSG(client.get_nickname(), "Username", channel_name,  to_send), channel_name, channels);
    }
}

